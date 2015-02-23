//#include <Servo.h>
#include <ServoSyringe.h>
#include <math.h>

const int pin = 2;

ServoSyringe syringe(pin);

void setup()
{
  syringe.setLinearDistancePermL(5.14);
  syringe.setRadiusSyringe(44.36);
  syringe.setDropTime(5e3);
  syringe.attach(pin);
  syringe.write(150);
  Serial.begin(9600);
  delay(10000);
}

int count = 1;

void loop()
{
  if(count)
  {
    //double t = 2 * asin( (double) (5.14/2.0) / 44.36) * 180 / M_PI;
    //Serial.println(t);
    //syringe.write(syringe.read() + 2*t);
    //delay(1000);
    syringe.pushmL(6);
    count = 0;
  }
}
