boolean connected;                  

#define buffer_length        32
static char buffer[buffer_length];

void sckBegin() {
  driver.begin();
  driver.chargerMode(true);
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

float sckReadCharge() {
//  float resistor = kr*sckReadMCP(MCP3, 0x00)/1000;    
//  float current = 1000./(2+((resistor * 10)/(resistor + 10)));
//#if debuggSCK
//  Serial.print("Resistor : ");
//  Serial.print(resistor);
//  Serial.print(" kOhm, ");  
//  Serial.print("Current : ");
//  Serial.print(current);
//  Serial.println(" mA");  
//#endif
//  return(current);
}

void sckWriteCharge(int current) {
//  if (current < 100) current = 100;
//  else if (current > 500) current = 500;
//  float Rp = (1000./current)-2;
//  float resistor = Rp*10/(10-Rp);
//  sckWriteMCP(MCP3, 0x00, (uint8_t)(resistor*1000/kr));    
//#if debuggSCK
//  Serial.print("Rc : ");
//  Serial.print(Rp + 2);
//  Serial.print(" kOhm, ");
//  Serial.print("Rpot : ");
//  Serial.print(resistor);
//  Serial.print(" kOhm, ");  
//  Serial.print("Current : ");
//  Serial.print(current);
//  Serial.println(" mA");  
//#endif
}  

boolean sckCheckRTC() {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00); //Address
  Wire.endTransmission();
  delay(4);
  Wire.requestFrom(RTC_ADDRESS,1);
  unsigned long time = millis();
  while (!Wire.available()) if ((millis() - time)>500) return false;
  Wire.read();
  return true;
}

uint16_t sckGetBattery() {
  uint16_t temp = driver.levelRead(1);
  temp = map(temp, VAL_MIN_BATTERY, VAL_MAX_BATTERY, 0, 1000);
  if (temp>1000) temp=1000;
  if (temp<0) temp=0;
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

char* itoa(int32_t number)
{
  byte count = 0;
  uint32_t temp;
  if (number < 0) {
    temp = number*(-1); 
    count++;
  } 
  while ((temp/10)!=0) 
  {
    temp = temp/10;
    count++;
  }
  int i;
  if (number < 0) {
    temp = number*(-1);
  } 
  else temp = number;
  for (i = count; i>=0; i--) 
  { 
    buffer[i] = temp%10 + '0'; 
    temp = temp/10; 
  }
  if (number < 0) {
    buffer[0] = '-';
  } 
  buffer[count + 1] = 0x00;
  return buffer;   
}









