#ifndef planetObjects_H
#define planetObjects_H

#include <Servo.h>
#include <StepperDS.h>
#include <Arduino.h>
#include <math.h>


class point{
public:
 int x;
 int y; 
 point(); 
 point(double X, double Y);
 point(int X, int Y);
};

class ptServo{
public:
 Servo pan; //Servo for panning
 Servo tilt; //Servo for tilting
 int theta; //Angle from left edge
 int phi; //90 - theta
 point xy;
 ptServo(Servo s1, Servo s2, int start, point thePoint);
 ptServo();
 void move(int p, int t);
 void dance(int scale);
};

class tripoint{
public:
	long x;
	long y;
	long z;
	tripoint();
	point d2tri(int a, int b, int c, int t1, int t2, int t3, int LENGTH, int WIDTH);
	void d3tri(ptServo & pts, point tar, int HEIGHT);
};

class sonic{
public:
	Servo mover;
	int theta; //Angle
	int pingPin;
	bool dir;
	sonic();
	sonic(Servo m, int pin);
	long ping();
	//tripoint ping();
	long microsecondsToCentimeters(long microseconds);
	long microsecondsToInches(long microseconds);
	void move(int step);
};

#endif