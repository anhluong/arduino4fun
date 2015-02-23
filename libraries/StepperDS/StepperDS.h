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
 
 That number is then divided by 2 because you need two of them per step. 
 The stepPin needs to go both high and low for a single step of the stepper. 
 
 This code uses the "measure the millis" technique for figuring out when 
 the step_delay has expired and you should move to the next motor phase. 
 In this case I wonder if the more simple delay() function wouldn't work
 just as well because you're stuck in the "step" loop until the movment is
 finished anyway... But, this way should work just fine. 
 
 
  */

// ensure this library description is only included once
#ifndef StepperDS_h
#define StepperDS_h

// Hack to include the right .h file depending on which version of
// the Arduino IDE you're usi
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// library interface description
class StepperDS {
  public:
    // constructors:
	StepperDS();
	StepperDS(int dirPin, int stepPin); 
    StepperDS(int steps_per_rev, int dirPin, int stepPin);
    StepperDS(int steps_per_rev, int dirPin, int stepPin, int microStepping);
	StepperDS(int steps_per_rev, int dirPin, int stepPin, int microStepping, int whatSpeed );

	// Attach (or change) the pin assignment of a stepper
	void attach(int dirPin, int stepPin); 

    // speed setter method (in rpm) 
    void setSpeed(int whatSpeed);
	
	// set microstepping parameter (1, 2, 4, 8, or 16) 
	void setMicroStepping(int microStepping); 

    // move by number of (micro)steps method
    void step(int number_of_steps);
	
	// move by degrees method (both int degrees and float degrees)
	// I'm including all the possibilities, just for completeness
	void stepDeg(int number_of_degrees); 
	void stepDeg(long number_of_degrees);
	void stepDeg(float number_of_degrees);
	void stepDeg(double number_of_degrees);
	
	// return the version number
    int version(void);

  private:
	void initStepperPins();     // initialize the stepper pins
    
	int RPM;					// speed in RPM
    unsigned long step_delay;   // delay between steps, in ms, based on speed and microStepping
    int steps_per_rev;		    // steps in one full revolution at full-stepping
	long last_step_time;		// time stamp in ms of when last step was taken
	int microStepping;			// microStepping divisor (1, 2, 4, 8, or 16)
    
    // motor driver pin numbers:
    int dirPin;
    int stepPin;
    
};

#endif

