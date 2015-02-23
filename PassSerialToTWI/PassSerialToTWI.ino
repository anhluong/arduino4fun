#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(19200);
}

char buffer[1] = {0};

void loop()
{
  if(Serial.available() > 0)
  {
    while(1)
    {
      digitalWrite(13, HIGH);
      while(Serial.available() <= 0)
      {
        delayMicroseconds(1);
      }
      
      if(Serial.read() == 0xaa)
      {
        if(Serial.read() == 0x55)
        {
          Wire.beginTransmission(0x00);
          Wire.write(Serial.read());
          Wire.write(Serial.read());
          Wire.write(Serial.read());
          Wire.endTransmission();
        }
      }
      digitalWrite(13, LOW);
      delay(100);
    }
  }
  delay(10);
}
