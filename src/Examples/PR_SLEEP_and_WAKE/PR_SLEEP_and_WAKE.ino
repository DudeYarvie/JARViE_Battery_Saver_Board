/*  PR_SLEEP_and_WAKE.ino
  Author: Jarvis Hill
  Purpose: Example showing how to use the JARViE Power Ranger module to wakeup an MCU placed in low-power/sleep mode
  Date: 27-JUNE-2020

  Version: 
          1.0 - Initial Release
          
  Refrences:
*/


/*INCLUDES*/
#include "BSB.h"
#include "Timers.h"


/*SETUP*/
void setup() {

  //Init serial comms
  //Serial.begin(115200);
  
  //stop interrupts
  cli();

  //Init BSB I/O
  BSB_init();

  //Enable global interrupts  
  sei();

  //Start system idle timeout 
  idle_timeout(ON);                                        
    
}


/*MAIN*/
void loop() {
  
  //Go to sleep if idle autotimeout has occured
  if (auto_timeout() == ON){                                //Check autoshutdown flag
    sleep_mode(POWER_DOWN, ON);                             //Put MCU to sleep
  }
  
}
