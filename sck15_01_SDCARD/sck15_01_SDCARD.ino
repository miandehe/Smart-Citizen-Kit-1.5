#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Constants.h"
#include "SCKDrivers.h"

SCKDriver driver;

#define USBEnabled      true 
#define sensorEnabled   false
#define debuggEnabled   false

uint32_t timetransmit = 0;  
uint32_t TimeUpdate   = 0;  //Variable temporal de tiempo entre actualizacion y actualizacion de los sensensores
uint32_t NumUpdates   = 0;  //Numero de actualizaciones antes de postear

File myFile;

uint32_t SENSORvalue[15];

void setup() {
  delay(5000);
  sckBegin();
  sckAccelDefault();
  sckConfig();
  driver.ESPoff();
  #if debuggEnabled
    SerialUSB.print(F("Initializing SD card..."));
  #endif 
    if (!SD.begin(11)) {
  #if debuggEnabled
      SerialUSB.println(F("initialization failed!"));
  #endif 
      return;
    }
  #if debuggEnabled
    SerialUSB.println(F("initialization done."));   
  #endif 
    if (!SD.exists("post.csv")) {
      #if debuggEnabled
        SerialUSB.println(F("Creating post.csv..."));
      #endif 
      SD.open("post.csv", FILE_WRITE);
      myFile.close();
      delay(1000);
      txHeader();
      
    } else{
      #if debuggEnabled
        SerialUSB.println(F("post.csv exists ..."));
      #endif 
    }
    timetransmit = millis();
    TimeUpdate = 10;
    //TimeUpdate = atol(driver.readData(EE_ADDR_TIME_UPDATE, 0, INTERNAL)); //Tiempo entre transmision y transmision en segundos
    
}

void loop() {  
#if sensorEnabled  
//  if ((millis()-timetransmit) >= (unsigned long)TimeUpdate*1000)
//  {  
    SerialUSB.println("*** loop ***");
    timetransmit = millis();
    updateSensorsSD();
    txSD();
    #if USBEnabled
        txDebugSD();
    #endif
    delay(2000);
//  }
#endif
    driver.writeADC(0, B11000110);
    //sckReadAcc(&SENSORvalue[8], &SENSORvalue[9], &SENSORvalue[10]);
    //sckReadMag(&SENSORvalue[11], &SENSORvalue[12], &SENSORvalue[13]);
    SerialUSB.print(driver.readADC(6));
    SerialUSB.print(" ");
    SerialUSB.print(driver.readADC(7));
    SerialUSB.print(" ");
    SerialUSB.println((uint16_t)(driver.readADC(6)<<4)|driver.readADC(7)>>4);
    SerialUSB.print(" ");
    SerialUSB.println(((uint16_t)(driver.readADC(6)<<4)|driver.readADC(7)>>4)*3300/4095.);
    delay(100);
    
}
