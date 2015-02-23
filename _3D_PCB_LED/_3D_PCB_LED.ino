
const int ledPin =  11;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);      
}

void loop()
{

   for(int i = 0; i < 3
   0; i++)
  { 
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  
    delay(100);
  }

   for(int i = 0; i < 3; i++)
  { 
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
  }
  
}

