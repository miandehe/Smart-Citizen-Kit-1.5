/*

  SCKDrivers.h
  Supports core and data management functions (Power, WiFi, SD storage, RTClock and EEPROM storage)

  - Modules supported:

    - WIFI
    - RTC 
    - EEPROM
    - POWER MANAGEMENT IC's

*/

#include "Constants.h"
#include "SCKDrivers.h"
#include <Wire.h>
#include <EEPROM.h>

#define debug false

void SCKDriver::begin() {
  Wire.begin();
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;  
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(IO0, OUTPUT); //VH_CO SENSOR
  pinMode(IO1, OUTPUT); //VH_NO2 SENSOR
  pinMode(IO2, OUTPUT); //NO2 SENSOR_HIGH_IMPEDANCE
  pinMode(AWAKE, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(SHCP, OUTPUT);
  pinMode(CONTROL, INPUT);
  digitalWrite(AWAKE, LOW); 
  digitalWrite(IO0, HIGH); 
  digitalWrite(IO1, HIGH); 
  resetshift();
  writeI2C(CHARGER, 0x04, B10110010); //CHARGE VOLTAGE LIMIT 4208 mV
}

/*RTC commands*/

#define buffer_length        32
static char buffer[buffer_length];

char* SCKDriver::sckDate(const char* date, const char* time){
    int j = 0;
    for  (int i = 7; date[i]!=0x00; i++)
      {
        buffer[j] = date[i];
        j++;
      }
    buffer[j] = '-';
    j++;
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': 
            if (date[1] == 'a') buffer[j] = '1';
            else if (date[2] == 'n') buffer[j] = '6';
            else buffer[j] = '7';
            break; 
        case 'F': 
            buffer[j] = '2'; 
            break;
        case 'A': 
            if (date[1] == 'p') buffer[j] = '4';
            else buffer[j] = '8';
            break;
        case 'M': 
            if (date[2] == 'r') buffer[j] = '3';
            else buffer[j] = '5';
            break;
        case 'S': 
            buffer[j] = '9'; 
            break;
        case 'O': 
            buffer[j] = '1'; 
            buffer[j+1] = '0';
            j++;
            break;
        case 'N': 
            buffer[j] = '1'; 
            buffer[j+1] = '1';
            j++;
            break;
        case 'D': 
            buffer[j] = '1'; 
            buffer[j+1] = '2';
            j++;
            break;
    }
  j++;
  buffer[j] = '-';
  j++;
  byte k = 4;
  if (date[4] == ' ')  k = 5;
  for  (int i = k; date[i]!=' '; i++)
      {
        buffer[j] = date[i];
        j++;
      }
  buffer[j] = ' ';
  j++;
  for  (int i = 0; time[i]!=0x00; i++)
    {
      buffer[j] = time[i];
      j++;
    }
  buffer[j]=0x00;   
  return buffer;
}

boolean SCKDriver::RTCadjust(char *time) {    
  byte rtc[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte count = 0x00;
  byte data_count=0;
  while (time[count]!=0x00)
  {
    if(time[count] == '-') data_count++;
    else if(time[count] == ' ') data_count++;
    else if(time[count] == ':') data_count++;
    else if ((time[count] >= '0')&&(time[count] <= '9'))
    { 
      rtc[data_count] =(rtc[data_count]<<4)|(time[count]-'0');
    }  
    else break;
    count++;
  }  
  if (data_count == 5)
  {
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write((int)0);
    Wire.write(rtc[5]|0x80); //0x00 SECOND
    Wire.write(rtc[4]); //0x01 MINUTES
    Wire.write(rtc[3]); //0x02 HOUR
    Wire.write(0x00|0x08);   //0x03 DAY
    Wire.write(rtc[2]); //0x04 DATE
    Wire.write(rtc[1]); //0x05 MONTH
    Wire.write(rtc[0]); //0x06 YEAR
    Wire.endTransmission();
    //delay(4);    
    return true;
  }
  return false;  
}

char timeRTC[20] = {'2', '0', '0', '0', '-', '0', '0', '-', '0', '0', ' ', '0', '0', ':', '0', '0', ':', '0', '0', 0x00};

boolean SCKDriver::RTCtime(char *time) {
  uint8_t date[6] = {0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0x7F};
  for (int i=0; i<20; i++) time[i] = timeRTC[i];
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((int)0);  
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7);
  for (int i=5; i>2; i--) date[i] = (Wire.read() & date[i]);
  Wire.read();
  for (int i=2; i>=0; i--) date[i] = (Wire.read() & date[i]);
  for (int i=0; i<6; i++) 
    {
       time[2+3*i] =  (date[i]>>4) + '0';
       time[3+3*i] =  (date[i]&0x0F) + '0';
    }
  return true;
}

char* SCKDriver::RTCtime() {
  uint8_t date[6] = {0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0x7F};
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write((int)0);  
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDRESS, 7);
  for (int i=5; i>2; i--) date[i] = (Wire.read() & date[i]);
  Wire.read();
  for (int i=2; i>=0; i--) date[i] = (Wire.read() & date[i]);
  for (int i=0; i<6; i++) 
    {
       timeRTC[2+3*i] =  (date[i]>>4) + '0';
       timeRTC[3+3*i] =  (date[i]&0x0F) + '0';
    }
  return timeRTC;
}

/*Inputs an outputs control*/

byte val_shift = B00000000;

void SCKDriver::resetshift()
{
   digitalWrite(STCP, LOW);
   shiftOut(DS, SHCP, MSBFIRST, val_shift);
   digitalWrite(STCP, HIGH);
}

void SCKDriver::shiftWrite(int pin, boolean state)
{
  if ((pin < 8)&&(pin >= 0))
  {
    bitWrite(val_shift, pin, state);
    digitalWrite(STCP, LOW);
    shiftOut(DS, SHCP, MSBFIRST, val_shift);
    digitalWrite(STCP, HIGH);
  }
}

float SCKDriver::average(int anaPin) {
  int lecturas = 100;
  long total = 0;
  float average = 0;
  for(int i=0; i<lecturas; i++)
  {
    //delay(1);
    total = total + analogRead(anaPin);
  }
  average = (float)total / lecturas;  
  return(average);
}

int SCKDriver::levelRead(boolean state) {
    shiftWrite(SEL_MUX, state);
    delay(10);
    return 2*(3300/1023.)*average(LEVEL);
}



/*EEPROM commands*/

void SCKDriver::writeEEPROM(uint16_t eeaddress, uint8_t data) {
  uint8_t retry = 0;
  while ((readEEPROM(eeaddress)!=data)&&(retry<10))
  {  
    Wire.beginTransmission(E2PROM);
    Wire.write((byte)(eeaddress >> 8));   // MSB
    Wire.write((byte)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    delay(6);
    retry++;
  }
}

byte SCKDriver::readEEPROM(uint16_t eeaddress) {
  byte rdata = 0xFF;
  Wire.beginTransmission(E2PROM);
  Wire.write((byte)(eeaddress >> 8));   // MSB
  Wire.write((byte)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(E2PROM,1);
  while (!Wire.available()); 
  rdata = Wire.read();
  return rdata;
}

void SCKDriver::writeData(uint32_t eeaddress, long data, uint8_t location)
{
    for (int i =0; i<4; i++) 
      {
        if (location == EXTERNAL) writeEEPROM(eeaddress + (3 -i) , data>>(i*8));
        else EEPROM.write(eeaddress + (3 -i), data>>(i*8));
      }

}

void SCKDriver::writeData(uint32_t eeaddress, uint16_t pos, char* text, uint8_t location)
{
  uint16_t eeaddressfree = eeaddress + buffer_length * pos;
  if (location == EXTERNAL)
    {
      for (uint16_t i = eeaddressfree; i< (eeaddressfree + buffer_length); i++) writeEEPROM(i, 0x00);
      for (uint16_t i = eeaddressfree; text[i - eeaddressfree]!= 0x00; i++) writeEEPROM(i, text[i - eeaddressfree]);
    }
  else
    {
      
      for (uint16_t i = eeaddressfree; i< (eeaddressfree + buffer_length); i++) EEPROM.write(i, 0x00);
      for (uint16_t i = eeaddressfree; text[i - eeaddressfree]!= 0x00; i++) 
        {
          //if (eeaddressfree>=DEFAULT_ADDR_SSID) if (text[i - eeaddressfree]==' ') text[i - eeaddressfree]='$';
          if (text[i - eeaddressfree]==' ') text[i - eeaddressfree]='$';
          EEPROM.write(i, text[i - eeaddressfree]); 
        }
    }
}

uint32_t SCKDriver::readData(uint16_t eeaddress, uint8_t location)
{
  uint32_t data = 0;
  for (int i =0; i<4; i++)
    {
      if (location == EXTERNAL)  data = data + (uint32_t)((uint32_t)readEEPROM(eeaddress + i)<<((3-i)*8));
      else data = data + (uint32_t)((uint32_t)EEPROM.read(eeaddress + i)<<((3-i)*8));
    }
  return data;
}

char* SCKDriver::readData(uint16_t eeaddress, uint16_t pos, uint8_t location)
{
  eeaddress = eeaddress + buffer_length * pos;
  uint16_t i;
  if (location == EXTERNAL)
    {
      uint8_t temp = readEEPROM(eeaddress);
      for ( i = eeaddress; ((temp!= 0x00)&&(temp<0x7E)&&(temp>0x1F)&&((i - eeaddress)<buffer_length)); i++) 
      {
        buffer[i - eeaddress] = readEEPROM(i);
        temp = readEEPROM(i + 1);
      }
    }
  else
    {
      uint8_t temp = EEPROM.read(eeaddress);
      for ( i = eeaddress; ((temp!= 0x00)&&(temp<0x7E)&&(temp>0x1F)&&((i - eeaddress)<buffer_length)); i++) 
      {
        buffer[i - eeaddress] = EEPROM.read(i);
        temp = EEPROM.read(i + 1);
      }
    }
  buffer[i - eeaddress] = 0x00; 
  return buffer;
}

/*ESP8266 commands*/

void SCKDriver::ESPini()
  {
     shiftWrite(CH_PD, HIGH);
     shiftWrite(P_WIFI, LOW);
     shiftWrite(RST_ESP, HIGH);
     shiftWrite(GPIO0, HIGH);
  }

void SCKDriver::ESPflash()
  {
     shiftWrite(CH_PD, HIGH);
     shiftWrite(P_WIFI, LOW);
     shiftWrite(RST_ESP, HIGH);
     shiftWrite(GPIO0, LOW);
  }
  
void SCKDriver::ESPoff()
  {
     shiftWrite(CH_PD, LOW);
     shiftWrite(P_WIFI, HIGH);
     shiftWrite(RST_ESP, LOW);
     shiftWrite(GPIO0, LOW);
  }

//Charger commands*/

void SCKDriver::chargerMode(boolean state1, boolean state2)
  {
    shiftWrite(PSEL, state1);  //Power source selection input. High indicates a USB host source and Low indicates an adapter source.
    shiftWrite(OTG, state2);
  }

    
/*Potenciometer*/ 

void SCKDriver::writeI2C(byte deviceaddress, byte address, byte data ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
  delay(4);
}

void SCKDriver::writeResistor(byte resistor, float value ) {
   byte POT = POT1;
   byte ADDR = resistor;
   int data=0x00;
   if (value>100000) value = 100000;
   data = (int)(value/kr);
   if ((resistor==2)||(resistor==3))
     {
       POT = POT2;
       ADDR = resistor - 2;
     }
   else if ((resistor==4)||(resistor==5))
     {
       POT = POT3;
       ADDR = resistor - 4;
     }
   writeI2C(POT, ADDR, data);
}

byte SCKDriver::readI2C(int deviceaddress, byte address ) {
  byte rdata = 0xFF;
  byte  data = 0x0000;
  Wire.beginTransmission(deviceaddress);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  unsigned long time = millis();
  while (!Wire.available()) if ((millis() - time)>500) return 0x00;
  data = Wire.read(); 
  return data;
}   

float SCKDriver::readResistor(byte resistor ) {
   byte POT = POT1;
   byte ADDR = resistor;
   if ((resistor==2)||(resistor==3))
     {
       POT = POT2;
       ADDR = resistor - 2;
     }
   else if ((resistor==4)||(resistor==5))
     {
       POT = POT3;
       ADDR = resistor - 4;
     }
   return readI2C(POT, ADDR)*kr;
}   


