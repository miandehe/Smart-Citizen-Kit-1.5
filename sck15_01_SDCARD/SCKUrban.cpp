#include "Constants.h"
#include "SCKUrban.h"
#include "SCKBase.h"
#include "SCKDrivers.h"

SCKDriver driver_;
SCKBase base;

void SCKUrban::begin()
  {
    base.begin();

    driver_.writeVH(CO_SENSOR, 2700); //VH_CO SENSOR Inicial
    digitalWrite(IO0, HIGH); //VH_CO SENSOR
    driver_.writeVH(NO2_SENSOR, 1700); //VH_CO SENSOR Inicial
    digitalWrite(IO1, HIGH); //VH_NO2 SENSOR 
    driver_.writeResistor(CO_SENSOR + 2, 100000); //Inicializacion de la carga del CO SENSOR
    driver_.writeResistor(NO2_SENSOR + 2, 100000); //Inicializacion de la carga del NO2 SENSOR
    driver_.beginUV();
  }

void SCKUrban::sleep()
  {
    digitalWrite(IO0, LOW); //VH_CO SENSOR
    digitalWrite(IO1, LOW); //VH_NO2 SENSOR 
    driver_.writeResistor(CO_SENSOR + 2, 100000); //Inicializacion de la carga del CO SENSOR
    driver_.writeResistor(NO2_SENSOR + 2, 100000); //Inicializacion de la carga del NO2 SENSOR
    base.sleep();
  }

void SCKUrban::awake()
  {
    base.awake();
    driver_.writeVH(CO_SENSOR, 2700); //VH_CO SENSOR Inicial
    digitalWrite(IO0, HIGH); //VH_CO SENSOR
    driver_.writeVH(NO2_SENSOR, 1700); //VH_CO SENSOR Inicial
    digitalWrite(IO1, HIGH); //VH_NO2 SENSOR 
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
