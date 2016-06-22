/*************************************************** 
  This is a library for the Si1145 UV/IR/Visible Light Sensor

  Designed specifically to work with the Si1145 sensor in the
  adafruit shop
  ----> https://www.adafruit.com/products/1777

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_SI1145.h"

Adafruit_SI1145 uv = Adafruit_SI1145();

void setup() {
  SerialUSB.begin(9600);
  
  SerialUSB.println("Adafruit SI1145 test");
  delay(4000);
  if (! uv.begin()) {
    SerialUSB.println("Didn't find Si1145");
    while (1);
  }

  SerialUSB.println("OK!");
}

void loop() {
  SerialUSB.println("===================");
  SerialUSB.print("Vis: "); SerialUSB.println(uv.readVisible());
  SerialUSB.print("IR: "); SerialUSB.println(uv.readIR());
  
  // Uncomment if you have an IR LED attached to LED pin!
  //SerialUSB.print("Prox: "); SerialUSB.println(uv.readProx());

  float UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;  
  SerialUSB.print("UV: ");  SerialUSB.println(UVindex);

  delay(1000);
}
