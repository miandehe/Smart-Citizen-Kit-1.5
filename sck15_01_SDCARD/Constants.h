#define debuggSCK     false
#define decouplerComp true
#define DataRaw       true

#define TWI_FREQ 100000L //Frecuencia bus I2C

/* 

ARDUINO ports definitions - GPIOS and ADCs 

*/

#define AWAKE   4      // WIFLY AWAKE
#define PANEL   A8     // PANEL LEVEL
#define LEVEL   A7     // BAT LEVEL 

#define IO0 5         // CO SENSOR_HEATHER
#define IO1 13        // NO2 SENSOR_HEATHER
#define IO2 9         
#define IO3 10        

//SHIFT REGISTER CONTROL
#define DS 7          // DS 74HC595
#define STCP 4        // STCP 74HC595 
#define SHCP 12       // SHCP 74HC595 

#define PSEL    0
#define P_WIFI  1
#define GPIO0   2
#define RST_ESP 3
#define CH_PD   4
#define GPIO16  5
#define OTG     6
#define SEL_MUX 7

#define CONTROL 8     // PIN CONTROL S2

#define S0 A4         //CO_SENSOR
#define S1 A5         //NO2_SENSOR
#define S2 A2         //SENS_CO CURRENT
#define S3 A3         //SENS_NO2 CURRENT
#define S4 A0         //MICRO
#define S5 A1         



#define DEFAULT_TIME_UPDATE  "30"     //Tiempo entre actualizacion y actualizacion
#define DEFAULT_MIN_UPDATES  "1"      //Minimo numero de actualizaciones antes de postear

#define POST_MAX             20       //Maximo numero de posteos a la vez

/* 
i2c ADDRESSES 
*/

#define RTC_ADDRESS          0x6F    // Direction of the RTC
#define E2PROM               0x50    // Direction of the EEPROM
#define ACCEL                0x1D    // Direction of the LSM303D
#define CHARGER              0x6B    // Direction of the BQ24292

#define POT1                 0x51    // Direction of the Potenciometer 1 for MICS heather voltage
#define POT2                 0x52    // Direction of the Potenciometer 2 for MICS measure
#define POT3                 0x50    // Direction of the Potenciometer 3 for sensor audio
#define SI1145               0x60 
#define BH1730               0x29    // Direction of the light sensor
#define Temperature          0x40    // Direction of the sht25    

//Espacio reservado para los parametros de configuracion del SCK  
#define EE_ADDR_TIME_VERSION                        0   //32BYTES 
#define EE_ADDR_TIME_UPDATE                         32  //16BYTES Tiempo entre actualizacion y actualizacion de los sensores en segundos
#define EE_ADDR_NUMBER_UPDATES                      48  //4BYTES  Numero de actualizaciones antes de postear


#define CO_SENSOR 0x00
#define NO2_SENSOR 0x01

#define  Rc0  10. //Ohm  Resistencia medida de corriente en el sensor CO sensor
#define  Rc1  39. //Ohm Resistencia medida de corriente en el sensor NO2 sensor
#define  R2  24000. //Ohm Resistencia de los reguladores
#define  kr  392.1568     //Constante de conversion a resistencia de potenciometrosen ohmios

#define  VCC   3300. //mV 

#define  VAL_MAX_BATTERY                             4200
#define  VAL_MIN_BATTERY                             3000



