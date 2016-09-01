#include "Constants.h"
#include "SCKUrban.h"
#include "SCKBase.h"
#include "SCKDrivers.h"

SCKDriver driver_;
SCKBase base;

void SCKUrban::begin()
  {
    base.begin();
    driver_.currentHeat(CO_SENSOR, 32); //Corriente en mA
    driver_.MICSini();
    driver_.beginUV();
  }

void SCKUrban::sleep()
  {
    
    driver_.MICSoff();
    base.sleep();
    USBDevice.detach();
    base.standby();
  }

void SCKUrban::awake()
  {
    base.awake();
    USBDevice.attach();
    driver_.MICSini();
    driver_.beginUV();
  }
  
boolean state = true;

void SCKUrban::management()
  {
    if (base.button()) 
      {
        if (state)
          {
            sleep();
            state = false;
            __WFI();
          }
      }
    else 
      {
        if (!state)
          {
            awake();
            state = true;
          }
      }
  }
