void txSD() {
  Serial.println("*** txSD ***");
  // if the file opened okay, write to it:
  myFile = SD.open("post.csv", FILE_WRITE);
  if (myFile) {
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

char* SENSOR[12]={
  "Temperature",
  "Humidity",
  "Light",
  "Battery",
  "Charger voltage",
  "Carbon Monxide",
  "Nitrogen Dioxide",
  "Noise",
  "Accel X",
  "Accel Y",
  "Accel Z",
  "UTC"
};

char* UNITS[12]={
  " C",
  " %",
  " lx",
#if DataRaw
  " mV",
#else
  " %",
#endif
  " V",
  " kOhm",
  " kOhm",
#if DataRaw
  " mV",
#else
  " dB",
#endif
  "",
  "",
  "",
  "" 
};            

void updateSensorsSD() {
    compass.read();
    sckGetSHT(&SENSORvalue[0], &SENSORvalue[1]);
    sckGetMICS(&SENSORvalue[5], &SENSORvalue[6]);
    SENSORvalue[2] = sckGetLight(); // %
    SENSORvalue[3] = sckGetBattery(); //%
    SENSORvalue[4] = sckGetCharger();  // mV
    SENSORvalue[7] = sckGetNoise(); //dB  
    SENSORvalue[8] = compass.a.x; //dB  
    SENSORvalue[9] = compass.a.y; //dB  
    SENSORvalue[10] = compass.a.z; //dB  
    
}

void txDebugSD() {
  Serial.println("*** txDebugSD ***");
  float dec = 0;
  for(int i=0; i<11; i++) 
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
  Serial.print(SENSOR[11]);
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
    for (int i=0; i<11; i++)
    {
      myFile.print(SENSOR[i]);
      myFile.print(" (");
      myFile.print(UNITS[i]);
      myFile.print(") ");
      myFile.print(", ");
    }
    myFile.print(SENSOR[11]);
    myFile.println();
    // close the file:
    myFile.close();
    #if debuggEnabled
        Serial.println(F("Closing...")); 
    #endif 
  }
}


