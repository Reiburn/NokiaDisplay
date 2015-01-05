/**
*******************************************************************************
NokiaDisplay.h

NokiaDisplay, Copyright 2014 oldtopman, oldtopman <at> gmail <dot> com

Project License: zlib
*******************************************************************************
**/
#include <Arduino.h>

#ifndef NOKIA_DISPLAY
#define NOKIA_DISPLAY
#define LCD_WIDTH   84
#define LCD_HEIGHT  48

class NokiaDisplay{

  private:
    /*
     * Now, a short sermon on "Best Practices", and why this section is empty.
     * 
     * Normally you would keep everything possible private, carefully
     * separating the pieces of your program so that everything is well siloed
     * and compartmentalized. On a device like an Arduino, it will only run on
     * a few select devices, is unlikely to be hacked from the "environment",
     * and most importantly, NEEDS TO BE KEPT SMALL.
     * 
     * Get/set functions are cool, but display code takes up too much space
     * without the library stealing your precious overhead.
     * 
     * tl;dr
     * You know what you're doing, so keeping this empty gives you power.
     * 
     */

  public:
    //Constructor
    NokiaDisplay();
    //frameBuffer functions.
    bool getPixel(int p_offset);
    bool getPixel(byte p_x, byte p_y);
    void setPixel(int p_offset, bool p_bw);
    void setPixel(byte p_x, byte p_y, bool p_bw);
    //Functions
    void LCDWrite(byte p_data_or_command, byte data);
    void gotoPixel(byte p_x, byte p_y);
    bool init();
    void updateContrast();
    void updateBrightness();
    void update();
    void clear(bool p_bw);
    void mvPrint(const char * p_text, byte p_x, byte p_y, bool p_bw);
    void print(const char * p_text, bool p_bw);
    void mvPrint_P(const char * p_text, byte p_x, byte p_y, bool p_bw);
    void print_P(const char * p_text, bool p_bw);
    void printChar(char p_char, bool p_bw);
    void mvPrintChar(char p_char, byte p_x, byte p_y, bool p_bw);
    void invertFramebuffer();
    void invertDisplay();
    void drawLine(int x0, int y0, int x1, int y1, boolean bw);
    void drawRect(int x0, int y0, int x1, int y1, boolean fill, boolean bw);
    void drawCircle(int x0, int y0, int radius, boolean bw, int lineThickness);
    void xbmBlitImage_P(const byte p_image[], byte p_x, byte p_y, byte p_width, byte p_height, bool p_invert);
    void ardBlitImage_P(const byte p_image[], byte p_x, byte p_y, byte p_width, byte p_height, bool p_invert);
    
    
    //The framebuffer - 1 variable per pixel, with one bit for every state of pixel.
    //This is addressed as an array of bools, see the *Pixel functions for how that operates.
    byte frameBuffer[LCD_WIDTH*LCD_HEIGHT/8];
    
    //The font (charMap) used to be declared here, but was moved to
    //a declaration in NokiaDisplay.cpp so PROGMEM could be used.
    
    //Current "cursor" position
    byte cursorX;
    byte cursorY;
    
    //Pin Variables.
    byte scePin;
    byte rstPin;
    byte dcPin;
    byte sdinPin; //Set to negative number if on 32u2
    byte sclkPin; //Set to negative number if on 32u2
    byte blPin;
    byte contrast;
    byte brightness;
};

#endif
