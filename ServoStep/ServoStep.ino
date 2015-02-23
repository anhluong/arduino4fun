#include <Servo.h>

const int servoPin = 7;

Servo servo;

void setup()
{
  servo.attach(servoPin);
}

int counter = 135;

void loop()
{
  servo.write(counter);
  //if(counter == 180)
  //    counter = 90;
  //else if(counter == 90)
  //    counter = 90;
  //else
      counter++;
  delay(100);
}

