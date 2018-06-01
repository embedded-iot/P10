/*--------------------------------------------------------------------------------------

 dmd_test.cpp 
   Demo and example project for the Freetronics DMD, a 512 LED matrix display
   panel arranged in a 32 x 16 layout.

 Copyright (C) 2011 Marc Alexander (info <at> freetronics <dot> com)

 See http://www.freetronics.com/dmd for resources and a getting started guide.

 Note that the DMD library uses the SPI port for the fastest, low overhead writing to the
 display. Keep an eye on conflicts if there are any other devices running from the same
 SPI port, and that the chip select on those devices is correctly set to be inactive
 when the DMD is being written to.

 USAGE NOTES
 -----------

 - Place the DMD library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Get the TimerOne library from here: http://code.google.com/p/arduino-timerone/downloads/list
   or download the local copy from the DMD library page (which may be older but was used for this creation)
   and place the TimerOne library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Restart the IDE.
 - In the Arduino IDE, you can open File > Examples > DMD > dmd_demo, or dmd_clock_readout, and get it
   running straight away!

 * The DMD comes with a pre-made data cable and DMDCON connector board so you can plug-and-play straight
   into any regular size Arduino Board (Uno, Freetronics Eleven, EtherTen, USBDroid, etc)
  
 * Please note that the Mega boards have SPI on different pins, so this library does not currently support
   the DMDCON connector board for direct connection to Mega's, please jumper the DMDCON pins to the
   matching SPI pins on the other header on the Mega boards.

 This example code is in the public domain.
 The DMD library is open source (GPL), for more see DMD.cpp and DMD.h

--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Arial_Black_16_ISO_8859_1.h"
#include "Arial14.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{

  Serial.begin(9600);
   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true);   //true is normal (all pixels off), false is negative (all pixels on)
  delay(1000);
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{

  // clock();
  // testDrawChar();
  // testScroll();

  // halfPexel();
   
  // HienThi2Hang();
   
  // draw a border rectangle around the outside of the display
  // testBorder();

  // testDrawImage();

  //  // stripe chaser
  // testStripeChaser();
  //  testWritePixel();
  // testDrawLine();
  testChar();
}

void print(String s) {
  Serial.println(s);
}
void testChar() {
  dmd.clearScreen( true );
  // dmd.selectFont(Arial_Black_16);
  // dmd.selectFont(Arial_14);
  dmd.selectFont(Arial_Black_16_ISO_8859_1);
  for (int i = 32; i < 256; i++) {
    dmd.clearScreen( true );
    print(String(i));
    dmd.drawChar(  0,  0, (char)i , GRAPHICS_NORMAL);
    delay(2000);
  }
}
void clock() {
  // 10 x 14 font clock, including demo of OR and NOR modes for pixels so that the flashing colon can be overlayed
   dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16);
   dmd.drawChar(  0,  3, '2', GRAPHICS_NORMAL );
   dmd.drawChar(  7,  3, '3', GRAPHICS_NORMAL );
   dmd.drawChar( 17,  3, '4', GRAPHICS_NORMAL );
   dmd.drawChar( 25,  3, '5', GRAPHICS_NORMAL );
   dmd.drawChar( 15,  3, ':', GRAPHICS_OR     );   // clock colon overlay on
   delay( 1000 );
   dmd.drawChar( 15,  3, ':', GRAPHICS_NOR    );   // clock colon overlay off
   delay( 1000 );
   dmd.drawChar( 15,  3, ':', GRAPHICS_OR     );   // clock colon overlay on
   delay( 1000 );
   dmd.drawChar( 15,  3, ':', GRAPHICS_NOR    );   // clock colon overlay off
   delay( 1000 );
   dmd.drawChar( 15,  3, ':', GRAPHICS_OR     );   // clock colon overlay on
   delay( 1000 );
}

void testDrawChar() {
  int cot = 10;
  int hang = 0;
  dmd.clearScreen( true );
  dmd.selectFont(Arial_Black_16);
  dmd.drawChar(  cot,  hang, '3', GRAPHICS_OR );
  delay(5000);
}

void testScroll() {
  dmd.clearScreen( true );
  dmd.selectFont(Arial_Black_16);
  dmd.drawMarquee("Scrolling Text",14,(32*DISPLAYS_ACROSS)-1,0);
  long start=millis();
  long timer=start;
  boolean ret=false;
  while(!ret){
    if ((timer+100) < millis()) {
      ret=dmd.stepMarquee(-1, 0);
      timer=millis();
    }
  }
}

void halfPexel() {
  dmd.clearScreen( true );
  dmd.selectFont(Arial_Black_16);
  // half the pixels on
   dmd.drawTestPattern( PATTERN_ALT_0 );
   delay( 1000 );

   // the other half on
   dmd.drawTestPattern( PATTERN_ALT_1 );
   delay( 1000 );

  dmd.drawTestPattern( PATTERN_STRIPE_0 );
   delay( 1000 );

  dmd.drawTestPattern( PATTERN_STRIPE_1 );
   delay( 1000 );
}

void HienThi2Hang() {
  // display some text
  //  dmd.clearScreen( true );  // Làm nháy màn màn hình
   dmd.selectFont(System5x7);
   for (byte x=0;x<DISPLAYS_ACROSS;x++) {
     for (byte y=0;y<DISPLAYS_DOWN;y++) {
       dmd.drawString(  2+(32*x),  1+(16*y), "freet", 5, GRAPHICS_NORMAL );
       dmd.drawString(  2+(32*x),  9+(16*y), "ronic", 5, GRAPHICS_NORMAL );
     }
   }
  //  while(1);
  delay( 500 );
}

void testBorder() {
   // draw a border rectangle around the outside of the display
   dmd.clearScreen( true );
   dmd.drawBox(  0,  0, (32*DISPLAYS_ACROSS)-1, (16*DISPLAYS_DOWN)-1, GRAPHICS_NORMAL );
   delay( 1000 );
   
}

void testDrawImage() {
  for (byte y=0;y<DISPLAYS_DOWN;y++) {
    for (byte x=0;x<DISPLAYS_ACROSS;x++) {
       // draw an X
       int ix=32*x;
       int iy=16*y;
       dmd.drawLine(  0+ix,  0+iy, 11+ix, 15+iy, GRAPHICS_NORMAL );
       dmd.drawLine(  0+ix, 15+iy, 11+ix,  0+iy, GRAPHICS_NORMAL );
       delay( 1000 );
   
       // draw a circle
       dmd.drawCircle( 16+ix,  8+iy,  5, GRAPHICS_NORMAL );
       delay( 1000 );
   
       // draw a filled box
       dmd.drawFilledBox( 24+ix, 3+iy, 29+ix, 13+iy, GRAPHICS_NORMAL );
       delay( 1000 );
     }
   }
}

void testStripeChaser() {
    for(byte b = 0 ; b < 20 ; b++ )
   {
      dmd.drawTestPattern( (b&1)+PATTERN_STRIPE_0 );
      delay( 200 );      
   }
   delay( 200 );      
}

void testWritePixel(){
  dmd.clearScreen(true);
  dmd.writePixel(1, 1, GRAPHICS_NORMAL, true);
  delay(1000);
}
void testDrawLine() {
   dmd.clearScreen(true);
  dmd.drawLine(0,1, 31, 1, GRAPHICS_NORMAL);
  dmd.drawLine(0,3, 31, 3, GRAPHICS_NORMAL);
   dmd.drawLine(0,0, 15, 10, GRAPHICS_NORMAL);
  delay(1000);
}