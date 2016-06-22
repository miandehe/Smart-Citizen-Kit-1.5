#include "Constants.h"
#include "SCKBase.h"
#include "SCKDrivers.h"

SCKDriver driver__;

void SCKBase::begin() {
  driver__.begin();
  driver__.writeCurrent(200);
  driver__.ADCini();
  driver__.RTCini();
  driver__.RTCadjust(driver__.sckDate(__DATE__,__TIME__));
  driver__.accelDefault();


  
}

void SCKBase::config(){
  
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

