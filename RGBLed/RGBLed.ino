#include <RGBLed.h>

#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define magenta 5
#define yellow 6
#define white 7

RGBLed light1(40, 41, 42);
RGBLed light2(43, 44, 45);
RGBLed light3(46, 47, 48);

void setup()
{
  
}

int count1 = 0;
int count2 = 0;
int count3 = 0;

void loop()
{
  light1.setColor(red);
  light2.setColor(green);
  light3.setColor(blue);
}  
