#include <Servo.h>
#include <AFMotor.h>

Servo name_servo;

int servo_position = 0;
AF_Stepper leftm(200, 2);
AF_Stepper rightm(200, 1);

void setup() {
  name_servo.attach(10);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");
 
  leftm.setSpeed(50);  // 10 rpm   
  rightm.setSpeed(50);  // 10 rpm   
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
  for(int s=0; s<200; s++)
  {
    leftm.step(1, FORWARD, DOUBLE);
    rightm.step(1, FORWARD, DOUBLE); 
//    delay(10);
  }
//  leftm.step(200, FORWARD, MICROSTEP); 
//  rightm.step(200, FORWARD, MICROSTEP);
//  motor.step(100, BACKWARD, SINGLE); 
 
//  motor.step(100, FORWARD, DOUBLE); 
//  motor.step(100, BACKWARD, DOUBLE);
// 
//  motor.step(100, FORWARD, INTERLEAVE); 
//  motor.step(100, BACKWARD, INTERLEAVE); 
// 
//  motor.step(100, FORWARD, MICROSTEP); 
//  motor.step(100, BACKWARD, MICROSTEP); 
}
