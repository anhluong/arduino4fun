#ifndef _SolenoidSyringe_h
#define _SolenoidSyringe_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class SolenoidSyringe
{
public:
	//minimal constructor - inputs are defaults
  SolenoidSyringe(int intakePin, int outtakePin, double intakePSI);
	//versatile constructor
  SolenoidSyringe(double diameterNeedle, double diameterSyringe, double inputPSI, double densityInk,
				  int intakePin, int outtakePin);

  // repeat these 2 steps to update time and disperse
  // split in case calulation make take too much time (able to preempt)
  
  // *** Step 1
  void calculateDispersionTime(double volumeToDisperse);
  // *** Step 2
  // returns 0 if dispersionTime = 0 or failure
  // returns 1 if successful
  int disperse();
  
  void testDispersion(double dispersionTime);
  double getdispersionTime();
    
private:
/*
   double dispersionTime	= 0;
   double volumeToDisperse;
   double diameterNeedle	= 0.21;		// assumes 27 gauge needle
   double diameterSyringe 	= 15.57;	// assume std 10cc/ml syringe
   double areaNeedle		= 0.0346;	// assumes 27 gauge needle
   double areaSyringe		= 190.4;	// assume std 10cc/ml syringe
   double inputPSI			= 100;		// assuming 100 PSI compressor
   double velocityNeedle;
   double velocitySyringe	= 0;		// negligable without using calculus
   double pressureSyringe	= 0;
   double density			= .000001;	// default = water in kg/mm^3	
 */
   double dispersionTime;
   double volumeToDisperse;
   double diameterNeedle;
   double diameterSyringe;
   double areaNeedle;
   double areaSyringe;
   double intakePSI;
   double velocityNeedle;
   double velocitySyringe;
   double pressureSyringe;
   double density;
   const static double psiToPaConversion	= 0.689476;	// Pascals in N/mm^2
   
   int currentTime;
   int tempTime;
   
   int intakePin;
   int outtakePin;
   
   double calcPascals(double inputPSI);
   double calcNeedleArea(double diameterNeedle);
   double calcSyringeArea(double diameterSyringe);
   double calcDispersionTime(double areaNeedle, double pressureSyringe, double volumeToDisperse);
   
   void beginDispersion();
   
   void setupDefaults();
   void calcNeedleArea();
   void calcSyringeArea();
   void calcDensity();
   };
#endif