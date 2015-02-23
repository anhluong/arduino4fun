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

Steppers (x2) 50, 51, 52, 53
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
// Step is 31
// Dir is 30
StepperDS step1(STEPS, 31, 30);
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

void setup()
{
  Serial.begin(9600);
 
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
  
    //Get input from sonic range finder
    tripoint tri;
    bool newVals = false;
    tri.x = sonic1.ping();
    tri.y = sonic2.ping();
    tri.z = sonic3.ping();
    
      Serial.print(old1);
      Serial.print("in, ");
      Serial.println();
    
    //Check for good values (not too far, not too close)
    if (tri.x < sMAX && tri.x > sMIN && tri.x != old1) {
      old1 = tri.x; //Store value
      olda1 = sonic1.theta;
      newVals = true;
    }
    
    /*
    if (tri.y < sMAX && tri.y > sMIN){
      old2 = tri.y;
      olda2 = sonic2.theta;
      newVals = true;
    }
    if (tri.z < sMAX && tri.y > sMIN){
      old3 = tri.z;
      olda3 = sonic3.theta;
      newVals = true;
    }
    */
      
  //Move all servos with same signal pin
    sonic1.move(5);
  //  sonic2.move(5);
  //  sonic3.move(5);
    
  
    //Calcuations
    

    if (newVals){
      
      //2D triangulation
      target = tri.d2tri(old1, old2, old3, olda1, olda2, olda3, LENGTH, WIDTH);
   
   
      //Print values to serial monitor
   
      /*
      Serial.print(old2);
      Serial.print("in, ");
      Serial.print(old3);
      Serial.print("in");
      Serial.print(" - ");
      */
      Serial.print("Target: (");
      Serial.print(target.x);
      Serial.print(", ");
      Serial.print(target.y);
      Serial.print(")");
      Serial.println();
      
      
      //Provide output to devices
      //Move Pan and Tilt servos to target
      tri.d3tri(block1, target, HEIGHT); //Triangulate in 3d, write phi and theta angles to servos.
      tri.d3tri(block2, target, HEIGHT);
      tri.d3tri(block3, target, HEIGHT);
      
    }
  /* END OF SONIC STUFF */
  
  
  //LED BANKS
//black blue green cyan red magenta yellow white

//Break area into 4 quadrents by dividing height and width by two.
//Change colors depending on which quadrent target is in.
  
  if (target.x > LENGTH / 2)
  {
     if (target.y > WIDTH / 2) //RED
     {
        light1.setColor(red);
        light2.setColor(red);
        light3.setColor(red);
     }
     else
     {
        light1.setColor(green); //GREEN
        light2.setColor(green);
        light3.setColor(green);
     }
  }
  else
  {
    if (target.y > WIDTH /2)
    {
        light1.setColor(blue); //BLUE
        light2.setColor(blue);
        light3.setColor(blue);
    }
    else
    {
        light1.setColor(cyan); //OTHER COLORS
        light2.setColor(yellow);
        light3.setColor(magenta);
    }
  }
  block1.dance(2);
  block2.dance(2);
  block3.dance(2);
  delay(100);
}
