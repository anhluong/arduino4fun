#ifndef LedControl_h
#define LedControl_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// R G B
#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define magenta 5
#define yellow 6
#define white 7

class RGBLed {
 private :
    int rpin;
    int gpin;
    int bpin;
    
 public:
    RGBLed(int rp, int gp, int bp);
    
    // color: 0 - 8
    void setColor(int r, int g, int b);
    void setColor(int c);
};

#endif	//LedControl.h



