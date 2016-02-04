void sckBegin() {
  driver.begin();
  
  driver.chargerMode(HIGH, LOW);
  
  sckWriteVH(CO_SENSOR, 2700); //VH_CO SENSOR Inicial
  digitalWrite(IO0, HIGH); //VH_CO SENSOR

  sckWriteVH(NO2_SENSOR, 1700); //VH_CO SENSOR Inicial
  digitalWrite(IO1, HIGH); //VH_NO2 SENSOR
    
  driver.writeResistor(CO_SENSOR + 2, 100000); //Inicializacion de la carga del CO SENSOR
  driver.writeResistor(NO2_SENSOR + 2, 100000); //Inicializacion de la carga del NO2 SENSOR
}

void sckConfig(){
  
  if (!sckCompareDate(__TIME__, driver.readData(EE_ADDR_TIME_VERSION, 0, INTERNAL)))
  {
    driver.RTCadjust(driver.sckDate(__DATE__,__TIME__));
#if debuggEnabled
    Serial.println(F("Resetting..."));
#endif
    for(uint16_t i=0; i<60; i++) driver.writeEEPROM(i, 0x00);  //Borrado de la memoria
    driver.writeData(EE_ADDR_TIME_VERSION, 0, __TIME__, INTERNAL);
    driver.writeData(EE_ADDR_TIME_UPDATE, 0, DEFAULT_TIME_UPDATE, INTERNAL);
    driver.writeData(EE_ADDR_NUMBER_UPDATES, 0, DEFAULT_MIN_UPDATES, INTERNAL);
  }

}

boolean sckCompareDate(char* text, char* text1)
{
  if ((strlen(text))!=(strlen(text1))) return false;
  else 
  {
    for(int i=0; i<strlen(text); i++)
    {
      if (text[i]!=text1[i]) return false;
    }
  }
  return true;
}

uint16_t sckGetBattery() {
  uint16_t temp = driver.levelRead(1)*10;
  #if !DataRaw 
    temp = map(temp, VAL_MIN_BATTERY, VAL_MAX_BATTERY, 0, 1000);
    if (temp>1000) temp=1000;
    if (temp<0) temp=0;
  #endif
#if debuggSCK
  Serial.print("Vbat: ");
  Serial.print(voltage);
  Serial.print(" mV, ");
  Serial.print("Battery level: ");
  Serial.print(temp/10.);
  Serial.println(" %");
#endif
  return temp; 
}

uint16_t sckGetCharger() {
  uint16_t temp = driver.levelRead(0);
#if debuggSCK
  Serial.print("Charger voltage: ");
  Serial.print(voltage);
  Serial.print(" mV, ");
#endif
  return temp; 
}








