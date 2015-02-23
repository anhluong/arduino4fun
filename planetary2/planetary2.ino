#include <Servo.h>
#include <StepperDS.h>
#include <Arduino.h>
#include <planetObjects.h>
#include <math.h>
#include <RGBLed.h>

/* PIN USEAGE
Pins
Analog A0 - A15
Digital 22 - 53
PMW 2 - 13
Other 1, 2, 14 - 21

Steppers (x2) 30, 31
Sonic Servos (x3) 2 (all three perform same actions)
LED Servos (x6) 5, 6, 7, 8, 9, 10
LEDs (x3) 11, 12, 13
Sonics (x3) 22, 23, 24
LED Banks 40-48

*/

//Stepper definitions
#define STEPS  200

//Sonic Rangefinder definitions
#define sMAX  100   //max distance we pay attention to
#define sMIN  5

//Structure Dimensions
#define HEIGHT  120  //height in inches
#define WIDTH  96 //y direction
#define LENGTH  72 //y direction

//Pan and Tilt locations (distance from lower left corner in inches)
#define PAN1X  56.85
#define PAN1Y  19.47
#define PAN2X  12.83
#define PAN2Y  48.49
#define PAN3X  55.77
#define PAN3Y  75.31

//LED Color Definitions
#define black 0
#define blue 1
#define green 2
#define cyan 3
#define red 4
#define magenta 5
#define yellow 6
#define white 7

//LED Light Banks
RGBLed light1(40, 41, 42);
RGBLed light2(43, 44, 45);
RGBLed light3(46, 47, 48);

//Pan and Tilt RGB LED bank
Servo block1p; //bank 1, pan
Servo block1t; //bank 1, tilt
Servo block2p; //etc
Servo block2t;
Servo block3p;
Servo block3t;

//Sonic Rangefinder servos
Servo son1;
Servo son2;
Servo son3;

//Planetary Gear Steppers
StepperDS step1(STEPS, 30, 31);
//StepperDS step2(STEPS, 53, 52);

//Variables
//input values from rangefinders
int val1; //New values
int val2; 
int val3;
int old1; //Old values
int old2;
int old3;
int olda1; //Old angles
int olda2;
int olda3;

//Objects
point target;
ptServo block1;
ptServo block2;
ptServo block3;
sonic sonic1;
sonic sonic2;
sonic sonic3;

//Timer for steppers vs sonics
int timing;

int colorPicker(point thePoint){
if (thePoint.x > LENGTH / 2)
  {
     if (thePoint.y > WIDTH / 2) //RED
     {
        return red;
     }
     else
     {
        return green;
     }
  }
  else
  {
    if (thePoint.y > WIDTH /2)
    {
        return blue;
    }
    else
    {
      int junk = random(1, 10);
      if (junk < 4)
        return cyan;
      if (junk > 7)
        return yellow;
      return magenta;
    }
  }
}

void setup()
{
  Serial.begin(9600);
 
   randomSeed(analogRead(0));
 
  //Attach Pan and Tilt servos
  block1p.attach(5);
  block1t.attach(6);
  block2p.attach(7);
  block2t.attach(8);
  block3p.attach(9);
  block3t.attach(10);
  
  son1.attach(2);
  son2.attach(3);
  son3.attach(4);
  
  //Set planetary stepper speed RPMs
  step1.setSpeed(100);
  //step2.setSpeed(100);
  
  //Initialize objects
  target = point();
  block1 = ptServo(block1p, block1t, 0, point(PAN1X, PAN1Y));
  block2 = ptServo(block2p, block2t, 0, point(PAN2X, PAN2Y));
  block3 = ptServo(block3p, block3t, 0, point(PAN3X, PAN3Y));
  sonic1 = sonic(son1, 22);
  sonic2 = sonic(son2, 23);
  sonic3 = sonic(son3, 24);
 
  //Initialize variables
  val1 = 0;
  val2 = 0;
  val3 = 0;
  old1 = 0;
  old2 = 0;
  old3 = 0;
  olda1 = 0;
  olda2 = 0;
  olda3 = 0;
  timing = 0;
  
}

void loop()
{ 
      //digitalWrite(31, HIGH);
      //digitalWrite(30, HIGH);
      //delayMicroseconds(1000);
      //digitalWrite(30, LOW);
      //delayMicroseconds(1000);
      
      step1.step(10);
      delay(100);
  
    //LED servo movement
      
      tripoint tri;
      int newval = random(5,50);
      if (newval > 30)
      {
        target.x = random(5,50); //sonic1.ping();
        target.y = random(50,170); //sonic2.ping();
        block1.pan.write((int) target.x);
        block1.tilt.write((int) target.y);
        light1.setColor(colorPicker(target));
      }
      newval = random(5,50);
      if (newval > 30)
      {
        target.x = random(5,50); //sonic1.ping();
        target.y = random(50,170); //sonic2.ping();
        block2.pan.write((int) target.x);
        block2.tilt.write((int) target.y);
        light2.setColor(colorPicker(target));
      }
      newval = random(5,50);
      if (newval > 30)
      {
        target.x = random(5,50); //sonic1.ping();
        target.y = random(50,170); //sonic2.ping();
        block3.pan.write((int) target.x);
        block3.tilt.write((int) target.y);
        light3.setColor(colorPicker(target));
      }
      
  /* END OF SONIC STUFF */
  
  
  //LED BANKS
//black blue green cyan red magenta yellow white

//Break area into 4 quadrents by dividing height and width by two.
//Change colors depending on which quadrent target is in.
  delay(1000);
}
