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
#define IO3 4        

#define P_WIFI  0
#define GPIO0   11
#define RST_ESP 43
#define CS_ESP  13
#define CS_MMC  2
#define CH_PD   1
#define GPIO16  5
#define RED     6
#define GREEN   12
#define BLUE    10

#define CONTROL 7     // PIN CONTROL S2

#define S0 A3         //CO_SENSOR
#define S1 A4         //NO2_SENSOR
#define S2 A1         //SENS_CO CURRENT
#define S3 A2         //SENS_NO2 CURRENT
#define S4 A5         //MICRO
#define S5 A0         



#define DEFAULT_TIME_UPDATE  "30"     //Tiempo entre actualizacion y actualizacion
#define DEFAULT_MIN_UPDATES  "1"      //Minimo numero de actualizaciones antes de postear

#define POST_MAX             20       //Maximo numero de posteos a la vez

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

//Parametros SI1145
/* COMMANDS */
#define SI1145_PARAM_QUERY 0x80
#define SI1145_PARAM_SET 0xA0
#define SI1145_NOP 0x0
#define SI1145_RESET    0x01
#define SI1145_BUSADDR    0x02
#define SI1145_PS_FORCE    0x05
#define SI1145_ALS_FORCE    0x06
#define SI1145_PSALS_FORCE    0x07
#define SI1145_PS_PAUSE    0x09
#define SI1145_ALS_PAUSE    0x0A
#define SI1145_PSALS_PAUSE    0xB
#define SI1145_PS_AUTO    0x0D
#define SI1145_ALS_AUTO   0x0E
#define SI1145_PSALS_AUTO 0x0F
#define SI1145_GET_CAL    0x12

/* Parameters */
#define SI1145_PARAM_I2CADDR 0x00
#define SI1145_PARAM_CHLIST   0x01
#define SI1145_PARAM_CHLIST_ENUV 0x80
#define SI1145_PARAM_CHLIST_ENAUX 0x40
#define SI1145_PARAM_CHLIST_ENALSIR 0x20
#define SI1145_PARAM_CHLIST_ENALSVIS 0x10
#define SI1145_PARAM_CHLIST_ENPS1 0x01
#define SI1145_PARAM_CHLIST_ENPS2 0x02
#define SI1145_PARAM_CHLIST_ENPS3 0x04

#define SI1145_PARAM_PSLED12SEL   0x02
#define SI1145_PARAM_PSLED12SEL_PS2NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS2LED1 0x10
#define SI1145_PARAM_PSLED12SEL_PS2LED2 0x20
#define SI1145_PARAM_PSLED12SEL_PS2LED3 0x40
#define SI1145_PARAM_PSLED12SEL_PS1NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS1LED1 0x01
#define SI1145_PARAM_PSLED12SEL_PS1LED2 0x02
#define SI1145_PARAM_PSLED12SEL_PS1LED3 0x04

#define SI1145_PARAM_PSLED3SEL   0x03
#define SI1145_PARAM_PSENCODE   0x05
#define SI1145_PARAM_ALSENCODE  0x06

#define SI1145_PARAM_PS1ADCMUX   0x07
#define SI1145_PARAM_PS2ADCMUX   0x08
#define SI1145_PARAM_PS3ADCMUX   0x09
#define SI1145_PARAM_PSADCOUNTER   0x0A
#define SI1145_PARAM_PSADCGAIN 0x0B
#define SI1145_PARAM_PSADCMISC 0x0C
#define SI1145_PARAM_PSADCMISC_RANGE 0x20
#define SI1145_PARAM_PSADCMISC_PSMODE 0x04

#define SI1145_PARAM_ALSIRADCMUX   0x0E
#define SI1145_PARAM_AUXADCMUX   0x0F

#define SI1145_PARAM_ALSVISADCOUNTER   0x10
#define SI1145_PARAM_ALSVISADCGAIN 0x11
#define SI1145_PARAM_ALSVISADCMISC 0x12
#define SI1145_PARAM_ALSVISADCMISC_VISRANGE 0x20

#define SI1145_PARAM_ALSIRADCOUNTER   0x1D
#define SI1145_PARAM_ALSIRADCGAIN 0x1E
#define SI1145_PARAM_ALSIRADCMISC 0x1F
#define SI1145_PARAM_ALSIRADCMISC_RANGE 0x20

#define SI1145_PARAM_ADCCOUNTER_511CLK 0x70

#define SI1145_PARAM_ADCMUX_SMALLIR  0x00
#define SI1145_PARAM_ADCMUX_LARGEIR  0x03



/* REGISTERS */
#define SI1145_REG_PARTID  0x00
#define SI1145_REG_REVID  0x01
#define SI1145_REG_SEQID  0x02

#define SI1145_REG_INTCFG  0x03
#define SI1145_REG_INTCFG_INTOE 0x01
#define SI1145_REG_INTCFG_INTMODE 0x02

#define SI1145_REG_IRQEN  0x04
#define SI1145_REG_IRQEN_ALSEVERYSAMPLE 0x01
#define SI1145_REG_IRQEN_PS1EVERYSAMPLE 0x04
#define SI1145_REG_IRQEN_PS2EVERYSAMPLE 0x08
#define SI1145_REG_IRQEN_PS3EVERYSAMPLE 0x10


#define SI1145_REG_IRQMODE1 0x05
#define SI1145_REG_IRQMODE2 0x06

#define SI1145_REG_HWKEY  0x07
#define SI1145_REG_MEASRATE0 0x08
#define SI1145_REG_MEASRATE1  0x09
#define SI1145_REG_PSRATE  0x0A
#define SI1145_REG_PSLED21  0x0F
#define SI1145_REG_PSLED3  0x10
#define SI1145_REG_UCOEFF0  0x13
#define SI1145_REG_UCOEFF1  0x14
#define SI1145_REG_UCOEFF2  0x15
#define SI1145_REG_UCOEFF3  0x16
#define SI1145_REG_PARAMWR  0x17
#define SI1145_REG_COMMAND  0x18
#define SI1145_REG_RESPONSE  0x20
#define SI1145_REG_IRQSTAT  0x21
#define SI1145_REG_IRQSTAT_ALS  0x01

#define SI1145_REG_ALSVISDATA0 0x22
#define SI1145_REG_ALSVISDATA1 0x23
#define SI1145_REG_ALSIRDATA0 0x24
#define SI1145_REG_ALSIRDATA1 0x25
#define SI1145_REG_PS1DATA0 0x26
#define SI1145_REG_PS1DATA1 0x27
#define SI1145_REG_PS2DATA0 0x28
#define SI1145_REG_PS2DATA1 0x29
#define SI1145_REG_PS3DATA0 0x2A
#define SI1145_REG_PS3DATA1 0x2B
#define SI1145_REG_UVINDEX0 0x2C
#define SI1145_REG_UVINDEX1 0x2D
#define SI1145_REG_PARAMRD 0x2E
#define SI1145_REG_CHIPSTAT 0x30

#define SI1145_ADDR 0x60


