#include "Arduino.h"        //Allows DDRD, PORTD syntax 
#include "BSB.h"
#include "Timers.h"


/*GLOBALS*/
#define IDLE_TIMEOUT 10                          //System idle timeout in seconds (turn off system power if no user input for spec idle time)
uint8_t MAN_SDSW_FLAG = 0; 

//Timing 
volatile uint32_t CNTR_TICKS = 0;
volatile uint8_t EDGE_CNT = 0;
//volatile boolean MEAS_COMPLETE = false;
uint32_t EDGE_TIMES[2] = {0x0000, 0x0000};
uint32_t ELAPSED_TIME = 0;

//Low Power
uint8_t SLEEP_MODE_FLAG = 0;
uint8_t TURN_OFF_FLAG = 0;

//Autotimeout
uint8_t AUTO_TIMEOUT_FLAG = 0;


/*INTERRUPTS*/
/**********************************************************************
 * TIMER2 COMPA Interrupt VECTOR
 * Purpose: Tracks 100us ticks
 **********************************************************************/
SIGNAL(TIMER2_COMPA_vect) 
{

  CNTR_TICKS += 1;          //Increment tick counter.  

}


/**********************************************************************
 * External Interript 1 VECTOR
 * Purpose: Execute the following code when a rising edge occurs 
 **********************************************************************/
ISR(INT1_vect){
  //IF MCU is Waking up
  if (SLEEP_MODE_FLAG == ON){
    //Disable sleep mode
    SMCR = 0;
    
    //Reset sleep flag
    SLEEP_MODE_FLAG = 0;

    //Restart timer
    idle_timeout(ON);
  }
}


/**********************************************************************
 * External Interript 0 VECTOR
 * Purpose: Execute the following code when a rising edge occurs 
 **********************************************************************/
ISR(INT0_vect){

    MAN_SDSW_FLAG = 1;                          //Set manual shutdown switch pressed flag
}


/*******************************************************************************
 * BSB_init
 * Purpose: Init I/O for BSB
 *******************************************************************************/
void BSB_init(){
  
  //nOFF
  DDRD  |= (1 << BSB_nOFF_DDR);                //Set BSB nOFF signal to output to control BSB shutdown        
  PORTD &= ~(1 << BSB_nOFF);                   //Init signal to logic HIGH 
  
  //nON
  DDRD  &= ~(1 << BSB_nON_DDR);                //Set BSB nON signal to input to monitor BSB SW1 state    
  PORTD &= ~(1 << BSB_nON); 		       //Sets GPIO to Hi-Z       

  //Turn-off all PORTB GPIO
  DDRB  =  0xFF;
  PORTB &= 0x00;

  //Clear external interrupt control register
  EICRA = 0;      
  
  //Configs TIMER2 (1 TICK = 100us) 
  config_timers();         
}


/*******************************************************************************
 * Turn_OFF_BSB
 * Purpose: Turns the BSB off 
 *******************************************************************************/
void Turn_OFF_BSB(){

  //Pulse MAX1614 nOFF signal LOW to disconnect input power (IN+ & IN-) from output (OUT+ & OUT-)
  PORTD |= (1 << BSB_nOFF);                     
  delay(100);                                   //Update to measure CNTR_TICKS for desired delay time
  PORTD &= ~(1 << BSB_nOFF);                  

}


/*******************************************************************************
 * idle_timeout
 * Purpose: Start or stop system timer that tracks idle timeout
 *******************************************************************************/
void idle_timeout(uint8_t state){
   if (state == ON){  
    CNTR_TICKS = 0;                                                  //Reset TIMER tick counter
    TIMSK2 |= (1 << OCIE2A);                                         //Enable timer compare interrupt for Timer/Counter2 Output Compare Match A
   }
   if (state == OFF){
    TIMSK2 &= ~(1 << OCIE2A);                                        //Disable timer compare interrupt for Timer/Counter2 Output Compare Match A
    CNTR_TICKS = 0;                                                  //Reset TIMER tick counter
   }
}


/****************************************************************************
 * manual_shutdown_trig
 * Purpose: Enables/disables manual system shutdown functionality via
	    Arduino UNO (Atmega328p MCU) INT0 pin.
 ***************************************************************************/
void manual_shutdown_trig(uint8_t state){

  //EICRA = 0;      //clear external interrupt control register (delete once testing complete this has been moved to BSB init)
  
  //Configure external interrupt INT0 (Arduino digital pin 2) to trigger on any rising edge
  EICRA |= (1<<ISC01)|(1 << ISC00);

  if (state == ON) EIMSK  |=   (1 << INT0);       //Enable external interrupt INT0
  if (state == OFF) EIMSK &= ~(1 << INT0);        //Disable external interrupt INT0  
  
}


/****************************************************************************
 * wakeup_trig
 * Purpose: Activates or deactivates system wake functionality via 
	    Arduino UNO (Atmega328p MCU) INT1 pin.
 ***************************************************************************/
void wakeup_trig(uint8_t state){

  //EICRA = 0;      //clear external interrupt control register (delete once testing complete this has been moved to BSB init)
  
  //Configure external interrupt INT1 (Arduino digital pin 2) to trigger on any rising edge
  EICRA |= (1<<ISC11)|(1 << ISC10);

  if (state == ON) EIMSK  |=   (1 << INT1);       //Enable external interrupt INT1
  if (state == OFF) EIMSK &= ~(1 << INT1);        //Disable external interrupt INT1  
  
}


/****************************************************************************
 * manual_shutdown_init
 * Purpose: Ensures manual shutdown button is intentionally pressed or held before executing
 *          manaul shutdown sequence
 ***************************************************************************/
void manual_shutdown_init(){

   //Enable TIMER
   idle_timeout(ON);

   //Wait 50 ms to software debounce SW
   while (CNTR_TICKS < (1*1e3));
    
   //Start TIMER to determine how long shutdown SW1 is held
   EDGE_TIMES[0] = CNTR_TICKS;
   //Serial.println(EDGE_TIMES[0]);                         //DEBUG
   
   //Check if SW1 is still being pressed (PD2 = HIGH)
   pinMode(2, INPUT);                                       //sets the digital pin 2 as input
   
   //Wait for INT1 (PD2) to return LOW 
   while(1){
     MAN_SDSW_FLAG = digitalRead(2);                       // read the input pin
     if (MAN_SDSW_FLAG == 0) break;
   }
   
   //Capture time when shutdown button, SW1, was released
   EDGE_TIMES[1] = CNTR_TICKS;
   while (CNTR_TICKS < (1*1e3));                            //Wait 50 ms to software debounce SW
   //Serial.println(EDGE_TIMES[1]);                         //DEBUG
   
   
   //IF SW1 was pressed for > 3 secs, shutdown
   if ((EDGE_TIMES[1] -  EDGE_TIMES[0]) > (3*1e4)){
    CNTR_TICKS = 0;                                         //Reset tick counter in order to implement delay below
    PORTD |= (1 << BSB_nOFF);                               //IF SW1 is held longer than 3 seconds turn-off system  
    while (CNTR_TICKS < (1*1e4));                           //Delay 1 second    
    PORTD &= ~(1 << BSB_nOFF); 
    //Serial.println("shutdown");                             //FOR DEBUG
   }

}


/**********************************************************************
 * auto_timeout
 * Purpose: Checks if autotimeout period has been reached
 **********************************************************************/
uint8_t auto_timeout(){

  //Check if autotimeout has occured
  if (CNTR_TICKS < (IDLE_TIMEOUT*1e4)){
    AUTO_TIMEOUT_FLAG = OFF;
    return OFF;   
  }
  else{
    AUTO_TIMEOUT_FLAG = ON;
    return ON;
  }
          
}


/**********************************************************************
 * gpio_sleep_state
 * Purpose: Enable/disable gpio for MCU sleep/low-power mode
 **********************************************************************/
//void gpio_sleep_state(uint8_t state){
//  
//  if (state == OFF){
//    //Turn-off all PORTB GPIO
//    DDRB  =  0xFF;
//    PORTB &= 0x00;
//
//    //Turn off PORTD GPIO except PD2 (INT0) and PD3 (INT1)
//    DDRD  |=  0xFF;
//    PORTD &=  0b00001100;         //BIT MASK
//  }
//  else{
//    return;                       //Do nothing, created for readibily 
//  }
//}
  

/**********************************************************************
 * sleep_mode
 * Purpose: Place MCU in sleep/low-power mode and enable/disable it
 **********************************************************************/
void sleep_mode(uint8_t mode, uint8_t state){
  //char buf[50];                                           //Create character buffer for print messages
  //sprintf(buf, "%d %d", mode, state);           
  //Serial.println(buf);                                    //DEBUG print function params that were passed in
  
  //Turn off TIMER
  idle_timeout(OFF);
  
  //Disable ADC
  ADCSRA = 0;                    

  //Disable all PORTB GPIO and only leaves PD2 and PD3 enabled for INTs
  //gpio_sleep_state(OFF);

  //Turn on INT0 (WAKE interrupt) and set trigger to rising edge
  wakeup_trig(ON);

  //Set sleep mode flag
  SLEEP_MODE_FLAG = ON;
  
  //Config sleep mode params
  PRR = 0;                                     //Turn-off all peripheral clocks via power reduciton reg
  SMCR = 0;                                    //Clear sleep mode control register

  SMCR |= (1<<SM1);                            //Put MCU to sleep by setting sleep mode = POWER DOWN 
                                               //(MCU does not go to sleep until the mode is selected after sleep is enabled)
  cli();
  SMCR |= (1 << SE);                           //Enable sleep mode
  sei();                                       //ensure interrupts enabled so we can wake up again
  asm volatile("SLEEP");                       //Issue sleep assembly instruction to put MCU to sleep
   
}

 
