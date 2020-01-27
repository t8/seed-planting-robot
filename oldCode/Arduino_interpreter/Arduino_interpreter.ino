void setup(){
  Serial.begin(9600);
}
void loop(){
  if(Serial.available()){         //From RPi to Arduino
    command = Serial.read();
    if (command === "turn on left motor") {
      turnOnLeftMotor();
    } else if (command === "turn on right motor") {
      turnOnRightMotor();
    } else if (command === "turn off left motor") {
      turnOffLeftMotor();
    } else if (command === "turn off right motor") {
      turnOffRightMotor();
    } else if (command === "turn on all motors") {
      turnOnAllMotors();
    } else if (command === "turn off all motors") {
      turnOffAllMotors();
    Serial.println(r);
  }
}

void turnOnLeftMotor(){
  
}

void turnOnRightMotor() {

}

void turnOffLeftMotor() {

}

void turnOffRightMotor() {

}

void turnOnAllMotors() {

}

void turnOffAllMotors() {
  
}
