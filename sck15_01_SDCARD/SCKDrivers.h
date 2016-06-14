/*

  SCKDrivers.h
  Supports core and data management functions (Power, WiFi, SD storage, RTClock and EEPROM storage)

  - Modules supported:

    - WIFI
    - RTC
    - EEPROM
    - POWER MANAGEMENT IC's

*/

#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <Arduino.h>

class SCKDriver {
public:  
    void begin();
    
    /*RTC commands*/
//    char* sckDate(const char* date, const char* time);
//    boolean RTCadjust(char *time);
//    boolean RTCtime(char *time);
//    char* RTCtime();
    
    /*Inputs an outputs control*/
    float average(int anaPin);
    int levelRead(boolean state);
    
    /*EEPROM commands*/
//    void writeEEPROM(uint16_t eeaddress, uint8_t data);                              //Write a byte into the EEPROM
//    byte readEEPROM(uint16_t eeaddress);                                             //Read a byte from the EEPROM
//    void writeData(uint32_t eeaddress, long data, uint8_t location);                 //Write a long into the EEPROM
//    void writeData(uint32_t eeaddress, uint16_t pos, char* text, uint8_t location);  //Write a string into the EEPROM
//    char* readData(uint16_t eeaddress, uint16_t pos, uint8_t location);              //Read a string from the EEPROM
//    uint32_t readData(uint16_t eeaddress, uint8_t location);                         //Read a long from the EEPROM
    
    /*ESP8266 commands*/
    void ESPini();
    void ESPflash();
    void ESPoff();
    
    /*Charger commands*/
    void chargerMode(boolean state1, boolean state2);
    
    /*Sensor temperature*/
    uint16_t readSHT(uint8_t type);
    void getSHT(uint32_t* __Temperature, uint32_t* __Humidity);
    
    /*Potenciometer*/ 
    void writeI2C(byte deviceaddress, byte address, byte data );
    void writeResistor(byte resistor, float value );
    byte readI2C(int deviceaddress, byte address );
    float readResistor(byte resistor);
   
    byte readADC(byte channel);
    void writeADC(byte address, byte value);
    
private:

};
#endif
