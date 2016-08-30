void sckBegin() {
  driver.begin();
  driver.writeCurrent(200);
  sckWriteVH(CO_SENSOR, 2700); //VH_CO SENSOR Inicial
  digitalWrite(IO0, HIGH); //VH_CO SENSOR
  sckWriteVH(NO2_SENSOR, 1700); //VH_CO SENSOR Inicial
  digitalWrite(IO1, HIGH); //VH_NO2 SENSOR 
  driver.writeResistor(CO_SENSOR + 2, 100000); //Inicializacion de la carga del CO SENSOR
  driver.writeResistor(NO2_SENSOR + 2, 100000); //Inicializacion de la carga del NO2 SENSOR
  driver.iniADC();
}






