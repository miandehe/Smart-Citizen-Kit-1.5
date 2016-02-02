void txSD() {
  Serial.println("*** txSD ***");
  // if the file opened okay, write to it:
  if (SD.open("post.csv", FILE_WRITE)) {
  #if debuggEnabled
      Serial.println(F("Writing...")); 
  #endif 
    float dec = 0;
    for (int i=0; i<8; i++)
    {
      if (i<2) dec = 100;
      else if (i<4) dec = 10;
      else if (i<7) dec = 1000;
      else if (i<8) dec = 100;
      else dec = 1;

      //myFile.print(i);
      //myFile.print(" ");
      myFile.print(SENSORvalue[i]/dec);
      myFile.print(",");
    }
    myFile.print(driver.RTCtime());
    myFile.println();
    // close the file:
    myFile.close();
#if debuggEnabled
    Serial.println(F("Closing...")); 
#endif 
  }
}

char* SENSOR[10]={
  "Temperature",
  "Humidity",
  "Light",
  "Battery",
  "Charger voltage",
  "Carbon Monxide",
  "Nitrogen Dioxide",
  "Noise",
  "Wifi Spots",
  "UTC"
};

char* UNITS[10]={
  " C",
  " %",
  " lx",
  " %",
  " V",
  " kOhm",
  " kOhm",
#if DataRaw
  " mV",
#else
  " dB",
#endif
  "",
  "" 
};            

void updateSensorsSD() {
    sckGetSHT(&SENSORvalue[0], &SENSORvalue[1]);
    sckGetMICS(&SENSORvalue[5], &SENSORvalue[6]);
    SENSORvalue[2] = sckGetLight(); // %
    SENSORvalue[3] = sckGetBattery(); //%
    SENSORvalue[4] = sckGetCharger();  // mV
    SENSORvalue[7] = sckGetNoise(); //dB    
}

void txDebugSD() {
  Serial.println("*** txDebugSD ***");
  float dec = 0;
  for(int i=0; i<8; i++) 
  {
    if (i<2) dec = 100;
    else if (i<4) dec = 10;
    else if (i<7) dec = 1000;
    else if (i<8) dec = 100;
    else dec = 1;
    Serial.print(SENSOR[i]);
    Serial.print(": "); 
    Serial.print((SENSORvalue[i])/dec); 
    Serial.println(UNITS[i]);
  }
  Serial.print(SENSOR[9]);
  Serial.print(": "); 
  Serial.println(driver.RTCtime());
  Serial.println(F("*******************"));     
}

void txHeader() { 
  Serial.println("*** txHeader ***");
  // if the file opened okay, write to it:
  if (SD.open("post.csv", FILE_WRITE)) {
  #if debuggEnabled
      Serial.println(F("Writing...")); 
  #endif 
    for (int i=0; i<8; i++)
    {
      myFile.print(SENSOR[i]);
      myFile.print(" (");
      myFile.print(UNITS[i]);
      myFile.print(") ");
      myFile.print(", ");
    }
    myFile.print(SENSOR[9]);
    myFile.println();
    // close the file:
    myFile.close();
    #if debuggEnabled
        Serial.println(F("Closing...")); 
    #endif 
  }
}


