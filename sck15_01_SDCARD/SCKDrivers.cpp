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
//#include <EEPROM.h>

/* Create an rtc object */
RTCZero rtc;

#define debug false

void SCKDriver::begin() {
  Wire.begin(); 
  SerialUSB.begin(115200);
  Serial.begin(115200);
  pinMode(IO0, OUTPUT); //VH_CO SENSOR
  pinMode(IO1, OUTPUT); //VH_NO2 SENSOR
  pinMode(IO2, OUTPUT); //NO2 SENSOR_HIGH_IMPEDANCE
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(CH_PD, OUTPUT);
  pinMode(P_WIFI, OUTPUT);
  pinMode(RST_ESP, OUTPUT);
  pinMode(GPIO0, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(CONTROL, INPUT);
  digitalWrite(IO0, HIGH); 
  digitalWrite(IO1, HIGH); 
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);  
}

/*Sensor temperature*/
  
uint16_t SCKDriver::readSHT(uint8_t type){
      uint16_t DATA = 0;
      Wire.beginTransmission(Temperature);
      Wire.write(type);
      Wire.endTransmission();
      Wire.requestFrom(Temperature,3);
      unsigned long time = millis();
      while (!Wire.available()) if ((millis() - time)>500) return 0x00;
      DATA = Wire.read()<<8; 
      DATA += Wire.read(); 
      Wire.read();
      DATA &= ~0x0003; 
      return DATA;
  }
  
void SCKDriver::getSHT(float* __Temperature, float* __Humidity)
   {
        *__Temperature = (-46.85 + (175.72*(readSHT(0xE3)/65536.0)));
        *__Humidity    = (-6 + (125*(readSHT(0xE5)/65536.0)));   
    }
    
/*RTC commands*/

void SCKDriver::RTCini()
  {
    rtc.begin(); // initialize RTC 24H format
  }
  
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
  char rtcval[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  byte count = 0x02;
  byte data_count=0;
  while (time[count]!=0x00)
  {
    if(time[count] == '-') data_count++;
    else if(time[count] == ' ') data_count++;
    else if(time[count] == ':') data_count++;
    else if ((time[count] >= '0')&&(time[count] <= '9'))
    { 
      //rtcval[data_count] =(rtcval[data_count]<<4)|(time[count]-'0');
      rtcval[data_count] =(rtcval[data_count]*10)+(time[count]-'0');
    }  
    else break;
    count++;
  }  
  if (data_count == 5)
  {
    rtc.setTime(rtcval[3], rtcval[4], rtcval[5]);
    rtc.setDate(rtcval[2], rtcval[1], rtcval[0]);
    return true;
  }
  return false;  
}

char timeRTC[20] = {'2', '0', '0', '0', '-', '0', '0', '-', '0', '0', ' ', '0', '0', ':', '0', '0', ':', '0', '0', 0x00};

boolean SCKDriver::RTCtime(char *time) {
  uint8_t date[6] = {rtc.getYear(),rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds()};
  for (int i=0; i<20; i++) time[i] = timeRTC[i];
  for (int i=0; i<6; i++) 
    {
       time[2+3*i] =  (date[i]/10) + '0';
       time[3+3*i] =  (date[i]%10) + '0';
    }
  return true;
}

char* SCKDriver::RTCtime() {
  uint8_t date[6] = {rtc.getYear(),rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds()};
  for (int i=0; i<6; i++) 
    {
       timeRTC[2+3*i] =  (date[i]/10) + '0';
       timeRTC[3+3*i] =  (date[i]%10) + '0';
    }
  return timeRTC;
}

/*Inputs an outputs control*/

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


/*EEPROM commands*/

//void SCKDriver::writeEEPROM(uint16_t eeaddress, uint8_t data) {
//  uint8_t retry = 0;
//  while ((readEEPROM(eeaddress)!=data)&&(retry<10))
//  {  
//    Wire.beginTransmission(E2PROM);
//    Wire.write((byte)(eeaddress >> 8));   // MSB
//    Wire.write((byte)(eeaddress & 0xFF)); // LSB
//    Wire.write(data);
//    Wire.endTransmission();
//    delay(6);
//    retry++;
//  }
//}
//
//byte SCKDriver::readEEPROM(uint16_t eeaddress) {
//  byte rdata = 0xFF;
//  Wire.beginTransmission(E2PROM);
//  Wire.write((byte)(eeaddress >> 8));   // MSB
//  Wire.write((byte)(eeaddress & 0xFF)); // LSB
//  Wire.endTransmission();
//  Wire.requestFrom(E2PROM,1);
//  while (!Wire.available()); 
//  rdata = Wire.read();
//  return rdata;
//}
//
//void SCKDriver::writeData(uint32_t eeaddress, long data, uint8_t location)
//{
//    for (int i =0; i<4; i++) 
//      {
//        if (location == EXTERNAL) writeEEPROM(eeaddress + (3 -i) , data>>(i*8));
//        else EEPROM.write(eeaddress + (3 -i), data>>(i*8));
//      }
//
//}
//
//void SCKDriver::writeData(uint32_t eeaddress, uint16_t pos, char* text, uint8_t location)
//{
//  uint16_t eeaddressfree = eeaddress + buffer_length * pos;
//  if (location == EXTERNAL)
//    {
//      for (uint16_t i = eeaddressfree; i< (eeaddressfree + buffer_length); i++) writeEEPROM(i, 0x00);
//      for (uint16_t i = eeaddressfree; text[i - eeaddressfree]!= 0x00; i++) writeEEPROM(i, text[i - eeaddressfree]);
//    }
//  else
//    {
//      
//      for (uint16_t i = eeaddressfree; i< (eeaddressfree + buffer_length); i++) EEPROM.write(i, 0x00);
//      for (uint16_t i = eeaddressfree; text[i - eeaddressfree]!= 0x00; i++) 
//        {
//          //if (eeaddressfree>=DEFAULT_ADDR_SSID) if (text[i - eeaddressfree]==' ') text[i - eeaddressfree]='$';
//          if (text[i - eeaddressfree]==' ') text[i - eeaddressfree]='$';
//          EEPROM.write(i, text[i - eeaddressfree]); 
//        }
//    }
//}
//
//uint32_t SCKDriver::readData(uint16_t eeaddress, uint8_t location)
//{
//  uint32_t data = 0;
//  for (int i =0; i<4; i++)
//    {
//      if (location == EXTERNAL)  data = data + (uint32_t)((uint32_t)readEEPROM(eeaddress + i)<<((3-i)*8));
//      else data = data + (uint32_t)((uint32_t)EEPROM.read(eeaddress + i)<<((3-i)*8));
//    }
//  return data;
//}
//
//char* SCKDriver::readData(uint16_t eeaddress, uint16_t pos, uint8_t location)
//{
//  eeaddress = eeaddress + buffer_length * pos;
//  uint16_t i;
//  if (location == EXTERNAL)
//    {
//      uint8_t temp = readEEPROM(eeaddress);
//      for ( i = eeaddress; ((temp!= 0x00)&&(temp<0x7E)&&(temp>0x1F)&&((i - eeaddress)<buffer_length)); i++) 
//      {
//        buffer[i - eeaddress] = readEEPROM(i);
//        temp = readEEPROM(i + 1);
//      }
//    }
//  else
//    {
//      uint8_t temp = EEPROM.read(eeaddress);
//      for ( i = eeaddress; ((temp!= 0x00)&&(temp<0x7E)&&(temp>0x1F)&&((i - eeaddress)<buffer_length)); i++) 
//      {
//        buffer[i - eeaddress] = EEPROM.read(i);
//        temp = EEPROM.read(i + 1);
//      }
//    }
//  buffer[i - eeaddress] = 0x00; 
//  return buffer;
//}

/*ESP8266 commands*/

void SCKDriver::ESPini()
  {
     digitalWrite(CH_PD, HIGH);
     digitalWrite(P_WIFI, LOW);
     digitalWrite(RST_ESP, HIGH);
     digitalWrite(GPIO0, HIGH);
  }

void SCKDriver::ESPflash()
  {
     digitalWrite(CH_PD, HIGH);
     digitalWrite(P_WIFI, LOW);
     digitalWrite(RST_ESP, HIGH);
     digitalWrite(GPIO0, LOW);
  }
  
void SCKDriver::ESPoff()
  {
     digitalWrite(CH_PD, LOW);
     digitalWrite(P_WIFI, HIGH);
     digitalWrite(RST_ESP, LOW);
     digitalWrite(GPIO0, LOW);
  }

//Charger commands*/

void SCKDriver::writeCurrent(int current)
  {
    int resistor = (4000000/current)-96-3300;
    writeResistor(0, resistor);
  }

float SCKDriver::readCurrent()
  {
    float current = 4000/(readResistor(0) + 3300 + 96)*1000;
    return current;
  }  

boolean SCKDriver::StatusCharge()
  {
    if (readADC(0)>2047) return true;
    else return false;
  }
  
float SCKDriver::readCurrentCharge()
  {
    return (readCurrent()/1000*(readADC(1))*VCC/4095.);
  }
     
/*Potenciometer*/ 

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
   else if ((resistor==6)||(resistor==7))
     {
       POT = POT4;
       ADDR = resistor - 6;
     }
   writeI2C(POT, ADDR, data);
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
   else if ((resistor==6)||(resistor==7))
     {
       POT = POT4;
       ADDR = resistor - 6;
     }
   return readI2C(POT, ADDR)*kr;
}   
  
 uint16_t SCKDriver::readADC(byte channel)
  {
    byte dir[4] = {2,4,6,8};
    byte temp = readI2C(ADC_DIR,0)|B11000000;
    writeI2C(ADC_DIR, 0, temp);
    
    delay(100);
    uint16_t data = (readI2C(ADC_DIR, dir[channel])<<4) + (readI2C(ADC_DIR, dir[channel] + 1)>>4);
    delay(100);
    return data;
  }

void SCKDriver::ADCini()
  {
    byte temp = readI2C(ADC_DIR,0)&B00000011;
    writeI2C(ADC_DIR, 0, temp);
  }


/*Gas sensor*/ 

  void SCKDriver::writeVH(byte device, long voltage ) {
    float resistor = ((voltage/800.)-1)*R2;
    writeResistor(device, resistor);
  }

  
  float SCKDriver::readVH(byte device) {
    float resistor = readResistor(device);
    float voltage = ((resistor/R2) + 1)*800;
    return(voltage);
  }
  
  void SCKDriver::currentHeat(byte device, int current)
  {
    float Rc=Rc0;
    byte Sensor = S2;
    if (device == NO2_SENSOR) { Rc=Rc1; Sensor = S3;}

    float Vc = (float)average(Sensor)*VCC/1023; //mV 
    float current_measure = Vc/Rc; //mA 
    float Rh = (readVH(device)- Vc)/current_measure;
    float Vh = (Rh + Rc)*current;

    writeVH(device, Vh);
      #if debuggSCK
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR current: ");
        else SerialUSB.print("CO SENSOR corriente: ");
        SerialUSB.print(current_measure);
        SerialUSB.println(" mA");
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR correction VH: ");
        else  SerialUSB.print("CO SENSOR correccion VH: ");
        SerialUSB.print(readVH(device));
        SerialUSB.println(" mV");
        Vc = (float)average(Sensor)*VCC/1023; //mV 
        current_measure = Vc/Rc; //mA 
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR corrected current: ");
        else SerialUSB.print("CO SENSOR corrected current: ");
        SerialUSB.print(current_measure);
        SerialUSB.println(" mA");
        SerialUSB.println("Heating...");
      #endif
    
  }

   float SCKDriver::readRs(byte device)
   {
     byte Sensor = S0;
     if (device == NO2_SENSOR) {Sensor = S1; }
     float RL = readResistor(device + 2); //Ohm
     float VL = ((float)average(Sensor)*VCC)/1023; //mV
     if (VL > VCC) VL = VCC;
     float Rs = ((VCC-VL)/VL)*RL; //Ohm
     #if debuggSCK
        if (device == CO_SENSOR) SerialUSB.print("CO SENSOR VL: ");
        else SerialUSB.print("NO2 SENSOR VL: ");
        SerialUSB.print(VL);
        SerialUSB.print(" mV, RS: ");
        SerialUSB.print(Rs);
        SerialUSB.print(" Ohm, RL: ");
        SerialUSB.print(RL);
        SerialUSB.println(" Ohm");
     #endif;  
     return Rs;
   }
   
  float SCKDriver::readMICS(byte device)
  {
      float Rs = readRs(device);
      float RL = readResistor(device + 2); //Ohm
      /*Correccion de impedancia de carga*/
      if ((Rs <= (RL - 1000))||(Rs >= (RL + 1000)))
      {
        if (Rs < 2000) writeResistor(device + 2, 2000);
        else writeResistor(device + 2, Rs);
        delay(100);
        Rs = readRs(device);
      }
       return Rs;
  }
  
  void SCKDriver::getMICS(float* __RsCO, float* __RsNO2){          
       
        /*Correccion de la tension del Heather*/
        currentHeat(CO_SENSOR, 32); //Corriente en mA
        currentHeat(NO2_SENSOR, 26); //Corriente en mA
        
        *__RsCO = readMICS(CO_SENSOR);
        *__RsNO2 = readMICS(NO2_SENSOR);
         
  }

/*Light sensor*/ 
  uint16_t SCKDriver::getLight(){
      uint8_t TIME0  = 0xDA;
      uint8_t GAIN0 = 0x00;
      uint8_t DATA [8] = {0x03, TIME0, 0x00 ,0x00, 0x00, 0xFF, 0xFF ,GAIN0} ;
      
      uint16_t DATA0 = 0;
      uint16_t DATA1 = 0;
      
      Wire.beginTransmission(BH1730);
      Wire.write(0x80|0x00);
      for(int i= 0; i<8; i++) Wire.write(DATA[i]);
      Wire.endTransmission();
      delay(100); 
      Wire.beginTransmission(BH1730);
      Wire.write(0x94);  
      Wire.endTransmission();
      Wire.requestFrom(BH1730, 4);
      DATA0 = Wire.read();
      DATA0=DATA0|(Wire.read()<<8);
      DATA1 = Wire.read();
      DATA1=DATA1|(Wire.read()<<8);
        
      uint8_t Gain = 0x00; 
      if (GAIN0 == 0x00) Gain = 1;
      else if (GAIN0 == 0x01) Gain = 2;
      else if (GAIN0 == 0x02) Gain = 64;
      else if (GAIN0 == 0x03) Gain = 128;
      
      float ITIME =  (256- TIME0)*2.7;
      
      float Lx = 0;
      float cons = (Gain * 100) / ITIME;
      float comp = (float)DATA1/DATA0;

      
      if (comp<0.26) Lx = ( 1.290*DATA0 - 2.733*DATA1 ) / cons;
      else if (comp < 0.55) Lx = ( 0.795*DATA0 - 0.859*DATA1 ) / cons;
      else if (comp < 1.09) Lx = ( 0.510*DATA0 - 0.345*DATA1 ) / cons;
      else if (comp < 2.13) Lx = ( 0.276*DATA0 - 0.130*DATA1 ) / cons;
      else Lx=0;
      
       #if debuggSCK
        SerialUSB.print("BH1730: ");
        SerialUSB.print(Lx);
        SerialUSB.println(" Lx");
      #endif
     return Lx;
  }

/*Audio sensor*/ 
  void SCKDriver::writeGAIN(float GAIN1, float GAIN2)
  {
    writeResistor(6, GAIN1);
    delay(20);
    writeResistor(7, GAIN2);
  }

  float SCKDriver::readGAIN()
  {
    return ((22000/(readResistor(6)+2440)+1)*(62000/readResistor(7)+1));
  }    
 
  float SCKDriver::getNoise() {  
    #define GAIN1 0
    #define GAIN2 400
    writeGAIN(GAIN1,GAIN2);  
    float mVRaw = (float)((average(S4))/1023.)*VCC;
    float dB = 0;
    
    #if debuggSCK
      SerialUSB.print("nOISE: ");
      SerialUSB.print(mVRaw);
      SerialUSB.print(" mV, RSpu: ");
      SerialUSB.print(readResistor(6));
      SerialUSB.print(", Ramp: ");
      SerialUSB.print(readResistor(7));
      SerialUSB.print(", GAIN: ");
      SerialUSB.println(readGAIN());
    #endif
    return mVRaw; 
  }


  boolean SCKDriver::beginUV(void) {
  Wire.begin();
 
  uint8_t id = readI2C(SI1145, SI1145_REG_PARTID);
  if (id != 0x45) return false; // look for SI1145
  
  reset();
  

    /***********************************/
  // enable UVindex measurement coefficients!
  writeI2C(SI1145, SI1145_REG_UCOEFF0, 0x29);
  writeI2C(SI1145, SI1145_REG_UCOEFF1, 0x89);
  writeI2C(SI1145, SI1145_REG_UCOEFF2, 0x02);
  writeI2C(SI1145, SI1145_REG_UCOEFF3, 0x00);

  // enable UV sensor
  writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV |
  SI1145_PARAM_CHLIST_ENALSIR | SI1145_PARAM_CHLIST_ENALSVIS |
  SI1145_PARAM_CHLIST_ENPS1);
  // enable interrupt on every sample
  writeI2C(SI1145, SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);  
  writeI2C(SI1145, SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);  

/****************************** Prox Sense 1 */

  // program LED current
  writeI2C(SI1145, SI1145_REG_PSLED21, 0x03); // 20mA for LED 1 only
  writeParam(SI1145_PARAM_PS1ADCMUX, SI1145_PARAM_ADCMUX_LARGEIR);
  // prox sensor #1 uses LED #1
  writeParam(SI1145_PARAM_PSLED12SEL, SI1145_PARAM_PSLED12SEL_PS1LED1);
  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_PSADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_PSADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in prox mode, high range
  writeParam(SI1145_PARAM_PSADCMISC, SI1145_PARAM_PSADCMISC_RANGE|
    SI1145_PARAM_PSADCMISC_PSMODE);

  writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);  
  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in high range mode
  writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);



  // fastest clocks, clock div 1
  writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);
  // take 511 clocks to measure
  writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
  // in high range mode (not normal signal)
  writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);


/************************/

  // measurement rate for auto
  writeI2C(SI1145, SI1145_REG_MEASRATE0, 0xFF); // 255 * 31.25uS = 8ms
  
  // auto run
  writeI2C(SI1145, SI1145_REG_COMMAND, SI1145_PSALS_AUTO);

  return true;
}

void SCKDriver::reset() {
  writeI2C(SI1145, SI1145_REG_MEASRATE0, 0);
  writeI2C(SI1145, SI1145_REG_MEASRATE1, 0);
  writeI2C(SI1145, SI1145_REG_IRQEN, 0);
  writeI2C(SI1145, SI1145_REG_IRQMODE1, 0);
  writeI2C(SI1145, SI1145_REG_IRQMODE2, 0);
  writeI2C(SI1145, SI1145_REG_INTCFG, 0);
  writeI2C(SI1145, SI1145_REG_IRQSTAT, 0xFF);

  writeI2C(SI1145, SI1145_REG_COMMAND, SI1145_RESET);
  delay(10);
  writeI2C(SI1145, SI1145_REG_HWKEY, 0x17);
  
  delay(10);
}


//////////////////////////////////////////////////////

// returns the UV index * 100 (divide by 100 to get the index)
uint16_t SCKDriver::readUV(void) {
 return read16(0x2C); 
}

// returns visible+IR light levels
uint16_t SCKDriver::readVisible(void) {
 return read16(0x22); 
}

// returns IR light levels
uint16_t SCKDriver::readIR(void) {
 return read16(0x24); 
}

// returns "Proximity" - assumes an IR LED is attached to LED
uint16_t SCKDriver::readProx(void) {
 return read16(0x26); 
}

/*********************************************************************/

uint8_t SCKDriver::writeParam(uint8_t p, uint8_t v) {
  //Serial.print("Param 0x"); Serial.print(p, HEX);
  //Serial.print(" = 0x"); Serial.println(v, HEX);
  
  writeI2C(SI1145, SI1145_REG_PARAMWR, v);
  writeI2C(SI1145, SI1145_REG_COMMAND, p | SI1145_PARAM_SET);
  return readI2C(SI1145, SI1145_REG_PARAMRD);
}

uint8_t SCKDriver::readParam(uint8_t p) {
  writeI2C(SI1145, SI1145_REG_COMMAND, p | SI1145_PARAM_QUERY);
  return readI2C(SI1145, SI1145_REG_PARAMRD);
}

/*********************************************************************/
void SCKDriver::writeI2C(byte deviceaddress, byte address, byte data ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
  delay(4);
}

byte SCKDriver::readI2C(int deviceaddress, byte address ) {
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

uint16_t SCKDriver::read16(uint8_t a) {
  uint16_t ret;

  Wire.beginTransmission(SI1145); // start transmission to device 
  Wire.write(a); // sends register address to read from
  Wire.endTransmission(); // end transmission
  
  Wire.requestFrom(SI1145, (uint8_t)2);// send data n-bytes read
  ret = Wire.read(); // receive DATA
  ret |= (uint16_t)Wire.read() << 8; // receive DATA

  return ret;
}






boolean SCKDriver::compareDate(char* text, char* text1)
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

uint16_t SCKDriver::getBattery() {
  uint16_t temp = 2*(readADC(3))*VCC/4095.;
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
  SerialUSB.print(temp);
  SerialUSB.println(" %");
#endif
  return temp; 
}

uint16_t SCKDriver::getCharger() {
  uint16_t temp = 2*(readADC(2))*VCC/4095.;
#if debuggSCK
  SerialUSB.print("Charger voltage: ");
  SerialUSB.print(temp);
  SerialUSB.print(" mV, ");
#endif
  return temp; 
}


void SCKDriver::accelDefault(void)
{
    // Accelerometer
    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    writeI2C(ACCMAG, 0x21, 0x00);
    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    writeI2C(ACCMAG, 0x20, 0x57);
    
    // Magnetometer
    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    writeI2C(ACCMAG, 0x24, 0x64);
    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    writeI2C(ACCMAG, 0x25, 0x20);
    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    writeI2C(ACCMAG, 0x26, 0x00);
}



void SCKDriver::readAccMag(byte reg, float* __x, float* __y, float* __z)
  {
    byte accmag[6];
    Wire.beginTransmission(ACCMAG);
    // assert the MSB of the address to get the accelerometer
    // to do slave-transmit subaddress updating.
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(ACCMAG, 6);
    for (int i=0; i<6; i++) accmag[i] = Wire.read();
    // combine high and low bytes
    // This no longer drops the lowest 4 bits of the readings from the DLH/DLM/DLHC, which are always 0
    // (12-bit resolution, left-aligned). The D has 16-bit resolution
    *__x = ((int16_t)(accmag[1] << 8 | accmag[0]));
    *__y = ((int16_t)(accmag[3] << 8 | accmag[2]));
    *__z = ((int16_t)(accmag[5] << 8 | accmag[4]));
  }

// Reads the 3 accelerometer channels and stores them in vector a
void SCKDriver::readAcc(float* __x, float* __y, float* __z)
{
  readAccMag(OUT_X_L_A | (1 << 7), __x, __y,  __z);
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
void SCKDriver::readMag(float* __x, float* __y, float* __z)
{
  readAccMag(OUT_X_L_M | (1 << 7), __x, __y,  __z);
  #if debuggSCK
    SerialUSB.print("Magnetometer x: ");
    SerialUSB.print(*__x);
    SerialUSB.print(" y: ");
    SerialUSB.print(*__y);
    SerialUSB.print(" z: ");
    SerialUSB.println(*__z);
  #endif
}







