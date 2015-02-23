#include <StepperDS.h>
#include <Servo.h>

#define STEPS 200
#define DIR_PIN_1 8
#define STEP_PIN_1 9
#define DIR_PIN_2 10
#define STEP_PIN_2 11
#define DIR_PIN_3 12
#define STEP_PIN_3 13
#define SERVO1_PIN 4
#define SERVO2_PIN 5

StepperDS motor1(STEPS, DIR_PIN_1, STEP_PIN_1);
StepperDS motor2(STEPS, DIR_PIN_2, STEP_PIN_2);
StepperDS motor3(STEPS, DIR_PIN_3, STEP_PIN_3);
Servo servo1, servo2;

int previous = 0;
int count = 0;
long steps, forb, rorl, rrl, srl;

void setup()
{
  Serial.begin(9600);
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);
}
/*
void loop()
{
  randomSeed(analogRead(A0));
  rrl = random(0, 2);
  srl = random(0, 2);
  rorl = random(0, 2);
  
  if(rorl)
    servo.write(180);
  else
    servo.write(0);
  delay(100);
    
  // Saw stepper
  if(srl)
  {
      motor1.step(10);//forb
  }
  else
  {
      motor1.step(-10);
  }
  
  // Turn table
  if(rrl)
    motor2.step(27);
  else
    motor2.step(-27);
    
  servo.write(90);
  delay(50);
}
*/

void loop()
{
  //motor1.step(1000);
  //motor2.step(1000);
  motor3.step(1000);
}
