#define stepPin 31
#define dirPin 30

void setup() {
  Serial.begin(9600);
  Serial.println("Starting stepper exerciser.");

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, LOW);
}

void loop() {
  int i, j;

    i = 1000;
    
    Serial.print("Speed: ");
    Serial.println(i);
 
    for (j=0;; j++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(i);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(i);
    }
}
