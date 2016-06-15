void sckBegin() {
  driver.begin();
  driver.writeCurrent(200);
  sckWriteVH(CO_SENSOR, 2700); //VH_CO SENSOR Inicial
  digitalWrite(IO0, HIGH); //VH_CO SENSOR
  sckWriteVH(NO2_SENSOR, 1700); //VH_CO SENSOR Inicial
  digitalWrite(IO1, HIGH); //VH_NO2 SENSOR 
  driver.writeResistor(CO_SENSOR + 2, 100000); //Inicializacion de la carga del CO SENSOR
  driver.writeResistor(NO2_SENSOR + 2, 100000); //Inicializacion de la carga del NO2 SENSOR
  driver.iniADC();
}

void sckConfig(){
  
//  if (!sckCompareDate(__TIME__, driver.readData(EE_ADDR_TIME_VERSION, 0, INTERNAL)))
//  {
//    driver.RTCadjust(driver.sckDate(__DATE__,__TIME__));
//#if debuggEnabled
//    SerialUSB.println(F("Resetting..."));
//#endif
//    for(uint16_t i=0; i<60; i++) driver.writeEEPROM(i, 0x00);  //Borrado de la memoria
//    driver.writeData(EE_ADDR_TIME_VERSION, 0, __TIME__, INTERNAL);
//    driver.writeData(EE_ADDR_TIME_UPDATE, 0, DEFAULT_TIME_UPDATE, INTERNAL);
//    driver.writeData(EE_ADDR_NUMBER_UPDATES, 0, DEFAULT_MIN_UPDATES, INTERNAL);
//  }

}

boolean sckCompareDate(char* text, char* text1)
{
  if ((strlen(text))!=(strlen(text1))) return false;
  else 
  {
    for(int i=0; i<strlen(text); i++)
    {
      if (text[i]!=text1[i]) return false;
    }
  }
  return true;
}

uint16_t sckGetBattery() {
  uint16_t temp = 2*(driver.readADC(3))*VCC/4095.;
  #if !DataRaw 
    temp = map(temp, VAL_MIN_BATTERY, VAL_MAX_BATTERY, 0, 100);
    if (temp>100) temp=100;
    if (temp<0) temp=0;
  #endif
#if debuggSCK
  SerialUSB.print("Vbat: ");
  SerialUSB.print(temp);
  SerialUSB.print(" mV, ");
  SerialUSB.print("Battery level: ");
  SerialUSB.print(temp.);
  SerialUSB.println(" %");
#endif
  return temp; 
}

uint16_t sckGetCharger() {
  uint16_t temp = 2*(driver.readADC(2))*VCC/4095.;
#if debuggSCK
  SerialUSB.print("Charger voltage: ");
  SerialUSB.print(temp);
  SerialUSB.print(" mV, ");
#endif
  return temp; 
}


void sckAccelDefault(void)
{
    // Accelerometer
    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    driver.writeI2C(ACCEL, 0x21, 0x00);
    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    driver.writeI2C(ACCEL, 0x20, 0x57);
    
    // Magnetometer
    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    driver.writeI2C(ACCEL, 0x24, 0x64);
    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    driver.writeI2C(ACCEL, 0x25, 0x20);
    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    driver.writeI2C(ACCEL, 0x26, 0x00);
}

byte accmag[6];

void sckReadAccMag(byte reg, uint32_t* __x, uint32_t* __y, uint32_t* __z)
  {
    Wire.beginTransmission(ACCEL);
    // assert the MSB of the address to get the accelerometer
    // to do slave-transmit subaddress updating.
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(ACCEL, 6);
    for (int i=0; i<6; i++) accmag[i] = Wire.read();
    // combine high and low bytes
    // This no longer drops the lowest 4 bits of the readings from the DLH/DLM/DLHC, which are always 0
    // (12-bit resolution, left-aligned). The D has 16-bit resolution
    *__x = (uint32_t)((int32_t)(accmag[1] << 8 | accmag[0]) + 32768);
    *__y = (uint32_t)((int32_t)(accmag[3] << 8 | accmag[2]) + 32768);
    *__z = (uint32_t)((int32_t)(accmag[5] << 8 | accmag[4]) + 32768);
  }

// Reads the 3 accelerometer channels and stores them in vector a
void sckReadAcc(uint32_t* __x, uint32_t* __y, uint32_t* __z)
{
  sckReadAccMag(OUT_X_L_A | (1 << 7), __x, __y,  __z);
  #if debuggSCK
    SerialUSB.print("Accelerometer x: ");
    SerialUSB.print(*__x);
    SerialUSB.print(" y: ");
    SerialUSB.print(*__y);
    SerialUSB.print(" z: ");
    SerialUSB.println(*__z);
  #endif
}

// Reads the 3 magnetometer channels and stores them in vector m
void sckReadMag(uint32_t* __x, uint32_t* __y, uint32_t* __z)
{
  sckReadAccMag(OUT_X_L_M | (1 << 7), __x, __y,  __z);
  #if debuggSCK
    SerialUSB.print("Magnetometer x: ");
    SerialUSB.print(*__x);
    SerialUSB.print(" y: ");
    SerialUSB.print(*__y);
    SerialUSB.print(" z: ");
    SerialUSB.println(*__z);
  #endif
}







