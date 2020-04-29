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
  Serial.begin(115200);
  
  //stop interrupts
  cli();

  //Init BSB I/O
  BSB_init();

  //Enable global interrupts  
  sei();
  
  //Start system idle timeout timer
  idle_timeout(ON); 

}


/*MAIN*/
void loop() {
  
  //Check autoshutdown
  if (auto_timeout() == ON){                                //Check autoshutdown flag
    Turn_OFF_BSB();
  }

}
