#ifndef OXS_OUT_HOTT_h
#define OXS_OUT_HOTT_h

#include "oXs_config.h"
#include "oXs_ms5611.h" // we need the variodata struct
#include "oXs_4525.h" // we need the airspeeddata struct
#include "oXs_curr.h" // we need the currentdata struct
#include "oXs_voltage.h" // we need the arduinodata struct
//#include <Arduino.h>
#include "oXs_general.h"


#ifdef HOTT


struct t_mbOneData {
	uint8_t volatile active;
	uint8_t volatile response[3];
} ;

struct t_mbAllData {
  struct t_mbOneData mbData[16] ;
} ;  


#define UNKNOWN false
#define KNOWN true

/***************************************************************************************/
/* Transmission status                                                                 */ 
/***************************************************************************************/
#define TO_LOAD     0
#define LOADED      1
#define SENDING     2
#define SEND        3


#define SERIAL_COM_SPEED    19200

// Hott protocol v4 delay
#define HOTTV4_TX_DELAY 1600 // Delai entre octets

// first byte sent by Rx for polling can have 2 values
#define HOTT_BINARY_MODE_REQUEST_ID      0x80
#define HOTT_TEXT_MODE_REQUEST_ID       0x7f

// in binary mode, second byte identifies one of 4 sensor types
#define HOTT_TELEMETRY_GEA_SENSOR_ID    0x8E // Electric Air Module ID
#define HOTT_TELEMETRY_GAM_SENSOR_ID    0x8d  
#define HOTT_TELEMETRY_GPS_SENSOR_ID    0x8a //Graupner #33600 Gps module
#define HOTT_TELEMETRY_VARIO_SENSOR_ID  0x89 //Graupner #33601 Vario Module
// note : when sensor relies, it sent first 2 bytes with the value = ??????????????? and then it starts a set of bytes depending on the type of sensor.


// in text mode, second byte identifies one of 4 sensor types
#define HOTT_GPS_SENSOR_TEXT_ID 0xA0 // GPS Module ID          

#define TXHOTTDATA_BUFFERSIZE 45

// structure of GENERAL AIR MODULE 
typedef struct {
  byte start_byte;          //#01 start byte constant value 0x7c
  byte gam_sensor_id;       //#02 EAM sensort id. constat value 0x8d=GENRAL AIR MODULE
  byte warning_beeps;       //#03 1=A 2=B ... 0x1a=Z  0 = no alarm
                  /* VOICE OR BIP WARNINGS
                    Alarme sonore A.. Z, octet correspondant 1 à 26
                    0x00  00  0  No alarm
                    0x01  01  A  
                    0x02  02  B  Negative Difference 2 B
                          0x03  03  C  Negative Difference 1 C
                    0x04  04  D  
                    0x05  05  E  
                    0x06  06  F  Min. Sensor 1 temp. F
                    0x07  07  G  Min. Sensor 2 temp. G
                    0x08  08  H  Max. Sensor 1 temp. H
                              0x09  09  I  Max. Sensor 2 temp. I
                        0xA   10  J  Max. Sens. 1 voltage J
                  0xB   11  K  Max. Sens. 2 voltage K
                                    0xC   12  L  
                  0xD   13  M  Positive Difference 2 M
                  0xE   14  N  Positive Difference 1 N
                  0xF   15  O  Min. Altitude O
                  0x10  16  P  Min. Power Voltage P    // We use this one for Battery Warning
                  0x11  17  Q  Min. Cell voltage Q
                  0x12  18  R  Min. Sens. 1 voltage R
                  0x13  19  S  Min. Sens. 2 voltage S
                  0x14  20  T  Minimum RPM T
                  0x15  21  U  
                  0x16  22  V  Max. used capacity V
                  0x17  23  W  Max. Current W
                  0x18  24  X  Max. Power Voltage X
                  0x19  25  Y  Maximum RPM Y
                  0x1A  26  Z  Max. Altitude Z
                        */
  byte sensor_id;                       //#04 constant value 0xd0
  byte alarm_invers1;                   //#05 alarm bitmask. Value is displayed inverted
              //Bit#  Alarm field
            // 0    all cell voltage
            // 1    Battery 1
            // 2    Battery 2
            // 3    Temperature 1
            // 4    Temperature 2
            // 5    Fuel
            // 6    mAh
            // 7    Altitude
  byte alarm_invers2;                    //#06 alarm bitmask. Value is displayed inverted
                //Bit#  Alarm Field
                // 0    main power current
                // 1    main power voltage
                // 2    Altitude
                // 3    m/s                            
                  // 4    m/3s
                  // 5    unknown
                // 6    unknown
                // 7    "ON" sign/text msg active
  byte cell[6];                         //#7 Volt Cell 1 (in 2 mV increments, 210 == 4.20 V)
                                        //#8 Volt Cell 2 (in 2 mV increments, 210 == 4.20 V)
                                        //#9 Volt Cell 3 (in 2 mV increments, 210 == 4.20 V)
                                        //#10 Volt Cell 4 (in 2 mV increments, 210 == 4.20 V)
                                        //#11 Volt Cell 5 (in 2 mV increments, 210 == 4.20 V)
                                        //#12 Volt Cell 6 (in 2 mV increments, 210 == 4.20 V)
  uint16_t  Battery1;                   //#13 LSB battery 1 voltage LSB value. 0.1V steps. 50 = 5.5V only pos. voltages
                                        //#14 MSB 
  uint16_t  Battery2;                   //#15 LSB battery 2 voltage LSB value. 0.1V steps. 50 = 5.5V only pos. voltages
                                        //#16 MSB
  byte temperature1;                    //#17 Temperature 1. Offset of 20. a value of 20 = 0°C
  byte temperature2;                    //#18 Temperature 2. Offset of 20. a value of 20 = 0°C
  byte fuel_procent;                    //#19 Fuel capacity in %. Values 0--100
                                        //graphical display ranges: 0-100% with new firmwares of the radios MX12/MX20/...
  uint16_t fuel_ml;                     //#20 LSB Fuel in ml scale. Full = 65535!
                                        //#21 MSB
  uint16_t rpm;                         //#22 RPM in 10 RPM steps. 300 = 3000rpm
                                        //#23 MSB
  uint16_t altitude;                    //#24 altitude in meters. offset of 500, 500 = 0m
                                        //#25 MSB
  uint16_t climbrate_L;                 //#26 climb rate in 0.01m/s. Value of 30000 = 0.00 m/s
                                        //#27 MSB
  byte climbrate3s;                     //#28 climb rate in m/3sec. Value of 120 = 0m/3sec
  uint16_t current;                     //#29 current in 0.1A steps 100 == 10,0A
                                        //#30 MSB current display only goes up to 99.9 A (continuous)
  uint16_t main_voltage;                //#31 LSB Main power voltage using 0.1V steps 100 == 10,0V
                                        //#32 MSB (Appears in GAM display right as alternate display.)
  uint16_t batt_cap;                    //#33 LSB used battery capacity in 10mAh steps
                                        //#34 MSB 
  uint16_t speed;                       //#35 LSB (air?) speed in km/h(?) we are using ground speed here per default
                                        //#36 MSB speed
  byte min_cell_volt;                   //#37 minimum cell voltage in 2mV steps. 124 = 2,48V
  byte min_cell_volt_num;               //#38 number of the cell with the lowest voltage
  uint16_t rpm2;                        //#39 LSB 2nd RPM in 10 RPM steps. 100 == 1000rpm
                                        //#40 MSB
  byte general_error_number;            //#41 General Error Number (Voice Error == 12) TODO: more documentation
  byte pressure;                        //#42 High pressure up to 16bar. 0,1bar scale. 20 == 2.0bar
  byte version;                         //#43 version number (Bytes 35 .43 new but not yet in the record in the display!)
  byte stop_byte;                       //#44 stop byte 0x7D
  byte parity;                          //#45 CHECKSUM CRC/Parity (calculated dynamicaly)
} HOTT_GAM_MSG ;



class OXS_OUT {
  public:
#ifdef DEBUG  
    OXS_OUT(uint8_t pinTx,HardwareSerial &print);
#else
    OXS_OUT(uint8_t pinTx);
#endif
    VARIODATA* varioData ;
    VARIODATA* varioData_2 ;
    AIRSPEEDDATA* airSpeedData ;
    CURRENTDATA* currentData ;
    VOLTAGEDATA* voltageData ;
    void setup();
    void sendData();
    
  private:
// used by both protocols  
    uint8_t _pinTx;
#ifdef DEBUG  
    HardwareSerial* printer;
#endif

    void formatAllMultiplexData() ; 
    uint8_t formatOneValue ( uint8_t currentFieldToSend ) ;
    void setMultiplexNewData(  uint16_t id, int32_t value  , uint8_t alarm) ;
};

extern int ppm ; 
extern bool ppmAvailable ;
extern int32_t compensatedClimbRate ;
extern bool compensatedClimbRateAvailable ;
extern int32_t switchVSpeed ; // used to transmit the selected Vspeed
extern bool switchVSpeedAvailable ;
extern int32_t averageVSpeed ; // used to transmit the average Vspeed
extern bool averageVSpeedAvailable ;
extern int32_t test1Value ;// used in order to test the transmission of any value
extern bool test1ValueAvailable ;
extern int32_t test2Value ;// used in order to test the transmission of any value
extern bool test2ValueAvailable ;
extern int32_t test3Value ;// used in order to test the transmission of any value
extern bool test3ValueAvailable ;
extern uint8_t selectedVario ;

#ifdef MEASURE_RPM
extern volatile uint16_t RpmValue ;
extern bool RpmAvailable ;
#endif // MEASURE_RPM


extern volatile uint8_t debug01 ;
extern volatile uint8_t debug02 ;
extern volatile uint8_t debug03 ;
extern volatile uint8_t debug04 ;

 
void setHottNewData( struct t_sportData * volatile pdata, uint16_t id, uint32_t value ) ;
void initHottUart( ) ;


extern volatile bool RpmSet ;
extern volatile uint16_t RpmValue ;


// UART's state.
#define   IDLE               0        // Idle state, both transmit and receive possible.
#define   TRANSMIT           1        // Transmitting byte.
#define   TRANSMIT_STOP_BIT  2        // Transmitting stop bit.
#define   RECEIVE            3        // Receiving byte.
#define   TxPENDING          4
#define   WAITING            5


//This section chooses the correct timer values for Hott protocol = 19200 baud.
  #if F_CPU == 20000000L     // 20MHz clock                                                  
    // Sinan: Not tested
    #define TICKS2COUNTHOTT         (1044)  // Ticks between two bits.
    #define TICKS2WAITONEHOTT       (1044)  // Wait one bit period.
    #define TICKS2WAITONE_HALFHOTT  (1560)    // Wait one and a half bit period.
  #elif F_CPU == 16000000L   // 16MHz clock                                                  
    #define TICKS2COUNTHOTT         (834)  // Ticks between two bits.
    #define TICKS2WAITONEHOTT       (834)  // Wait one bit period.
    #define TICKS2WAITONE_HALFHOTT  (1248)    // Wait one and a half bit period.
  #elif F_CPU == 8000000L    // 8MHz clock                                                   
    #define TICKS2COUNTHOTT         (417)  // Ticks between two bits.
    #define TICKS2WAITONEHOTT       (417)  // Wait one bit period.
    #define TICKS2WAITONE_HALFHOTT  (624)    // Wait one and a half bit period.
  #else
    #error Unsupported clock speed
  #endif



//#define INTERRUPT_EXEC_CYCL   90       // Cycles to execute interrupt routines from interrupt.
//#define INTERRUPT_EARLY_BIAS  32       // Cycles to allow of other interrupts.
// INTERRUPT_EARLY_BIAS is to bias the sample point a bit early in case
// the Timer 0 interrupt (5.5uS) delays the start bit detection
  #if F_CPU == 20000000L     // 20MHz clock                                                  
    #define INTERRUPT_EXEC_CYCL   112       // Cycles to execute interrupt routines from interrupt.
    #define INTERRUPT_EARLY_BIAS  40       // Cycles to allow of other interrupts.
  #elif F_CPU == 16000000L   // 16MHz clock                                                  
    #define INTERRUPT_EXEC_CYCL   90       // Cycles to execute interrupt routines from interrupt.
    #define INTERRUPT_EARLY_BIAS  32       // Cycles to allow of other interrupts.
  #elif F_CPU == 8000000L    // 8MHz clock                                                   
    #define INTERRUPT_EXEC_CYCL   45       // Cycles to execute interrupt routines from interrupt.
    #define INTERRUPT_EARLY_BIAS  16       // Cycles to allow of other interrupts.
  #else
    #error Unsupported clock speed
  #endif

// this section define some delays used ; values can be used by any protocol
  #if F_CPU == 20000000L     // 20MHz clock                                                  
    #define DELAY_4000  ((uint16_t)4000.0 * 20.0 /16.0 )
    #define DELAY_3500  ((uint16_t)3500.0 * 20.0 /16.0 )    
    #define DELAY_2000  ((uint16_t)2000.0 * 20.0 /16.0 )
    #define DELAY_1600  ((uint16_t)1600.0 * 20.0 /16.0 )    
    #define DELAY_1000  ((uint16_t)1000.0 * 20.0 /16.0 )    
    #define DELAY_400  ((uint16_t)400.0 * 20.0 /16.0 )
    #define DELAY_100  ((uint16_t)100.0 * 20.0 /16.0 )
    
  #elif F_CPU == 16000000L   // 16MHz clock                                                  
    #define DELAY_4000 ((uint16_t) (4000L * 16) )     
    #define DELAY_3500 ((uint16_t) (3500L * 16) )         
    #define DELAY_2000 ((uint16_t) (2000L * 16) )     
    #define DELAY_1600 ((uint16_t) (1600L * 16) )     
    #define DELAY_1000 ((uint16_t) (1000L * 16) )     
    #define DELAY_400 ((uint16_t) (400 * 16) )     
    #define DELAY_100 ((uint16_t) (100 * 16) )     
  #elif F_CPU == 8000000L    // 8MHz clock                                                   
    #define  DELAY_4000 ((uint16_t)4000L * 8 )
    #define  DELAY_3500 ((uint16_t)3500L * 8 )    
    #define  DELAY_2000 ((uint16_t)2000 * 8 )
    #define  DELAY_1600 ((uint16_t)1600 * 8 )    
    #define  DELAY_1000 ((uint16_t)1000 * 8 )    
    #define  DELAY_400 ((uint16_t)400 * 8 )
    #define  DELAY_100 ((uint16_t)100 * 8 )    
  #else
    #error Unsupported clock speed
  #endif

#define TCCR             TCCR1A             //!< Timer/Counter Control Register
#define TCCR_P           TCCR1B             //!< Timer/Counter Control (Prescaler) Register
#define OCR              OCR1A              //!< Output Compare Register
#define EXT_IFR          EIFR               //!< External Interrupt Flag Register
#define EXT_ICR          EICRA              //!< External Interrupt Control Register

#define TRXDDR  DDRD
#define TRXPORT PORTD
#define TRXPIN  PIND

#define SET_TX_PIN( )    ( TRXPORT |= ( 1 << PIN_SERIALTX ) )
#define CLEAR_TX_PIN( )  ( TRXPORT &= ~( 1 << PIN_SERIALTX ) )
#define GET_RX_PIN( )    ( TRXPIN & ( 1 << PIN_SERIALTX ) )

#define SET_TX_PIN_MB( ) ( TRXDDR &= ~( 1 << PIN_SERIALTX ) ) // in fact we put the port in high impedance because the rx has a pull up resistance
#define CLEAR_TX_PIN_MB( ) ( TRXDDR |= ( 1 << PIN_SERIALTX ) ) // in fact we put the port in output mode. PORT is already set to 0 during initialisation

// values used by the state ".active"
#define NOT_ACTIVE     0
#define NOT_AVAILABLE  1
#define LOCKED         2
#define AVAILABLE      3

#endif // End of HOTT
#endif // OXS_OUT_HOTT_h

/*
In Hott protocol, the Rx send a polling to each of the possible sensors (there are 5 types of sensors).
Each sensor is in fact polled twice: one asking to reply in a text format and another in a binary format.
This version of oXs replies only to the request for a binary format sent to a GAM (general air module)
Later on, it would be possible to simulate other types of module (like a GPS, a vario,... )
It seems that for binary format, the polling is every 200 msec. Still this strange because the reply of only one sensor takes already about 140 msec (45 bytes * 3msec/byte).
When the sensor identifies that it must reply to a polling, it has to wait 5 msec before sending the first byte and afterward it can send all the bytes (45 in total in binary GAM format) keeping 3 msec between each byte.
It seems that the delay of 3 msec can be reduced to less than 1 msec.
The format of the message is given here above. 
It is possible to send some info to reverse some fields on the display and to activate some alarms (but this version does not support it)
*/

