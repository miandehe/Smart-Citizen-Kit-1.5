void txSD() {
  SerialUSB.println("*** txSD ***");
  // if the file opened okay, write to it:
  myFile = SD.open("post.csv", FILE_WRITE);
  if (myFile) {
  driver.RGBled(255,0,0);
  #if debuggEnabled
      SerialUSB.println(F("Writing...")); 
  #endif 
    float dec = 0;
    for (int i=0; i<16; i++)
    {
      myFile.print(SENSORvalue[i]);
      myFile.print(",");
    }
    myFile.print(driver.RTCtime());
//    myFile.print("00:00:00");
    myFile.println();
    // close the file:
    myFile.close();
  #if debuggEnabled
      SerialUSB.println(F("Closing...")); 
  #endif 
  driver.RGBled(0,255,0);
  }
}

char* SENSOR[20]={
  "Temperature",
  "Humidity",
  "Light",
  "UVindex",
  "Battery",
  "Charger voltage",
  "Charger current",
  "Carbon Monxide",
  "Nitrogen Dioxide",
  "Noise",
  "Accel X",
  "Accel Y",
  "Accel Z",
  "Mag X",
  "Mag Y",
  "Mag Z",
  "UTC"
};

char* UNITS[20]={
  " C",
  " %",
  " lx",
  " ",
#if DataRaw
  " mV",
#else
  " %",
#endif
  " V",
  " mA",
  " Ohm",
  " Ohm",
#if DataRaw
  " mV",
#else
  " dB",
#endif
  "",
  "",
  "",
  "", 
  "",
  "",
  "" 
};            

void updateSensorsSD() {
    driver.getSHT(&SENSORvalue[0], &SENSORvalue[1]);
    SENSORvalue[2] = driver.getLight(); // %
    SENSORvalue[3] = driver.readUV()/100.;
    SENSORvalue[4] = driver.getBattery(); //%
    SENSORvalue[5] = driver.getCharger();  // mV
    SENSORvalue[6] = driver.readCurrentCharge(); //mA
    driver.getMICS(&SENSORvalue[7], &SENSORvalue[8]);
    SENSORvalue[9] = driver.getNOISE(); //dB  
    driver.readAcc(&SENSORvalue[10], &SENSORvalue[11], &SENSORvalue[12]);
    driver.readMag(&SENSORvalue[13], &SENSORvalue[14], &SENSORvalue[15]);
}

void txDebugSD() {
  SerialUSB.println("*** txDebugSD ***");
  float dec = 0;
  for(int i=0; i<16; i++) 
  {
    SerialUSB.print(SENSOR[i]);
    SerialUSB.print(": "); 
    SerialUSB.print((SENSORvalue[i])); 
    SerialUSB.println(UNITS[i]);
  }
  SerialUSB.print(SENSOR[16]);
  SerialUSB.print(": "); 
  SerialUSB.println(driver.RTCtime());
  SerialUSB.println(F("*******************"));     
}

void txHeader() { 
  SerialUSB.println("*** txHeader ***");
  // if the file opened okay, write to it:
  if (SD.open("post.csv", FILE_WRITE)) {
  #if debuggEnabled
      SerialUSB.println(F("Writing...")); 
  #endif 
    for (int i=0; i<16; i++)
    {
      myFile.print(SENSOR[i]);
      myFile.print(" (");
      myFile.print(UNITS[i]);
      myFile.print(") ");
      myFile.print(", ");
    }
    myFile.print(SENSOR[16]);
    myFile.println();
    // close the file:
    myFile.close();
    #if debuggEnabled
        SerialUSB.println(F("Closing...")); 
    #endif 
  }
}


