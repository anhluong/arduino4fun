const int pulse = 9;
void setup()
{
  pinMode(pulse, OUTPUT);
}

void loop()
{
  //digitalWrite(pulse, HIGH);
  //delayMicroseconds(1500);
  //digitalWrite(pulse, LOW);
  //delayMicroseconds(1500);
  analogWrite(pulse, 2000);
  delay(300);
  analogWrite(pulse, 4000);
  delay(300);
}
