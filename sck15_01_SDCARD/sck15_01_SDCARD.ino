#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include "Constants.h"
#include "SCKDrivers.h"

SCKDriver driver;

#define USBEnabled      true 
#define sensorEnabled   true
#define debuggEnabled   true

uint32_t timetransmit = 0;  
uint32_t TimeUpdate   = 0;  //Variable temporal de tiempo entre actualizacion y actualizacion de los sensensores
uint32_t NumUpdates   = 0;  //Numero de actualizaciones antes de postear

File myFile;

uint32_t SENSORvalue[8];
boolean csvInit = false;


//void setup()
//  {
//    sckBegin();
//    driver.ESPoff();
//    driver.chargerMode(true); //USB source
//  }
//  
//void loop()
//  {
//    //Serial.write(map((long int)sckGetNoise(), 0, 3300, 0, 255));
//    Serial.println((unsigned long int)(sckGetNoise()), DEC);
////      driver.writeResistor(4, 2500);
////      delay(20);
////      driver.writeResistor(5, 10000);
////      delay(1000);
////      Serial.print(driver.readResistor(4));
////      Serial.print(", ");
////      Serial.println(driver.readResistor(5));
//  }

void setup() {
  delay(5000);
  sckBegin();
  sckConfig();
  driver.ESPoff();
  #if debuggEnabled
    Serial.print(F("Initializing SD card..."));
  #endif 
    if (!SD.begin(11)) {
  #if debuggEnabled
      Serial.println(F("initialization failed!"));
  #endif 
      return;
    }
  #if debuggEnabled
    Serial.println(F("initialization done."));   
  #endif 
    if (!SD.exists("post.csv")) {
      #if debuggEnabled
        Serial.println(F("Creating post.csv..."));
      #endif 
      SD.open("post.csv", FILE_WRITE);
      myFile.close();
      delay(1000);
      txHeader();
      
    } else{
      #if debuggEnabled
        Serial.println(F("post.csv exists ..."));
      #endif 
    }
    
    timetransmit = millis();
    TimeUpdate = atol(driver.readData(EE_ADDR_TIME_UPDATE, 0, INTERNAL)); //Tiempo entre transmision y transmision en segundos
}

void loop() {  
#if sensorEnabled  
  if ((millis()-timetransmit) >= (unsigned long)TimeUpdate*1000)
  {  
    Serial.println("*** loop ***");
    timetransmit = millis();
    TimeUpdate = atol(driver.readData(EE_ADDR_TIME_UPDATE, 0, INTERNAL)); 
    updateSensorsSD();
    txSD();
    #if USBEnabled
        txDebugSD();
    #endif
  }
#endif
}
















