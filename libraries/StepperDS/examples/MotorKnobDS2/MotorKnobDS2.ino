/*
 * MotorKnobDS2
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * This example uses the StepperDS library that targets the 
 * dir/step interface of the EasyDriver, or Pololu driver
 *
 * This example just moves the stepper through a pre-set sequence 
 * of rotations. It prints what it's doing to the serial monitor. 
 * 
 */

#include <StepperDS.h>

// change this to the number of steps on your motor
// 1.8 degrees per step = 200 steps per revolution
#define STEPS 200

// define the dir and step pins
#define DIR_PIN 8
#define STEP_PIN 9

// create an instance of the StepperDS class, specifying
// the number of steps of the motor and the pins it's
// attached to (dir, step)
StepperDS stepper(STEPS, DIR_PIN, STEP_PIN);

void setup()
{
  // set the speed of the motor to 60 RPMs
  stepper.setSpeed(60);
  
  // set up serial connection so you can print what's going on
  Serial.begin(9600); 
}

void loop()
{
  //rotate a specific number of degrees 
  Serial.println("rotateDeg(360) - 60 RPM i.e. clockwise fast");
  stepper.stepDeg(360);
  delay(1000);

  Serial.println("rotateDeg(-360) - at speed 10 i.e. CCW slow"); 
  stepper.setSpeed(10); 
  stepper.stepDeg(-360);  //reverse
  delay(1000); 

  //rotate a specific number of microsteps (default is full-step)
  // 1.8 degree motors have 200 steps per revolution
  Serial.println("rotate(300) CW at half speed two times around");
  stepper.setSpeed(30); 
  stepper.step(300); 
  delay(1000); 

  Serial.println("rotate(-300) CCW at quarter speed, two times around"); 
  stepper.setSpeed(15);
  stepper.step(-300); //reverse
  delay(1000);

 
}
