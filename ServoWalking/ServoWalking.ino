#include <Servo.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int leftPin = 6; // Analog output pin that the left Servo is attached to
const int rightPin = 5; // Analog output pin that the right Servo is attached to
const int middlePin = 10; // Analog output pin that the middle Servo is attached to
const int t = 700;
const int leftEye = A0;
const int rightEye = A1;

void walkfoward();
void turnright();
void turnleft();

Servo left, right, middle;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  // init pins
  left.attach(leftPin);
  right.attach(rightPin);
  middle.attach(middlePin);
  
  // write initial values
  left.write(90);
  right.write(90);
  middle.write(90);
}

void loop() {
  int senval1 = analogRead(leftEye);
  int senval2 = analogRead(rightEye);
  
  int val1 = map(senval1, 52, 509, 0, 100);
  int val2 = map(senval2, 41, 536, 0, 100);
 
  // left equal to right, go straight
  if(val1 == val2)
    walkforward();
  // left is greater than right, go right
  else if (val1 > val2)
    turnright();
  // left is less than right, go left
  else if (val1 < val2)
    turnleft();
  else
    walkbackward();
      
  // print the results to the serial monitor:
  //walkforward();

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);                     
}

void walkforward()
{
  //frame0();
  //right front & back, left middle legs touching the ground
  middle.write(80);
  delay(t);
  
  //frame1();
  //down legs move back, front legs move forward 
  right.write(80);
  left.write(80);
  delay(t);
  
  //frame2();
  //weight shifted to the other tripod
  middle.write(110);
  delay(t);
  
  //frame3();
  //down legs move back, front legs move forward
  right.write(110);
  left.write(110);
  delay(t);
}

void walkbackward()
{
  //frame0();
  //right front & back, left middle legs touching the ground
  middle.write(80);
  delay(t);
  
  //frame3();
  //down legs move back, front legs move forward
  right.write(110);
  left.write(110);
  delay(t);
  
  //frame2();
  //weight shifted to the other tripod
  middle.write(110);
  delay(t);
  
  //frame1();
  //down legs move back, front legs move forward 
  right.write(80);
  left.write(80);
  delay(t);
}

void turnright()
{
  //frame0();
  //right front & back, left middle legs touching the ground
  middle.write(80);
  delay(t);
  
  //frame1();
  //down legs move back, front legs move forward 
  right.write(110);
  left.write(80);
  delay(t);
  
  //frame2();
  //weight shifted to the other tripod
  middle.write(110);
  delay(t);
  
  //frame3();
  //down legs move back, front legs move forward
  right.write(80);
  left.write(110);
  delay(t);
}

void turnleft()
{
  //frame0();
  //right front & back, left middle legs touching the ground
  middle.write(80);
  delay(t);
  
  //frame1();
  //down legs move back, front legs move forward 
  right.write(80);
  left.write(110);
  delay(t);
  
  //frame2();
  //weight shifted to the other tripod
  middle.write(110);
  delay(t);
  
  //frame3();
  //down legs move back, front legs move forward
  right.write(110);
  left.write(80);
  delay(t);
}
