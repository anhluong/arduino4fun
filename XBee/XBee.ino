int DIN = 2;
int DOUT = 3;

void setup()
{
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);
}

byte incomingByte;

void loop()
{
    int sensorValue = digitalRead(2);
    Serial.println(sensorValue);
}
