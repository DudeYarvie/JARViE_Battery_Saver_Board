/*  BSB_Autotimeout.ino
  Author: Jarvis Hill
  Purpose: Example showing how use the JARViE Battery Saver Board to disconnect system power after a specified amout of time
  Date: 15-MAR-2020

  Version: 
          1.0 - Initial Release
          
  Refrences:
*/


/*INCLUDES*/
#include "BSB.h"
#include "Timers.h"

/*GLOBALS*/







void setup() {

  //Init serial comms
  //Serial.begin(115200);
  
  //stop interrupts
  cli();

  //Init BSB I/O
  BSB_init();

  //Enable global interrupts  
  sei();
  
  //Read when BRB SW1 is pressed
  manual_shutdown_trig(ON);

  
}


/*MAIN*/
void loop() {
  
  //Manual shutdown using BRB SW1
  if (MAN_SDSW_FLAG == 1){
    manual_shutdown_init();
  }
  

}
