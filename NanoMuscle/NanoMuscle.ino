const int control = 8;
const int contracted = 7;
const int extended = 6;
const int led = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(control, OUTPUT);
  pinMode(contracted, INPUT);
  pinMode(extended, INPUT);
}

void loop()
{
  digitalWrite(control, HIGH);
  
  digitalWrite(led, LOW);
  
  while(!digitalRead(contracted))
  {
    Serial.println("not contracted yet");
    delayMicroseconds(1000000);
    digitalWrite(led, HIGH);
  }
  
  
  digitalWrite(led, LOW);
  
  digitalWrite(control, LOW);
  
  while(!digitalRead(extended))
  {
    Serial.println("not extended yet");
    delayMicroseconds(1000000);
    digitalWrite(led, HIGH);
  }
}
