#include <Servo.h>
#include <StepperDS.h>
#include <Arduino.h>
#include "planetObjects.h"
#include <math.h>

//point methods
  point::point()
  {
    x = 0;
    y = 0;
  }
  point::point(double X, double Y){
	x = (int) X;
	y = (int) Y;
  }
  point::point(int X, int Y){
	x = X;
	y = Y;
  }
  
//tripoint methods
tripoint::tripoint()
{
	x = 0;
	y = 0;
	z = 0;
}

/*
point tripoint::d2tri(int a, int b, int c, int t1, int t2, int t3, int LENGTH, int WIDTH){
  int p1, p2, p3; //phi 1 2 3
  int x1, x2, x3, y1, y2, y3; //We'll average results
  //find phi angles
  p1 = 90 - t1;
  p2 = 90 - t2;
  p3 = 90 - t3;
  
  //find values for x and y
  x1 = a * sin(t1);
  x2 = b * sin(p2);
  x3 = WIDTH - c * sin(t3);
  y1 = a * sin(p1);
  y2 = LENGTH - b * sin(t2);
  y3 = LENGTH - c * sin(p3);
  
  point tar = point();
  
  //average values (hopefully they are the same anyway)
  tar.x = (x1 + x2 + x3)/3; 
  tar.y = (y1 + y2 + y3)/3;  
  return tar;
*/
 point tripoint::d2tri(int a, int b, int c, int t1, int t2, int t3, int LENGTH, int WIDTH){
  //Using t1 as Theta, a as Rangefinder Distance
  
  point target;
  int l;
  
  target.x = LENGTH * .5;
  
  if (t1 > 90)
  {
	l = a * cos(180 - t1);
	target.x += l;
	target.y = a * sin(180 - t1);
  
  }
  else
  {
	l = a * cos (t1);
	target.x -= l;
	target.y = a * sin(t1);
  }
  return target;
}

void tripoint::d3tri(ptServo & pts, point tar, int HEIGHT){
  double phi, theta;
  theta = atan( abs( (tar.y - pts.xy.y)/(tar.x - pts.xy.x) ) );
  phi = atan( abs( (tar.x - pts.xy.x) / HEIGHT ) );
  if (tar.x < pts.xy.x){ //If target is to the 'right' of pan tilt
    theta = 180-theta; //Reverse pan angle, since tilt will point backwards
    phi += 90;  //Point tilt angle backwards
  }
  pts.pan.write((int) theta);
  pts.tilt.write((int) phi);
}

  //ptServo methods
 ptServo::ptServo(Servo s1, Servo s2, int start, point thePoint)
 {
  //Initialize stuff
  pan = s1;
  tilt = s2;
  theta = start;
  xy.x = thePoint.x;
  xy.y = thePoint.y;
  //Move block to starting position
  pan.write(start);
  tilt.write(start);
 }
 ptServo::ptServo()
 {}
 void ptServo::move(int p, int t){
	xy.x = p;
	xy.y = t;
	pan.write(p);
	tilt.write(t);
 }
 void ptServo::dance(int scale){
	pan.write(xy.x+scale);
	pan.write(xy.x-scale*2);
	pan.write(xy.x+scale);
	
	tilt.write(xy.y+scale);
	tilt.write(xy.y-scale*2);
	tilt.write(xy.y+scale);
 }
 

 //sonic methods
sonic::sonic(){}
sonic::sonic(Servo m, int pin){
	pingPin = pin;
	mover = m;
	theta = 0;
	dir = true;
}
/*
tripoint sonic::ping(){
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
   long d1, d2, d3;
   tripoint tri;
 
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
   pinMode(pingPin, OUTPUT);
   pinMode(pingPin+1, OUTPUT);
   pinMode(pingPin+2, OUTPUT);
   digitalWrite(pingPin, LOW);
   digitalWrite(pingPin+1, LOW);
   digitalWrite(pingPin+2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   digitalWrite(pingPin+1, HIGH);
   digitalWrite(pingPin+2, HIGH);
   delayMicroseconds(5);
   digitalWrite(pingPin, LOW);
   digitalWrite(pingPin+1, LOW);
   digitalWrite(pingPin+2, LOW);
 
  // The same pin is used to read the signal from the PING))): a HIGH
   // pulse whose duration is the time (in microseconds) from the sending
   // of the ping to the reception of its echo off of an object.
   pinMode(pingPin, INPUT);
   pinMode(pingPin+1, INPUT);
   pinMode(pingPin+2, INPUT);
   d1 = pulseIn(pingPin, HIGH);
   d2 = pulseIn(pingPin+1, HIGH);
   d3 = pulseIn(pingPin+2, HIGH);
   
   Serial.print(d1);
   Serial.print(", ");
   Serial.print(d2);
   Serial.print(", ");
   Serial.print(d3);
   Serial.println();
 
  // convert the time into a distance
   //inches = microsecondsToInches(duration);
   tri.x = microsecondsToCentimeters(d1);
   tri.y = microsecondsToCentimeters(d2);
   tri.z = microsecondsToCentimeters(d3);
   
   return tri;
}
*/

long sonic::ping(){
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
   long d1;
 
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
   // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(5);
   digitalWrite(pingPin, LOW);
 
  // The same pin is used to read the signal from the PING))): a HIGH
   // pulse whose duration is the time (in microseconds) from the sending
   // of the ping to the reception of its echo off of an object.
   pinMode(pingPin, INPUT);
   d1 = pulseIn(pingPin, HIGH);
 
  // convert the time into a distance
   d1 = microsecondsToInches(d1);
   //d1 = microsecondsToCentimeters(d1);
   
   return d1;
}


long sonic::microsecondsToCentimeters(long microseconds){
   // The speed of sound is 340 m/s or 29 microseconds per centimeter.
   // The ping travels out and back, so to find the distance of the
   // object we take half of the distance travelled.
   return microseconds / 29 / 2;
}
long sonic::microsecondsToInches(long microseconds){
   // According to Parallax's datasheet for the PING))), there are
   // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
   // second).  This gives the distance travelled by the ping, outbound
   // and return, so we divide by 2 to get the distance of the obstacle.
   // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
   return microseconds / 74 / 2;
}
void sonic::move(int step){
	if (dir){ //If going in original direction,
		if ((theta + step) < 175){ //If we can still move,
			theta += step;
		}
		else{
			dir = !dir; //Flip direction
			theta -= step;
		}
	}
	else{ //Not going original direction
		if ((theta - step) > 0){ //If we can still move,
			theta -= step;
		}
		else{
			dir = !dir; //Flip direction
			theta += step;
		}
	}
	mover.write(theta); //move
}