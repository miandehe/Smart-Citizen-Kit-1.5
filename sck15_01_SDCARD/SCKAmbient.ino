
//Valores por defecto de la resistencia en vacio de los MICS
float RoCO  = 750000;
float RoNO2 = 2200;

  void sckWriteGAIN(float GAIN1, float GAIN2)
  {
    driver.writeResistor(6, GAIN1);
    delay(20);
    driver.writeResistor(7, GAIN2);
  }

  float sckReadGAIN()
  {
    return ((22000/(driver.readResistor(6)+2440)+1)*(62000/driver.readResistor(7)+1));
  }    
 


  void sckWriteVH(byte device, long voltage ) {
    float resistor = ((voltage/800.)-1)*R2;
    driver.writeResistor(device, resistor);
  }

  
  float sckReadVH(byte device) {
    float resistor = driver.readResistor(device);
    float voltage = ((resistor/R2) + 1)*800;
    return(voltage);
  }
  
  void sckHeat(byte device, int current)
  {
    float Rc=Rc0;
    byte Sensor = S2;
    if (device == NO2_SENSOR) { Rc=Rc1; Sensor = S3;}

    float Vc = (float)driver.average(Sensor)*VCC/1023; //mV 
    float current_measure = Vc/Rc; //mA 
    float Rh = (sckReadVH(device)- Vc)/current_measure;
    float Vh = (Rh + Rc)*current;

    sckWriteVH(device, Vh);
      #if debuggSCK
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR current: ");
        else SerialUSB.print("CO SENSOR corriente: ");
        SerialUSB.print(current_measure);
        SerialUSB.println(" mA");
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR correction VH: ");
        else  SerialUSB.print("CO SENSOR correccion VH: ");
        SerialUSB.print(sckReadVH(device));
        SerialUSB.println(" mV");
        Vc = (float)driver.average(Sensor)*VCC/1023; //mV 
        current_measure = Vc/Rc; //mA 
        if (device == NO2_SENSOR) SerialUSB.print("NO2 SENSOR corrected current: ");
        else SerialUSB.print("CO SENSOR corrected current: ");
        SerialUSB.print(current_measure);
        SerialUSB.println(" mA");
        SerialUSB.println("Heating...");
      #endif
    
  }

   float sckReadRs(byte device)
   {
     byte Sensor = S0;
     if (device == NO2_SENSOR) {Sensor = S1; }
     float RL = driver.readResistor(device + 2); //Ohm
     float VL = ((float)driver.average(Sensor)*VCC)/1023; //mV
     if (VL > VCC) VL = VCC;
     float Rs = ((VCC-VL)/VL)*RL; //Ohm
     #if debuggSCK
        if (device == CO_SENSOR) SerialUSB.print("CO SENSOR VL: ");
        else SerialUSB.print("NO2 SENSOR VL: ");
        SerialUSB.print(VL);
        SerialUSB.print(" mV, RS: ");
        SerialUSB.print(Rs);
        SerialUSB.print(" Ohm, RL: ");
        SerialUSB.print(RL);
        SerialUSB.println(" Ohm");
     #endif;  
     return Rs;
   }
   
  float sckReadMICS(byte device)
  {
      float Rs = sckReadRs(device);
      float RL = driver.readResistor(device + 2); //Ohm
      /*Correccion de impedancia de carga*/
      if ((Rs <= (RL - 1000))||(Rs >= (RL + 1000)))
      {
        if (Rs < 2000) driver.writeResistor(device + 2, 2000);
        else driver.writeResistor(device + 2, Rs);
        delay(100);
        Rs = sckReadRs(device);
      }
       return Rs;
  }
  
  void sckGetMICS(float* __RsCO, float* __RsNO2){          
       
        /*Correccion de la tension del Heather*/
        sckHeat(CO_SENSOR, 32); //Corriente en mA
        sckHeat(NO2_SENSOR, 26); //Corriente en mA
        
        *__RsCO = sckReadMICS(CO_SENSOR);
        *__RsNO2 = sckReadMICS(NO2_SENSOR);
         
  }
         
  uint16_t sckGetLight(){
      uint8_t TIME0  = 0xDA;
      uint8_t GAIN0 = 0x00;
      uint8_t DATA [8] = {0x03, TIME0, 0x00 ,0x00, 0x00, 0xFF, 0xFF ,GAIN0} ;
      
      uint16_t DATA0 = 0;
      uint16_t DATA1 = 0;
      
      Wire.beginTransmission(BH1730);
      Wire.write(0x80|0x00);
      for(int i= 0; i<8; i++) Wire.write(DATA[i]);
      Wire.endTransmission();
      delay(100); 
      Wire.beginTransmission(BH1730);
      Wire.write(0x94);	
      Wire.endTransmission();
      Wire.requestFrom(BH1730, 4);
      DATA0 = Wire.read();
      DATA0=DATA0|(Wire.read()<<8);
      DATA1 = Wire.read();
      DATA1=DATA1|(Wire.read()<<8);
        
      uint8_t Gain = 0x00; 
      if (GAIN0 == 0x00) Gain = 1;
      else if (GAIN0 == 0x01) Gain = 2;
      else if (GAIN0 == 0x02) Gain = 64;
      else if (GAIN0 == 0x03) Gain = 128;
      
      float ITIME =  (256- TIME0)*2.7;
      
      float Lx = 0;
      float cons = (Gain * 100) / ITIME;
      float comp = (float)DATA1/DATA0;

      
      if (comp<0.26) Lx = ( 1.290*DATA0 - 2.733*DATA1 ) / cons;
      else if (comp < 0.55) Lx = ( 0.795*DATA0 - 0.859*DATA1 ) / cons;
      else if (comp < 1.09) Lx = ( 0.510*DATA0 - 0.345*DATA1 ) / cons;
      else if (comp < 2.13) Lx = ( 0.276*DATA0 - 0.130*DATA1 ) / cons;
      else Lx=0;
      
       #if debuggSCK
        SerialUSB.print("BH1730: ");
        SerialUSB.print(Lx);
        SerialUSB.println(" Lx");
      #endif
     return Lx;
  }
 
  
  unsigned int sckGetNoise() {  
    #define GAIN1 0
    #define GAIN2 400
    sckWriteGAIN(GAIN1,GAIN2);  
    float mVRaw = (float)((driver.average(S4))/1023.)*VCC;
    float dB = 0;
    
    #if debuggSCK
      SerialUSB.print("nOISE: ");
      SerialUSB.print(mVRaw);
      SerialUSB.print(" mV, RSpu: ");
      SerialUSB.print(driver.readResistor(6));
      SerialUSB.print(", Ramp: ");
      SerialUSB.print(driver.readResistor(7));
      SerialUSB.print(", GAIN: ");
      SerialUSB.println(sckReadGAIN());
    #endif
    return mVRaw; 
  }

