#include "LSM303.h"
#include <Wire.h>
#include <math.h>

// Defines ////////////////////////////////////////////////////////////////

#define TEST_REG_ERROR -1

#define D_WHO_ID    0x49
#define DLM_WHO_ID  0x3C

// Constructors ////////////////////////////////////////////////////////////////

LSM303::LSM303(void)
{
  /*
  These values lead to an assumed magnetometer bias of 0.
  Use the Calibrate example program to determine appropriate values
  for your particular unit. The Heading example demonstrates how to
  adjust these values in your own sketch.
  */
  m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};

  io_timeout = 0;  // 0 = no timeout
  did_timeout = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Did a timeout occur in readAcc(), readMag(), or read() since the last call to timeoutOccurred()?
bool LSM303::timeoutOccurred()
{
  bool tmp = did_timeout;
  did_timeout = false;
  return tmp;
}

void LSM303::setTimeout(unsigned int timeout)
{
  io_timeout = timeout;
}

unsigned int LSM303::getTimeout()
{
  return io_timeout;
}

bool LSM303::init()
{ 
  // set device addresses and translated register addresses
  acc_address = 0x1D;//(sa0 == sa0_high) ? D_SA0_HIGH_ADDRESS : D_SA0_LOW_ADDRESS;
  translated_regs[-OUT_X_L_M] = D_OUT_X_L_M;
  translated_regs[-OUT_X_H_M] = D_OUT_X_H_M;
  translated_regs[-OUT_Y_L_M] = D_OUT_Y_L_M;
  translated_regs[-OUT_Y_H_M] = D_OUT_Y_H_M;
  translated_regs[-OUT_Z_L_M] = D_OUT_Z_L_M;
  translated_regs[-OUT_Z_H_M] = D_OUT_Z_H_M;
  return true;
}

void LSM303::enableDefault(void)
{
    // Accelerometer
    
    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    writeReg(0x21, 0x00);

    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    writeReg(0x20, 0x57);

    // Magnetometer

    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    writeReg(0x24, 0x64);

    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    writeReg(0x25, 0x20);

    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    writeReg(0x26, 0x00);
}


      
// Writes an accelerometer register
void LSM303::writeReg(byte reg, byte value)
{
  Wire.beginTransmission(acc_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Reads an accelerometer register
byte LSM303::readAccReg(byte reg)
{
  byte value;
  Wire.beginTransmission(acc_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(acc_address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();
  return value;
}

// Reads a magnetometer register
byte LSM303::readMagReg(int reg)
{
  byte value;

  // if dummy register address (magnetometer Y/Z), look up actual translated address (based on device type)
  if (reg < 0)
  {
    reg = translated_regs[-reg];
  }
  Wire.beginTransmission(acc_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(acc_address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM303::readAcc(void)
{
  Wire.beginTransmission(acc_address);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  Wire.write(OUT_X_L_A | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(acc_address, (byte)6);

  unsigned int millis_start = millis();
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }

  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  // combine high and low bytes
  // This no longer drops the lowest 4 bits of the readings from the DLH/DLM/DLHC, which are always 0
  // (12-bit resolution, left-aligned). The D has 16-bit resolution
  a.x = (int16_t)(xha << 8 | xla);
  a.y = (int16_t)(yha << 8 | yla);
  a.z = (int16_t)(zha << 8 | zla);
}

// Reads the 3 magnetometer channels and stores them in vector m
void LSM303::readMag(void)
{
  Wire.beginTransmission(acc_address);
  // If LSM303D, assert MSB to enable subaddress updating
  // OUT_X_L_M comes first on D, OUT_X_H_M on others
  Wire.write((true) ? translated_regs[-OUT_X_L_M] | (1 << 7) : translated_regs[-OUT_X_H_M]);
  Wire.endTransmission();
  Wire.requestFrom(acc_address, (byte)6);

  unsigned int millis_start = millis();
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }

  byte xlm, xhm, ylm, yhm, zlm, zhm;

    // D: X_L, X_H, Y_L, Y_H, Z_L, Z_H
    xlm = Wire.read();
    xhm = Wire.read();
    ylm = Wire.read();
    yhm = Wire.read();
    zlm = Wire.read();
    zhm = Wire.read();

  // combine high and low bytes
  m.x = (int16_t)(xhm << 8 | xlm);
  m.y = (int16_t)(yhm << 8 | ylm);
  m.z = (int16_t)(zhm << 8 | zlm);
}

// Reads all 6 channels of the LSM303 and stores them in the object variables
void LSM303::read(void)
{
  readAcc();
  readMag();
}


