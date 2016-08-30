#define debuggSCK     false
#define DataRaw       true

#define TWI_FREQ 100000L //Frecuencia bus I2C

/* 

ARDUINO ports definitions - GPIOS and ADCs 

*/
#define PANEL_LEVEL   0     // PANEL LEVEL
#define BAT_LEVEL     1     // BAT LEVEL 

#define IO0 9         // CO SENSOR_HEATHER
#define IO1 8        // NO2 SENSOR_HEATHER
#define IO2 3         
#define IO3 2        

#define P_WIFI  35
#define GPIO0   11
#define PS      38
#define CS_ESP  13
#define CS_MMC  4
#define CH_PD   36
#define INT_POWER  5
#define REDpin     6
#define GREENpin   12
#define BLUEpin    10

#define CONTROL 7     // PIN CONTROL 

#define S0 A3         //CO_SENSOR
#define S1 A4         //NO2_SENSOR
#define S2 A1         //SENS_CO CURRENT
#define S3 A2         //SENS_NO2 CURRENT
#define S4 A5         //MICRO
#define S5 A0         



#define DEFAULT_TIME_UPDATE  "30"     //Tiempo entre actualizacion y actualizacion
#define DEFAULT_MIN_UPDATES  "1"      //Minimo numero de actualizaciones antes de postear

#define POST_MAX             20       //Maximo numero de posteos a la vez
#define RESOLUTION_ANALOG    4095.     //Resolucion de las entradas analogicas

/* 
i2c ADDRESSES 
*/

#define ACCMAG                0x1D    // Direction of the LSM303D


#define POT1                 0x50    
#define POT2                 0x51    // Direction of the Potenciometer 2 for MICS heather voltage
#define POT3                 0x52    // Direction of the Potenciometer 3 for MICS measure
#define POT4                 0x53    // Direction of the Potenciometer 4 for sensor audio

#define SI1145               0x60 
#define BH1730               0x29    // Direction of the light sensor
#define Temperature          0x40    // Direction of the sht21   
#define ADC_DIR              0x48    // Direction of the ADC

//Espacio reservado para los parametros de configuracion del SCK  
#define EE_ADDR_TIME_VERSION                        0   //32BYTES 
#define EE_ADDR_TIME_UPDATE                         32  //16BYTES Tiempo entre actualizacion y actualizacion de los sensores en segundos
#define EE_ADDR_NUMBER_UPDATES                      48  //4BYTES  Numero de actualizaciones antes de postear

//Registros del acelerometro
#define OUT_X_L_A          0x28
#define OUT_X_L_M          0x08

#define CO_SENSOR 0x02
#define NO2_SENSOR 0x03

#define  Rc0  10. //Ohm  Resistencia medida de corriente en el sensor CO sensor
#define  Rc1  39. //Ohm Resistencia medida de corriente en el sensor NO2 sensor
#define  R2  24000. //Ohm Resistencia de los reguladores
#define  kr  392.1568     //Constante de conversion a resistencia de potenciometrosen ohmios

#define  VCC   3300. //mV 

#define  VAL_MAX_BATTERY                             4208
#define  VAL_MIN_BATTERY                             3000
