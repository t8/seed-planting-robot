#include <Servo.h>
#include <AFMotor.h>

Servo name_servo;

int servo_position = 0;

void setup() {
  name_servo.attach(10);
}

void loop() {
//  for (servo_position = 0; servo_position <=180; servo_position +=1) {
//    name_servo.write(servo_position);
//    delay(10);
//  }
  
//  for (servo_position = 180; servo_position >=0; servo_position -=1) {
//    name_servo.write(servo_position);
//    delay(10);
//  }
  name_servo.write(servo_position);
}
