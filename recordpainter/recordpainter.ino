/* 
This program drives two old style record player things.
The first record player just spins slowly (DC motor) and has a light 
sensor (and servo) reading any marks on it and sending the values to the other
record player. The second record player paints on the record that 
is also slowly turning (step/dc), while also tracking in and out (stepper)
and sweeping  (servo)

There may also be four buttons. Rewind, Stop, Play, Fastforward.
*/

#include <Servo.h>
#include <StepperDS.h>
#include <Arduino.h>
#include <math.h>

/*** PIN USAGE - Arduino Uno ***
Rewind                   - 0
Stop                     - 1
Play                     - 2
Fastforward              - 3


Paint Disc Stepper       - 7, 8
Light Disk Stepper       - 7, 8
Light Sensor (PMW)       - 9
Light Senser Servo (PMW) - 10
Painting Servo (PMW)     - 11
Painting Stepper         - 12, 13
DC Motors                - N/A
*/
//Pin definitions
#define REWIND 0
#define STOP 1
#define PLAY 2
#define FASTFORWARD 3
#define DISKSTEPPER1 10
#define DISKSTEPPER2 11
#define LIGHTSENSOR A0
#define LIGHTSERVO 5
#define PAINTSERVO 6
#define PAINTSTEPPER1 12
#define PAINTSTEPPER2 13

//Stepper definitions
#define STEPS 200
#define SPEED 10

//Number of steps for one disk rotation
#define DISKROTATION 50

//Max steps for paint stepper
#define PSTEPMAX 35*8
//Min steps for paint stepper
#define PSTEPMIN 0*8

//Max angle for paint servo
#define PSERVOMAX 40
//Min angle for paint servo
#define PSERVOMIN 0
//Max angle for light servo
#define LSERVOMAX 40
//Min angle for light servo
#define LSERVOMIN 0

//Step interval for light servo
#define LSERVOSTEP 1
//Step interval for paint servo
#define PSERVOSTEP 3
//Step interval for paint stepper
#define PAINTSTEP 8 //Not used

//Default play speed
#define PLAYSPEED 1000

//Max and Min light sensor values for calibration
#define LIGHTMAX 400
#define LIGHTMIN 30



//Variable definitions

int lightValue = 0;  // value from light sensor
int lsAngle = 0;     // light servo angle
int psAngle = 0;     // paint servo angle
int stPos = 0;       // paint stepper position
int playSpeed = PLAYSPEED;   // Overall speed

bool Play = false;   //Stop = false, Play = true
bool FF = false;
bool RR = false;

bool lightDir = false; //Keep track of which direction the servo is moving
bool paintDir = false;
int rotation = 0; //Keep track of complete rotations

int lastLightValue = 0;

//Object definitions
Servo ls;            // light servo
Servo ps;            // paint servo
StepperDS pStep(STEPS, PAINTSTEPPER1, PAINTSTEPPER2); //Painting stepper
StepperDS dStep(STEPS, DISKSTEPPER1, DISKSTEPPER2); //Disc steppers (Both are wired to the same pins)

void sweepps(Servo &serv, int dist)
{
  if(paintDir)
  {
    if ((psAngle + dist) >= PSERVOMAX)
    {
      paintDir = false;
      psAngle = PSERVOMAX;
    }
    else
    {
      psAngle += dist;
    }
  }
  else
  {
    if ((psAngle - dist) <= PSERVOMIN)
    {
      paintDir = true;
      psAngle = PSERVOMIN;
    }
    else
    {
      psAngle -= dist;
    }
  }
  serv.write(psAngle);
}
void sweepls(Servo &serv, int dist)
{
    if ((lsAngle + dist) >= LSERVOMAX)
    {
      lsAngle = 0;
    }
    else
    {
      lsAngle += dist;
    }
  serv.write(lsAngle);
}
void move(StepperDS &stDS, int val)
{
  if (val >= PSTEPMAX){ //Move to stepper max
    stDS.step(PSTEPMAX - stPos);  
    stPos = PSTEPMAX;
  }
  if (val <= PSTEPMIN){ //Move to stepper min
    stDS.step(stPos - PSTEPMIN);
    stPos = PSTEPMIN;
  }
  if (val < PSTEPMAX && val > PSTEPMIN){
    stDS.step(val - stPos);
    stPos = val;
  }
  
}
void setup(){
  Serial.begin(9600);
  ls.attach(LIGHTSERVO);
  ps.attach(PAINTSERVO);
  pStep.setSpeed(SPEED);
  dStep.setMicroStepping(2);
  Play = true; //Remove this 
}
void loop(){
  //Check buttons.
  //Set Play boolean accordingly.
  
  //While playing,
  while(Play)
  {
 //Reset variables
  playSpeed = PLAYSPEED;
  
 //Get input from light sensor
 lightValue = analogRead(A5);
 Serial.println(lightValue);
 lightValue = map(lightValue, LIGHTMIN, LIGHTMAX, PSTEPMIN, PSTEPMAX); //Map the light value accordingly

Serial.println("Mapped Value" + lightValue);
 //Do basic movements based on actions 
 
 if (rotation >= DISKROTATION){ //Every rotation of disk, move light servo in
   Serial.println("Moving servos");
   sweepls(ls, LSERVOSTEP); //Move light servo
   rotation = 0;
 }
   
   sweepps(ps, PSERVOSTEP); //Move paint servo
   move(pStep, lightValue); //Move paint stepper
   dStep.step(playSpeed); //Move disk forward
   rotation += playSpeed;
   Serial.println(rotation);
  }
  //delay(10);
}
