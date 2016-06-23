/*

  SCKDrivers.h
  Supports core and data management functions (Power, WiFi, SD storage, RTClock and EEPROM storage)

  - Modules supported:

    - WIFI
    - RTC
    - EEPROM
    - POWER MANAGEMENT IC's

*/

#include <Arduino.h>
#include <RTCZero.h>

class SCKDriver {
public:  
    void begin();
    
    /*RTC commands*/
    void RTCini();
    char* sckDate(const char* date, const char* time);
    boolean RTCadjust(char *time);
    boolean RTCtime(char *time);
    char* RTCtime();
    
    /*Inputs an outputs control*/
    float average(int anaPin);
    void RGBled(uint8_t REDcolor, uint8_t GREENcolor, uint8_t BLUEcolor);
       
    /*ESP8266 commands*/
    void ESPini();
    void ESPflash();
    void ESPoff();
    
    /*Charger commands*/
    void writeCurrent(int current);
    float readCurrent();
    boolean StatusCharge();
    float readCurrentCharge();
    
    /*Sensor temperature*/
    uint16_t readSHT(uint8_t type);
    void getSHT(float* __Temperature, float* __Humidity);
    
    /*Potenciometer*/ 
    void writeResistor(byte resistor, float value );
    float readResistor(byte resistor);

    /*Analog to digital converter*/
    uint16_t readADC(byte channel);
    void ADCini();
    void ADCoff();
    
    /*Gas sensor*/ 
    void MICSini();
    void MICSoff();
    void writeVH(byte device, long voltage );
    float readVH(byte device);
    void currentHeat(byte device, int current);
    float readRs(byte device);
    float readMICS(byte device);
    void getMICS(float* __RsCO, float* __RsNO2);

    /*Light sensor*/ 
    uint16_t getLight();

    //Adafruit_SI1145(void);
    boolean beginUV();
    void reset();
  
    uint16_t readUV();
    uint16_t readIR();
    uint16_t readVisible();
    uint16_t readProx();
  
    /*Audio sensor*/ 
    void NOISEini();
    void writeGAIN(float GAIN1, float GAIN2);
    float readGAIN();  
    float getNOISE();


   
    uint16_t getBattery();
    uint16_t getCharger();
    
    //Accelerometer and magnetometer sensors
    void accelDefault(void);
    void readAccMag(byte reg, float* __x, float* __y, float* __z);
    void readAcc(float* __x, float* __y, float* __z);
    void readMag(float* __x, float* __y, float* __z);


private:
  byte readI2C(int deviceaddress, byte address );
  void writeI2C(byte deviceaddress, byte address, byte data );
  uint16_t read16(uint8_t addr);
  
  uint8_t readParam(uint8_t p);
  uint8_t writeParam(uint8_t p, uint8_t v);
};

