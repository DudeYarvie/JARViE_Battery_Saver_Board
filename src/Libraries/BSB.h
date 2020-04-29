#ifndef BSB_H
#define BSB_H

//BSB DIO
#define BSB_nOFF_DDR DDD5
#define BSB_nOFF PIN5             //Arduino pin 5
#define BSB_nON_DDR DDD6    
#define BSB_nON PIN6              //Arduino pin 6

//Commands
#define ON  1
#define OFF 0

//Flags
extern uint8_t MAN_SDSW_FLAG;
extern uint8_t TURN_OFF_FLAG;
extern uint8_t SLEEP_MODE_FLAG;
extern uint8_t AUTO_TIMEOUT_FLAG;

//Low-Power Settings
enum sleep : uint8_t {_IDLE, ADC_NOISE, POWER_DOWN, POWER_SAVE, STANDBY, EXT_STANDBY};

//! Inits JARViE Battery Saver Board I/O
//! @return void
void BSB_init(
			);

//! Turns on JARViE Battery Saver Board
//! @return void
void Turn_OFF_BSB(
      );

//! Enables system idle timeout timer
//! @return void
void idle_timeout(uint8_t state                             //State is either ON or OFF
      );


//! Enables manual system shutdown using BRB SW1
//! @return void
void manual_shutdown_trig(uint8_t state                     //State is either ON or OFF
      );


//! Allows MCU to be woken up by pressing button attached to Atmega328 portD pin 3
//! @return void
void wakeup_trig(uint8_t state
      );


//! Starts manual system shutdown process
//! @return void
void manual_shutdown_init(
      );


//! Waits user specified # of seconds before turning of BSB output power
//! @return void
uint8_t auto_timeout(
      );


////! Provides ability to turn off PORTB and PORTD GPIO when putting MCU to sleep
////! @return void
//void gpio_sleep_state(uint8_t state                         //State is either ON or OFF
//     );


//! Places MCU in sleep mode reducting power consumption
//! @return void
void sleep_mode(uint8_t mode,                               //Sleep mode selection
                uint8_t state                               //State is either ON or OFF
     );      

                                          

#endif  // BSB_H
