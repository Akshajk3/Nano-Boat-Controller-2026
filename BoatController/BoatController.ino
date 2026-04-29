// Pin Definitions
const int potPin = A0;
const int enPin = 8;
const int dirPin = 9;
const int stepPin = 10;

// Variables
int potValue = 0;
int targetValue = 512; // Mid-point of 0-1023 range
int speedDelay = 800;  // Microsecondszzzzzzzzzzzzzzzzzzzzzzzzzz
bool isEnabled = true;

void setup() {
  Serial.begin(115200);
  
  pinMode(enPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  // Pulling Enable LOW disables it. 
  // HIGH to start.
  digitalWrite(enPin, HIGH); 
}

void loop() {

  // CHECK FOR NEW TARGET FROM PI
  if (Serial.available() > 0) {
    // Read the incoming string until a newline character
    String input = Serial.readStringUntil('\n');
    int newTarget = input.toInt();
    
    // Constrain the value to valid Potentiometer range (0-1023)
    if (newTarget >= 0 && newTarget <= 1023) {
      targetValue = newTarget;
    }
  }

  potValue = analogRead(potPin);
  

  potValue = analogRead(potPin);
  
  // Deadzone to prevent the motor from jittering 

  if (abs(potValue - targetValue) > 5) {
    isEnabled = true;
    digitalWrite(enPin, HIGH); 
    
    // Set Direction
    if (potValue < targetValue) {
      digitalWrite(dirPin, HIGH);
    } else {
      digitalWrite(dirPin, LOW);
    }
    
    // Pulse the motor
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedDelay);
    
  } else {
    // We reached the target
    isEnabled = false;
    digitalWrite(enPin, LOW); 
  }

  // Output to Serial Monitor
  printStatus();
}

void printStatus() {
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 100) { // Limit output to every 100ms
    Serial.print("POT: "); Serial.print(potValue);
    Serial.print(" | TARGET: "); Serial.print(targetValue);
    Serial.print(" | EN: "); Serial.print(isEnabled ? "ON" : "OFF");
    Serial.print(" | DIR: "); Serial.print(digitalRead(dirPin));
    Serial.print(" | FREQ: ~"); Serial.print(1000000 / (speedDelay * 2)); 
    Serial.println("Hz");
    lastPrint = millis();
  }
}
