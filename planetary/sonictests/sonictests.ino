#include <Servo.h>
#include <StepperDS.h>
#include <Arduino.h>
#include <planetObjects.h>
#include <math.h>
#include <RGBLed.h>


//Pins
//Sonics (x3) 22, 23, 24

//Sonic Rangefinder definitions
#define sMAX  90   //max distance we pay attention to. 96in is 8 ft
#define sMIN  5

//Structure Dimensions
#define HEIGHT  120  //height in inches
#define WIDTH  96 //y direction
#define LENGTH  72 //y direction

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

//Sonic Rangefinder servos
Servo son1;
Servo son2;
Servo son3;

sonic sonic1;
sonic sonic2;
sonic sonic3;

void setup(){
 Serial.begin(9800);
 
  son1.attach(2);
  son2.attach(3);
  son3.attach(4);
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
}

void loop(){
     //Get input from sonic range finder
    tripoint tri;
    bool newVals = false;
    tri.x = sonic1.ping();
    //tri.y = sonic2.ping();
    //tri.z = sonic3.ping(); 
    
    /*
    //Check for good values (not too far, not too close, not same value)
    if (tri.x < sMAX && tri.x > sMIN && tri.x != old1) {
      old1 = tri.x; //Store value
      olda1 = sonic1.theta;
      newVals = true;
    }
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
    
      //Print values to serial monitor
      
      Serial.print(tri.x);
      Serial.print("in, ");
      
      Serial.println();
      
      delay(1000);
}
