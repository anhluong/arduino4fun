/*
#include <SoftwareSerial.h>

SoftwareSerial mySerial =  SoftwareSerial(3, 2);


void setup()  {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}



void loop()                     // run over and over again
{

  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
  delay(100);
}
*/

/* Serial Test code
Sends "testing..." over the serial connection once per second
while blinking the LED on pin 13. Used for testing serial devices
such as the XBee.
Adapted from the SoftSerial Demonstration code
*/
#define ledPin 13

byte pinState = 0;

void setup() {
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
}
void loop() {
Serial.println("testing...");
// toggle an LED just so you see the thing's alive.
toggle(13);
delay(1000);
}
void toggle(int pinNum) {
// set the LED pin using the pinState variable:
digitalWrite(pinNum, pinState);
// if pinState = 0, set it to 1, and vice versa:
pinState = !pinState;
}
