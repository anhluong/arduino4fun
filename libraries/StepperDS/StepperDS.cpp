/*
 StepperDS.h - - Stepper library for Arduino, using a Dir/Step (DS) driver
 such as the EasyDriver or Pololu (A4988) driver
 
 version (0.1) by Erik Brunvand
 
 Loosly based on the "Stepper" library: 
 Original library     (0.1) by Tom Igoe.
 Two-wire modifications   (0.2) by Sebastian Gassner
 Combination version   (0.3) by Tom Igoe and David Mellis
 Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley
 
 This is based on the original Arduino Stepper library, but uses the 
 step/dir interface instead of driving the stepper phases directly.
 
 These Dir/Step drivers use a two wire interface where one pin says 
 what direction, and the other causes the stepper to move one step
 for each rising edge on the step pin.  
  
 These drivers also allow microstepping by driving the motors so 
 that they move a fraction of a full step on each "step" pulse. The 
 microstepping is set by some configuration pins on the driver boards. 
 This library doesn't manage the microstepping configuration pins. It 
 assumes that you've already done that. But, you can tell the StepperDS
 objects what you've set things to. 
 
 If you do set the microstepping to something besides 1, then the 
 library scales the delay_per_step so that you're still operating
 at the overall RPM that you specified. That is, it automatically 
 reducesthe delay_per_step to compensate for the larger number of 
 steps required to go 360 degrees. But, it's up to the user to know 
 how many steps it takes to go all the way around. The "step" method
 always takes the raw number of (micro)steps to move. 
 
 defaults: 
 
 steps_per_rev: 200 (1.8 degree motor)(this parameter is always defined 
    for full-step. That is, how many full-steps for a complete revolution.) 
 RPM: 100 rpm (quite fast!)
 microStepping: 1 (full step) (possible values = 1, 2, 4, 8, 16)
 
 step_delay is computed to be in ms/step of delay. The calculation is
 
	ms/min * rev/step * min/rev = ms/step. 
 
 This code uses the "measure the millis" technique for figuring out when 
 the step_delay has expired and you should move to the next motor phase. 
 In this case I wonder if the more simple delay() function wouldn't work
 just as well because you're stuck in the "step" loop until the movment is
 finished anyway... But, this way should work just fine. 
 
 */


#include "StepperDS.h"

// STEP_EDGE is the delay in microseconds between step-pin edges
// The A4988 driver used on the Pololu board has a 1us min 
// delay between step edges.
#define STEP_EDGE 1				// delay between step-pin edges (us) 
#define DEFAULT_RPM 60			// default RPM speed of the motor
#define DEFAULT_STEPS 200		// default steps/rev (200 = 1.8deg/step)
#define DEFAULT_MICRO 1			// default microstepping divisor (1, 2, 4, 8, or 16)
#define MS_PER_MIN 60L * 1000L	// number of microseconds in a second

/*
 * zero-arg constructor.
 * 
 */
StepperDS::StepperDS()
{
	this->RPM = DEFAULT_RPM;				// motor speed in RPM
	this->steps_per_rev = DEFAULT_STEPS;	// total number of steps for this motor
	this->microStepping = DEFAULT_MICRO;	// microstepping divisor
	// step_delay is ms per step. 
	// 60000ms/min * 1min/60rev * 1rev/200steps  = ms/step
	this->step_delay = MS_PER_MIN / DEFAULT_RPM / DEFAULT_STEPS; 
	
	// Arduino pins for the motor control connection. It's not valid
	// to have them both 0. If you use this constructor, you need to 
	// use the name.attach(dir,step); function to assign pins
	this->dirPin = 0;
	this->stepPin = 0;

}


/*
 * two-arg constructor.
 * Sets which wires should control the motor.
 */
StepperDS::StepperDS(int dirPin, int stepPin)
{
	this->RPM = DEFAULT_RPM;				// motor speed in RPM
	this->steps_per_rev = DEFAULT_STEPS;	// total number of steps for this motor
	this->microStepping = DEFAULT_MICRO;	// microstepping divisor
	this->step_delay = MS_PER_MIN / DEFAULT_RPM / DEFAULT_STEPS; 
  
	// Arduino pins for the motor control connection:
	this->dirPin = dirPin;
	this->stepPin = stepPin;

	initStepperPins(); // init the stepper pins
	
 }

/*
 * three-arg constructor.
 * Sets steps per rev, and which wires should control the motor
 */
StepperDS::StepperDS(int steps_per_rev, int dirPin, int stepPin)
{
	this->RPM = DEFAULT_RPM;				// motor speed in RPM
	this->steps_per_rev = steps_per_rev;	// total number of steps for this motor
	this->microStepping = DEFAULT_MICRO;	// microstepping divisor
	this->step_delay = MS_PER_MIN / DEFAULT_RPM / steps_per_rev; 
	
	// Arduino pins for the motor control connection:
	this->dirPin = dirPin;
	this->stepPin = stepPin;
	
	initStepperPins(); // init the stepper pins
	
}

/*
 * four-arg constructor.
 * Sets steps per rev, and which wires should control the motor, and microstepping
 */
StepperDS::StepperDS(int steps_per_rev, int dirPin, int stepPin, int microStepping)
{
	this->RPM = DEFAULT_RPM;				// motor speed in RPM
	this->steps_per_rev = steps_per_rev;	// total number of steps for this motor
	this->microStepping = microStepping;	// microstepping divisor
	this->step_delay = MS_PER_MIN / DEFAULT_RPM / steps_per_rev / microStepping; 
	
	// Arduino pins for the motor control connection:
	this->dirPin = dirPin;
	this->stepPin = stepPin;
	
	initStepperPins(); // init the stepper pins
	
}

/*
 * five-arg constructor.
 * Sets steps_per_rev, and which wires should control the motor, microstepping, and speed
 */
StepperDS::StepperDS(int steps_per_rev, int dirPin, int stepPin, int microStepping, int whatSpeed)
{
	this->RPM = whatSpeed;					// motor speed in RPM
	this->steps_per_rev = steps_per_rev;	// total number of steps for this motor
	this->microStepping = microStepping;	// microstepping parameter
	this->step_delay = MS_PER_MIN / whatSpeed / steps_per_rev / microStepping; 
	
	// Arduino pins for the motor control connection:
	this->dirPin = dirPin;
	this->stepPin = stepPin;
	
	initStepperPins(); // init the stepper pins
	
}

/*
 Initialize the step/dir pins as ouputs, and to a LOW value
 */
void StepperDS::initStepperPins()
{
	// setup the pins on the microcontroller as outputs
	pinMode(dirPin, OUTPUT);
	pinMode(stepPin, OUTPUT);
	// set the output values to some defaults
	digitalWrite(dirPin, LOW); 
	digitalWrite(stepPin, LOW); 
}

/*
 * Set the dir and step pins for the stepper
 */
void StepperDS::attach(int dir, int step)
{
	this->dirPin = dir;
	this->stepPin = step; 
	
	initStepperPins(); // init the stepper pins
}

/*
  Sets the speed in revs per minute
*/
void StepperDS::setSpeed(int whatSpeed)
{
	this->RPM = whatSpeed;
	this->step_delay = MS_PER_MIN / whatSpeed / steps_per_rev / microStepping;
}

/*
 Sets the microStepping parameter - also sets speed as a byproduct
 The microStepping should be 1, 2, 4, 8, or 16 depending on what your 
 driver board supports. If it's something else, the stepper will still
 operate, but the number of steps to make a full revoluation won't be 
 so obvious. But, the function doesn't check to make sure the number is
 in range... 
 */
void StepperDS::setMicroStepping(int microStepping)
{
	this->microStepping = microStepping;
	this->step_delay = MS_PER_MIN / RPM / steps_per_rev / microStepping;
}

/*
 Moves the motor steps_to_move steps.  If the number is negative, 
 the motor moves in the reverse direction. This measures the time
 in ms since the last step so that other things can keep going in 
 the background. 
 
 Because we're targetting a step/dir stepper driver, we need
 to set the dir pin, and then make the step pin go both high 
 and low to take a single step. 
 */
void StepperDS::step(int steps_to_move)
{  
	int steps_left = abs(steps_to_move);  // how many steps to take	
  
	// determine direction based on whether steps_to_move is + or -:
	// set the direction pin to the correct value
	digitalWrite(dirPin, (steps_to_move > 0) ? HIGH:LOW);
	delayMicroseconds(STEP_EDGE); // tiny delay for dirPin to take effect
	// the stepPin will start out low because it's initialized that way, and
	// this step function leaves it LOW when it's done. 
    
	// Decrement the number of steps, moving one step each time.
	// A full step needs both rising and falling edges on stepPin.
	while(steps_left > 0) {
		// move only if the appropriate delay has passed:
		if (millis() - last_step_time >= step_delay) { // time to take a step
			// get the timeStamp of when you stepped:
			this->last_step_time = millis();
			
			steps_left--;					// decrement the number of steps left
			digitalWrite(stepPin, HIGH);	// Take a single step
			delayMicroseconds(STEP_EDGE);	// make sure to delay a little
			digitalWrite(stepPin, LOW);		// between stepPin edges
			delayMicroseconds(STEP_EDGE); 
								
		}
	}
}

/* 
 Moves the motor by deg degrees. This value can be negative to move 
 in the other direction. This has both an int and float version. Especially 
 with microstepping, you might wnat to specify degrees with a decimal point
 */
void StepperDS::stepDeg(int number_of_degrees)
{
	// Figure out how many steps are in that number of degrees
	// degrees * steps/rev * microsteps/step * 1rev/360deg = # of microsteps
	// Need a long here for the intermediate values in steps
	long steps = ((long)number_of_degrees * steps_per_rev * microStepping) / 360L; 
	// Now, move that number of steps
	step((int)steps); 
	
}
void StepperDS::stepDeg(long number_of_degrees)
{
	// Figure out how many steps are in that number of degrees
	// degrees * steps/rev * microsteps/step * 1rev/360deg = # of microsteps
	// Need a long here for the intermediate values in steps
	long steps = (number_of_degrees * steps_per_rev * microStepping) / 360L; 
	// Now, move that number of steps
	step((int)steps); 
	
}

/*
 version with a floating point degree arguments. 
 */
void StepperDS::stepDeg(float number_of_degrees)
{
	// Figure out how many steps are in that number of degrees
	// degrees * steps/rev * microsteps/step * 1rev/360deg = # of microsteps
	float steps = (number_of_degrees * steps_per_rev * microStepping) / 360.0f; 
	// Now, move that number of steps - cast back to int
	step((int)steps); 
	
}
void StepperDS::stepDeg(double number_of_degrees)
{
	// Figure out how many steps are in that number of degrees
	// degrees * steps/rev * microsteps/step * 1rev/360deg = # of microsteps
	float steps = ((float)number_of_degrees * steps_per_rev * microStepping) / 360.0f; 
	// Now, move that number of steps - cast back to int
	step((int)steps); 
	
}



/*
  version() returns the version of the library:
*/
int StepperDS::version(void)
{
  return 1;
}
