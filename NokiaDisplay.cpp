/**
*******************************************************************************
NokiaDisplay.cpp

NokiaDisplay, Copyright 2014 oldtopman, oldtopman <at> gmail <dot> com

Project License: zlib
*******************************************************************************
**/

#include <Arduino.h>
#include <SPI.h>
#include "NokiaDisplay.h"

#define COMMAND_SIGNAL  0 
#define DATA_SIGNAL     1
#define LCD_WIDTH   84
#define LCD_HEIGHT  48

//Character map declared into PROGMEM, not RAM.
PROGMEM const byte nokiaDisplay_charMap[96][5] = {
   {0x00, 0x00, 0x00, 0x00, 0x00} // 0x20  
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 0x21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 0x22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 0x23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 0x24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 0x25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 0x26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 0x27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 0x28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 0x29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 0x2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 0x2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 0x2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 0x2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 0x2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 0x2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 0x30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 0x31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 0x32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 0x33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 0x34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 0x35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 0x36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 0x37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 0x38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 0x39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 0x3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 0x3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 0x3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 0x3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 0x3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 0x3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 0x40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 0x41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 0x42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 0x43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 0x44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 0x45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 0x46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 0x47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 0x48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 0x49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 0x4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 0x4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 0x4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 0x4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 0x4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 0x4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 0x50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 0x51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 0x52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 0x53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 0x54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 0x55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 0x56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 0x57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 0x58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 0x59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 0x5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 0x5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 0x5c <backslash>
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 0x5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 0x5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 0x5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 0x60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 0x61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 0x62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 0x63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 0x64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 0x65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 0x66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 0x67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 0x68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 0x69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 0x6a j 
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 0x6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 0x6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 0x6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 0x6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 0x6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 0x70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 0x71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 0x72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 0x73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 0x74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 0x75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 0x76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 0x77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 0x78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 0x79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 0x7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 0x7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 0x7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 0x7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 0x7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 0x7f DEL
};

NokiaDisplay::NokiaDisplay(){
  cursorX = 0;
  cursorY = 0;
  scePin = 0;
  rstPin = 0;
  dcPin = 0;
  sdinPin = 0;
  sclkPin = 0;
  blPin = 0;
  contrast = 55; //Seems to work on most 3310 displays
  brightness = 255; //Bright as possible, amirite?
  
  //Clear framebuffer
  memset(frameBuffer, 0, LCD_HEIGHT*LCD_WIDTH/8);
}

bool NokiaDisplay::getPixel(int p_offset){
  return bitRead(frameBuffer[(int)(p_offset/8)], (p_offset%8));
}

bool NokiaDisplay::getPixel(byte p_x, byte p_y){
  //Calculate offset, use the offset function instead.
  return NokiaDisplay::getPixel( ((p_y*LCD_WIDTH) + p_x));
}

void NokiaDisplay::setPixel(int p_offset, bool p_bw){
  //If you write beyond the length of the display, then things go badly.
  //Of course, if you check here, you slow things down a lot.
  //Don't screw up.
  //if(p_offset > LCD_WIDTH*LCD_HEIGHT){ return; }
  bitWrite(frameBuffer[(int)(p_offset/8)], (p_offset%8), p_bw);
}

void NokiaDisplay::setPixel(byte p_x, byte p_y, bool p_bw){  
  //Calculate offset, use the offset function instead.
  NokiaDisplay::setPixel( ((p_y*LCD_WIDTH) + p_x), p_bw);
}


void NokiaDisplay::gotoPixel(byte p_x, byte p_y){
  
  //Move by LCD commands.
  LCDWrite(0, 0x80 | p_x);
  LCDWrite(0, 0x40 | p_y);
  
  //Record position.
  cursorX = p_x;
  cursorY = p_y;
}


void NokiaDisplay::LCDWrite(byte p_data_or_command, byte data){
  //Pick data or command.
  digitalWrite(dcPin, p_data_or_command);

  //Send data.
  digitalWrite(scePin, LOW);
  SPI.transfer(data);
  digitalWrite(scePin, HIGH);
}


bool NokiaDisplay::init(){

  //Check if values were initialized, then configure pins.
  if(scePin == 0){ return true; } else{ pinMode(scePin, OUTPUT); }
  if(rstPin == 0){ return true; } else{ pinMode(rstPin, OUTPUT); }
  if(dcPin == 0){ return true; } else{ pinMode(dcPin, OUTPUT); }
  if(sdinPin == 0){ return true; } else if(sdinPin > 0){ pinMode(sdinPin, OUTPUT); }
  if(sclkPin == 0){ return true; } else if(sclkPin > 0){ pinMode(sclkPin, OUTPUT); }
  if(blPin == 0){ return true; } else{ pinMode(blPin, OUTPUT); }

  //Configure SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  //Reset LCD
  digitalWrite(rstPin, LOW);
  digitalWrite(rstPin, HIGH);
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0x21); 
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0xB0);
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0x04);
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0x14);
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0x20); 
  NokiaDisplay::LCDWrite(COMMAND_SIGNAL, 0x0C);
  
  //Set contrast
  NokiaDisplay::updateContrast();
  
  //Set brightness
  NokiaDisplay::updateBrightness();
  
  //Make sure cursor is put away properly.
  NokiaDisplay::gotoPixel(0,0);
  return false;
}


void NokiaDisplay::updateContrast(){
  LCDWrite(COMMAND_SIGNAL, 0x21); 
  LCDWrite(COMMAND_SIGNAL, 0x80 | contrast);
  LCDWrite(COMMAND_SIGNAL, 0x20);
}


void NokiaDisplay::updateBrightness(){
  analogWrite(blPin, brightness);
}


void NokiaDisplay::update(){
  
  //Initialize variables.
  byte counterWidth = 0;
  byte counterHeight = 0;
  byte bitCounter = 0;
  byte lcdByte = 0;
  
  //Address the LCD at 0,0.
  //Don't use gotoPixel so that NokiaDisplay::print still works properly.
  LCDWrite(0, 0x80 | 0);
  LCDWrite(0, 0x40 | 0);
  
  while(counterHeight < LCD_HEIGHT/8){
    //For every column-height....
    counterWidth = 0;
    while(counterWidth < LCD_WIDTH){
      //For every pixel across the screen...
      bitCounter = 0;
      while(bitCounter < 8){
        //For every pixel in a column...
        lcdByte = lcdByte >> 1;
        
        //Pixel retrieving functions:
        
        //This one is easily read, but costs a ton of processing time to execute.
        //Left here for readability/reference purposes.
        //lcdByte = lcdByte | (getPixel(counterWidth, (counterHeight*8)+bitCounter) << 7);
        
        //This is the same as above, but runs in about 20x faster. (tested)
        lcdByte = lcdByte | (bitRead(frameBuffer[(int)((counterWidth+ ((counterHeight*8)+bitCounter)*LCD_WIDTH)/8)], ((counterWidth+ ((counterHeight*8)+bitCounter)*LCD_WIDTH)%8)) << 7);
        
        bitCounter++;
      }
      LCDWrite(DATA_SIGNAL, lcdByte); 
      counterWidth++;
    }
    counterHeight++;
  }
  digitalWrite(scePin, LOW);
}


void NokiaDisplay::clear(bool p_bw){
  
  //Write zeroes/ones to the memory where the framebuffer lies.
  
  if(p_bw == true){
    memset(frameBuffer, 0b11111111, LCD_HEIGHT*LCD_WIDTH/8);
  }
  else{
    memset(frameBuffer, 0b00000000, LCD_HEIGHT*LCD_WIDTH/8);
  }
}


void NokiaDisplay::mvPrint(const char * p_text, byte p_x, byte p_y, bool p_bw){
  
  char tempChar;

  tempChar = *p_text++;
  mvPrintChar(tempChar, p_x, p_y, p_bw);
  
  while (*p_text != 0x00){ // loop until null terminator

    tempChar = *p_text++;
    printChar(tempChar, p_bw);
  }
}


void NokiaDisplay::print(const char * p_text, bool p_bw){
  NokiaDisplay::mvPrint(p_text, cursorX, cursorY, p_bw);
}


void NokiaDisplay::mvPrint_P(const char * p_text, byte p_x, byte p_y, bool p_bw){
  
  char tempChar;

  tempChar = pgm_read_byte(p_text++);
  mvPrintChar(tempChar, p_x, p_y, p_bw);
  
  while (pgm_read_byte(p_text) != 0x00){ // loop until null terminator

    tempChar = pgm_read_byte(p_text++);
    printChar(tempChar, p_bw);
  }
}


void NokiaDisplay::print_P(const char * p_text, bool p_bw){
  NokiaDisplay::mvPrint_P(p_text, cursorX, cursorY, p_bw);
}


void NokiaDisplay::printChar(char p_char, bool p_bw){
  NokiaDisplay::mvPrintChar(p_char, cursorX, cursorY, p_bw);
}


void NokiaDisplay::mvPrintChar(char p_char, byte p_x, byte p_y, bool p_bw){
  
  byte column; //Holds bitmap for a column of a character.
  
  for (int i=0; i<5; i++) // 5 columns (x) per character
  {
    column = pgm_read_byte_near(&nokiaDisplay_charMap[p_char - 0x20][i]);
    for (int j=0; j<8; j++) // 8 rows (y) per character
    {
      if (column & (0x01 << j)) // test bits to set pixels
        NokiaDisplay::setPixel(p_x+i, p_y+j, p_bw);
      else
        NokiaDisplay::setPixel(p_x+i, p_y+j, !p_bw);
    }
  }
  
  p_x=p_x+5; //Next character.
  
  for (int i=p_y; i<p_y+8; i++){ //Print white row between characters.
    setPixel(p_x, i, !p_bw);
  }
  
  p_x++; //Space between characters.
  if (p_x > (LCD_WIDTH - 5)){ // Enables wrap around
    p_x = 0;
    p_y += 8;
  }
  
  //Record current character position.
  cursorX = p_x;
  cursorY = p_y;
}


void NokiaDisplay::invertFramebuffer(){
  for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++){
    frameBuffer[i] = ~frameBuffer[i] & 0xFF;
  }
}


void NokiaDisplay::invertDisplay(){
  LCDWrite(DATA_SIGNAL, 0x20); //Tell LCD that extended commands follow
  LCDWrite(DATA_SIGNAL, 0x08 | contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(DATA_SIGNAL, 0x20); //Set display mode*/
}


void NokiaDisplay::drawLine(int x0, int y0, int x1, int y1, boolean bw){
  //This *entire* function was directly copied from SF's library.
  int dy = y1 - y0; // Difference between y0 and y1
  int dx = x1 - x0; // Difference between x0 and x1
  int stepx, stepy;

  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else
    stepy = 1;

  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
    stepx = 1;

  dy <<= 1; // dy is now 2*dy
  dx <<= 1; // dx is now 2*dx
  setPixel(x0, y0, bw); // Draw the first pixel.

  if (dx > dy) 
  {
    int fraction = dy - (dx >> 1);
    while (x0 != x1)
    {
      if (fraction >= 0)
      {
        y0 += stepy;
        fraction -= dx;
      }
      x0 += stepx;
      fraction += dy;
      setPixel(x0, y0, bw);
    }
  }
  else
  {
    int fraction = dx - (dy >> 1);
    while (y0 != y1)
    {
      if (fraction >= 0)
      {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      setPixel(x0, y0, bw);
    }
  }
}


void NokiaDisplay::drawRect(int x0, int y0, int x1, int y1, boolean fill, boolean bw){
  //This *entire* function was directly copied from SF's library.
  // check if the rectangle is to be filled
  if (fill == 1)
  {
    int xDiff;

    if(x0 > x1)
      xDiff = x0 - x1; //Find the difference between the x vars
    else
      xDiff = x1 - x0;

    while(xDiff > 0)
    {
      NokiaDisplay::drawLine(x0, y0, x0, y1, bw);

      if(x0 > x1)
        x0--;
      else
        x0++;

      xDiff--;
    }
  }
  else 
  {
    // best way to draw an unfilled rectangle is to draw four lines
    NokiaDisplay::drawLine(x0, y0, x1, y0, bw);
    NokiaDisplay::drawLine(x0, y1, x1, y1, bw);
    NokiaDisplay::drawLine(x0, y0, x0, y1, bw);
    NokiaDisplay::drawLine(x1, y0, x1, y1, bw);
  }
}


void NokiaDisplay::drawCircle(int x0, int y0, int radius, boolean bw, int lineThickness){
  //You'll never guess - this one's from sparkfun as well.
  for(int r = 0; r < lineThickness; r++)
  {
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    NokiaDisplay::setPixel(x0, y0 + radius, bw);
    NokiaDisplay::setPixel(x0, y0 - radius, bw);
    NokiaDisplay::setPixel(x0 + radius, y0, bw);
    NokiaDisplay::setPixel(x0 - radius, y0, bw);

    while(x < y)
    {
      if(f >= 0)
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x + 1;

      NokiaDisplay::setPixel(x0 + x, y0 + y, bw);
      NokiaDisplay::setPixel(x0 - x, y0 + y, bw);
      NokiaDisplay::setPixel(x0 + x, y0 - y, bw);
      NokiaDisplay::setPixel(x0 - x, y0 - y, bw);
      NokiaDisplay::setPixel(x0 + y, y0 + x, bw);
      NokiaDisplay::setPixel(x0 - y, y0 + x, bw);
      NokiaDisplay::setPixel(x0 + y, y0 - x, bw);
      NokiaDisplay::setPixel(x0 - y, y0 - x, bw);
    }
    radius--;
  }
}


void NokiaDisplay::xbmBlitImage_P(const byte p_image[], byte p_x, byte p_y, byte p_width, byte p_height, bool p_invert){
  
  //=====
    // Interesting note:
    //This function could be greatly optimized by simply replacing pieces of the framebuffer
    //with sections from the xbm image, since they have the same layout.
    //Unfortunately, this is incredibly complex when not dealing with images that start on multiples of eight.
    //The performance gains are marginal, so this is unlikely to be accomplished in the near future.
    
    //Furthermore, an implementation like that would require several variables for bitmasking,
    //several buffers, etc.
  //=====
  
  //Set counting variables.
  byte widthCounter = 0; //Pixels
  byte heightCounter = 0; //Pixels
  byte wastedBits = 8-(p_width%8); //Bits
  bool pixelBit = false; //Pixel
  
  while(heightCounter < p_height){
    while(widthCounter < p_width){
      
      pixelBit = bitRead(pgm_read_byte_near(p_image + ((heightCounter*(p_width+wastedBits)) + widthCounter)/8), widthCounter%8);
      
      //Invert pixel if required.
      pixelBit = pixelBit^p_invert;
      
      //Write it out.
      NokiaDisplay::setPixel(p_x+widthCounter,p_y+heightCounter, pixelBit);
      
      widthCounter++;
    }
    widthCounter = 0;
    heightCounter++;
  }
}


void NokiaDisplay::ardBlitImage_P(const byte p_image[], byte p_x, byte p_y, byte p_width, byte p_height, bool p_invert){
  
  //Set counting variables.
  byte widthCounter = 0; //This is in pixels.
  byte heightCounter = 0; //This is in pixels due to the fact that ard images may have waste-sizes.
  byte bitCounter = 0; //This is a per-pixel height counter, loops every eight.
  byte remainingBits = 8; //This tells us how many bits we have left in a column that doesn't end nicely.
  bool pixelBit = false; //The pixel we read.
  
  while(heightCounter < p_height){
    
    //Every 8 pixels tall... (every ardByte column height...)
    //set RemainingBits.
    remainingBits = p_height - heightCounter;
    if (remainingBits > 8){ remainingBits = 8; }
    while(bitCounter < remainingBits){
      
      //For every vertical bit in the ard array (height)
      while(widthCounter < p_width){
        
        //For every pixel width...
        pixelBit = bitRead(pgm_read_byte_near(p_image + ((heightCounter/8)*LCD_WIDTH+widthCounter)), bitCounter);
        
        //For non-progmem pictures.
        //pixelBit = bitRead(p_image[(heightCounter/8)*LCD_WIDTH+widthCounter], bitCounter);
        
        //Manually invert the pixel if asked to.
        pixelBit = pixelBit^p_invert;
        
        //Write to the array.
        NokiaDisplay::setPixel(p_x+widthCounter, p_y+bitCounter+heightCounter, pixelBit);
        widthCounter++;
      }
      widthCounter = 0; //Finished a row.
      bitCounter++;
    }
    bitCounter = 0; //Finished a byte-column (8 rows.)
    heightCounter = heightCounter + 8; //Every eight bits we'd end up here.
  }
}

