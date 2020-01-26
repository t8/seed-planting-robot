/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */
// Define stepper motor connections:


#define leftStepperDirPin 6
#define leftStepperStepPin 7

#define rightStepperDirPin 8
#define rightStepperStepPin 9

void setup() {
  // Declare pins as output:
  pinMode(leftStepperStepPin, OUTPUT);
  pinMode(leftStepperDirPin, OUTPUT);

  pinMode(rightStepperStepPin, OUTPUT);
  pinMode(rightStepperDirPin, OUTPUT);
  // Set the spinning direction CW/CCW:
  digitalWrite(leftStepperDirPin, HIGH);
  digitalWrite(rightStepperDirPin, HIGH);
}

void loop() {
  if(Serial.available()){         //From RPi to Arduino
    command = Serial.read();
    Serial.println(command);
  }
  // These four lines result in 1 step:
  digitalWrite(leftStepperStepPin, HIGH);
  digitalWrite(rightStepperStepPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(leftStepperStepPin, LOW);
  digitalWrite(rightStepperStepPin, LOW);
  delayMicroseconds(1000);
}