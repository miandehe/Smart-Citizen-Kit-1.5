#include <Wire.h>
#include "Constants.h"
#include "SCKDrivers.h"

SCKDriver driver;

void setup() {
  sckBegin();
  digitalWrite(BLUEpin, LOW);
  driver.ESPoff();
  driver.ESPflash();
}

void loop() {  
  if (Serial1.available())
      SerialUSB.write(Serial1.read());
  if (SerialUSB.available())
    Serial1.write(SerialUSB.read());
}
