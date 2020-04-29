/*

 Arduino TFT Bitmap Logo example

 This example reads an image file from a micro-SD card
 and draws it on the screen, at random locations.

 In this sketch, the Arduino logo is read from a micro-SD card.
 There is a .bmp file included with this sketch.
 - open the sketch folder (Ctrl-K or Cmd-K)
 - copy the "arduino.bmp" file to a micro-SD
 - put the SD into the SD slot of the Arduino TFT module.

 This example code is in the public domain.

 Created 19 April 2013 by Enrico Gueli

 http://www.arduino.cc/en/Tutorial/TFTBitmapLogo

 */

// include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library
#include "BSB.h"
#include "Timers.h"

// pin definition for the Uno
#define sd_cs  4
#define lcd_cs 10
#define dc     9
#define rst    8

// pin definition for the Leonardo
//#define sd_cs  8
//#define lcd_cs 7
//#define dc     0
//#define rst    1

TFT TFTscreen = TFT(lcd_cs, dc, rst);

// this variable represents the image to be drawn on screen
PImage logo;

void display_image(){
  // initialize the GLCD and show a message
  // asking the user to open the serial line
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  //Serial.print(F("Initializing SD card..."));
  if (!SD.begin(sd_cs)) {
    return;
  }

  delay(250);

  // now that the SD card can be access, try to load the
  // image file.
  logo = TFTscreen.loadImage("home.bmp");
  if (logo.isValid() == true) {
    
    //center picture in display
    int x = (TFTscreen.width() - logo.width())/2;
    int y = (TFTscreen.height() - logo.height())/2;
    
    // draw the image to the screen
    TFTscreen.image(logo, x, y);
  }
}

void setup() {
  //stop interrupts
  cli();

  //Init BSB I/O
  BSB_init();

  //Enable global interrupts  
  sei();

  //Power on TFT LCD
  DDRD |= (1<<DDD7);
  PORTD |= (1<<PIN7);
  delay(250);
  
  // initialize the GLCD and show a message
  // asking the user to open the serial line
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);

  TFTscreen.stroke(0, 0, 255);
  TFTscreen.println(F("Running BIOS..."));
  delay(5000);
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.println(F("Android 8.1 - OREO..."));
  delay(5000);

  // clear the GLCD screen before starting
  TFTscreen.background(255, 255, 255);
  delay(250);
  
  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  //Serial.print(F("Initializing SD card..."));
  if (!SD.begin(sd_cs)) {
    return;
  }
  delay(250);

  // now that the SD card can be access, try to load the
  // image file.
  //logo = TFTscreen.loadImage("GE_logo2.bmp");
  logo = TFTscreen.loadImage("icecream.bmp");
  if (logo.isValid() == true) {
    int x = (TFTscreen.width() - logo.width())/2;
    int y = (TFTscreen.height() - logo.height())/2;
    // draw the image to the screen
    TFTscreen.image(logo, x, y);
  }

  //Read when BRB SW1 is pressed
  //manual_shutdown_trig(ON);

  
  //Start system idle timeout for sleep mode
  idle_timeout(ON);   
}

void loop() {
//  //Manual shutdown using BRB SW1
//  if (MAN_SDSW_FLAG == 1){
//    manual_shutdown_init();
//  }

  //Go to sleep if autotimeout has occured
  if (auto_timeout() == ON){                                //Check autoshutdown flag
    
    //Power off TFT LCD
    PORTD &= ~(1<<PIN7);
    
    sleep_mode(POWER_DOWN, ON);                             //Put MCU to sleep

    while (SLEEP_MODE_FLAG);                                //Wait until MCU wakes up
    
    //Power on TFT LCD
    PORTD |= (1<<PIN7);

    //Display Homescreen image
    display_image();
  }

  
}
