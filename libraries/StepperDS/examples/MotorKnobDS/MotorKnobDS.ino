/*
 * MotorKnobDS
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * This example uses the StepperDS library that targets the 
 * dir/step interface of the EasyDriver, or Pololu driver
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

// the previous reading from the analog input
int previous = 0;

void setup()
{
  // set the speed of the motor to 100 RPMs
  stepper.setSpeed(100);
}

void loop()
{
  // get the sensor value
  int val = analogRead(0);

  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(val - previous);

  // remember the previous value of the sensor
  previous = val;
}
