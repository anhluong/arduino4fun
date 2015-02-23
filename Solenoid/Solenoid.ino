

const int button = 1;    		// button pin

const int pumpSolenoid =  10;      // pump control pin

const int releaseSolenoid =  11; 	// pressure release pin



int buttonState = 0;         // pushbutton status



void setup() {

  // Solenoid control = outputs

  pinMode(pumpSolenoid, OUTPUT);     

  pinMode(releaseSolenoid, OUTPUT);   

  Serial.begin(9600);  

  // button = input (duh)

  pinMode(button, INPUT);    

}



void loop(){

  // State of the button

  buttonState = digitalRead(button);

  Serial.println(buttonState);


  // if pressed, extrude

  if (buttonState == HIGH) {   

	 // Close release valve	(retain pressure)

	digitalWrite(releaseSolenoid, LOW);  

     // Open pump control 	(increase downward pressure)

    digitalWrite(pumpSolenoid, HIGH);  

  }

  // else, stop extruding

  else {

  	// Open release valve	(release pressure)

	digitalWrite(releaseSolenoid, HIGH);

    // Close pump control 	(kill pressure)

    digitalWrite(pumpSolenoid, LOW);

  }

}
