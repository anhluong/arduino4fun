#include "SolenoidSyringe.h"
#include <math.h>

SolenoidSyringe::SolenoidSyringe(int intakePin, int outtakePin, double intakePSI)
{
	//set up closed solenoids
	// Solenoid control = outputs
	this->intakePin = intakePin;
	this->outtakePin = outtakePin;
	this->intakePSI = intakePSI;
	
	pinMode(intakePin, OUTPUT);     
	pinMode(outtakePin, OUTPUT);  
	
	digitalWrite(intakePin, LOW);
	digitalWrite(outtakePin, LOW);
	
	pressureSyringe = intakePSI * psiToPaConversion;
	
	setupDefaults();
}

SolenoidSyringe::SolenoidSyringe(double diameterNeedle, double diameterSyringe,
					double intakePSI, double density, int intakePin, int outtakePin)
{
	//set up closed solenoids
	// Solenoid control = outputs
	this->intakePin = intakePin;
	this->outtakePin = outtakePin;
	
	pinMode(intakePin, OUTPUT);     
	pinMode(outtakePin, OUTPUT);  
	
	digitalWrite(intakePin, LOW);
	digitalWrite(outtakePin, LOW);
	
	
	this->diameterNeedle = diameterNeedle;
	this->diameterSyringe = diameterSyringe;
	this->intakePSI = intakePSI;
	this->density = density;
	
	beginDispersion();
    calcNeedleArea();
    calcSyringeArea();

}
  // *** Step 1
void SolenoidSyringe::calculateDispersionTime(double volumeToDisperse)
{
	dispersionTime = (areaNeedle * sqrt(2) * sqrt(density * (pressureSyringe - 1))) / 
					(density * volumeToDisperse);
					
	//calculate in milliSeconds			
	dispersionTime = dispersionTime / 1000;
}
  // *** Step 2
int SolenoidSyringe::disperse()
{
	if (!dispersionTime)
		return 0;
		
	beginDispersion();

	return 1;
}

void SolenoidSyringe::beginDispersion()
{
	//assumes both solenoids are closed
	currentTime = millis();
	tempTime = currentTime + dispersionTime;
	
	// open intake until temp time reached
	while (tempTime <= millis())
	{
		digitalWrite(intakePin, HIGH);
	}
	
	// open outtake to start releasing pressure
	// then close intake
	digitalWrite(outtakePin, HIGH);
	digitalWrite(intakePin, LOW);

	// wait 1/10 sec for all excess pressure to leave
	delay(100);
	
	// close outtake valve
	digitalWrite(outtakePin, LOW);
}
void SolenoidSyringe::setupDefaults()
{

   dispersionTime	= 0;
   diameterNeedle	= 0.21;		// assumes 27 gauge needle
   diameterSyringe 	= 15.57;	// assume std 10cc/ml syringe
   areaNeedle		= 0.0346;	// assumes 27 gauge needle
   areaSyringe		= 190.4;	// assume std 10cc/ml syringe
   intakePSI			= 100;		// assuming 100 PSI compressor
   velocitySyringe	= 0;		// negligable without using calculus
   pressureSyringe	= 0;
   density			= .000001;	// default = water in kg/mm^3	
    
}

double SolenoidSyringe::getdispersionTime()
{
	return dispersionTime;
}

void SolenoidSyringe::testDispersion(double dispersionTime)
{
	//assumes both solenoids are closed
	currentTime = millis();
	tempTime = currentTime + dispersionTime;
	
	// open intake until temp time reached
	//digitalWrite(intakePin, HIGH);
	
	//** testing
	//Serial.println("	Opened intake - Wait for dispersionTime");
	
	delay(dispersionTime);
    //digitalWrite(intakePin, LOW);
    // Serial.println("	Closed intake - Wait for dispersionTime");
		
    delay(3000);
	// open outtake to start releasing pressure
	// then close intake
	digitalWrite(outtakePin, HIGH);
	
	//** testing
	Serial.println("	Opened outtake - Wait for 1 Sec...");
	delay(3000);
	
	//digitalWrite(intakePin, LOW);
	
	Serial.println("	closed outtake - Wait for 1 Sec...");
	//delay(100);

	// wait 1/10 sec for all excess pressure to leave
	delay(3000);
	
	// close outtake valve
	digitalWrite(outtakePin, LOW);
	Serial.println("	Closed outtake - dispersion done");
}


//------------ used with complex constructor-------------------

void SolenoidSyringe::calcNeedleArea()
 {
	areaNeedle = exp(diameterNeedle / 2) * PI; 
 }
void SolenoidSyringe::calcSyringeArea()
 {
	areaSyringe = exp(diameterSyringe / 2) * PI; 
 }
