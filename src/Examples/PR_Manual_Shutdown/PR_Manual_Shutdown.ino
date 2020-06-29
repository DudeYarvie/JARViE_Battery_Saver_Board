/*  BSB_Manual_Shutdown.ino
  Author: Jarvis Hill
  Purpose: Example showing how to use the JARViE Power Ranger module to power on and off system power using the module tactile switch
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
