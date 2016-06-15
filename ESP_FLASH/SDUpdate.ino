void txSD() {
  SerialUSB.println("*** txSD ***");
  // if the file opened okay, write to it:
  myFile = SD.open("post.csv", FILE_WRITE);
  if (myFile) {
  #if debuggEnabled
      SerialUSB.println(F("Writing...")); 
  #endif 
    float dec = 0;
    for (int i=0; i<15; i++)
    {
      if (i<2) dec = 100;
      else if (i<4) dec = 10;
      else if (i<7) dec = 1000;
      else if (i<8) dec = 100;
      else dec = 1;
      myFile.print(SENSORvalue[i]/dec);
      myFile.print(",");
    }
    //myFile.print(driver.RTCtime());
    myFile.print("00:00:00");
    myFile.println();
    // close the file:
    myFile.close();
#if debuggEnabled
    SerialUSB.println(F("Closing...")); 
#endif 
  }
}

char* SENSOR[20]={
  "Temperature",
  "Humidity",
  "Light",
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
    sckGetMICS(&SENSORvalue[6], &SENSORvalue[7]);
    SENSORvalue[2] = sckGetLight(); // %
    SENSORvalue[3] = sckGetBattery(); //%
    SENSORvalue[4] = sckGetCharger();  // mV
    SENSORvalue[5] = driver.readCurrentCharge(); //mA
    SENSORvalue[8] = sckGetNoise(); //dB  
    sckReadAcc(&SENSORvalue[9], &SENSORvalue[10], &SENSORvalue[11]);
    sckReadMag(&SENSORvalue[12], &SENSORvalue[13], &SENSORvalue[14]);
}

void txDebugSD() {
  SerialUSB.println("*** txDebugSD ***");
  float dec = 0;
  for(int i=0; i<15; i++) 
  {
    SerialUSB.print(SENSOR[i]);
    SerialUSB.print(": "); 
    SerialUSB.print((SENSORvalue[i])); 
    SerialUSB.println(UNITS[i]);
  }
  SerialUSB.print(SENSOR[15]);
  SerialUSB.print(": "); 
  //SerialUSB.println(driver.RTCtime());
  SerialUSB.println("00:00:00");
  SerialUSB.println(F("*******************"));     
}

void txHeader() { 
  SerialUSB.println("*** txHeader ***");
  // if the file opened okay, write to it:
  if (SD.open("post.csv", FILE_WRITE)) {
  #if debuggEnabled
      SerialUSB.println(F("Writing...")); 
  #endif 
    for (int i=0; i<15; i++)
    {
      myFile.print(SENSOR[i]);
      myFile.print(" (");
      myFile.print(UNITS[i]);
      myFile.print(") ");
      myFile.print(", ");
    }
    myFile.print(SENSOR[15]);
    myFile.println();
    // close the file:
    myFile.close();
    #if debuggEnabled
        SerialUSB.println(F("Closing...")); 
    #endif 
  }
}


