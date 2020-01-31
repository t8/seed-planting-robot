#include <Servo.h>
#include <AccelStepper.h>

AccelStepper leftStepper(AccelStepper::DRIVER, 7, 6);
AccelStepper rightStepper(AccelStepper::DRIVER, 9, 8);

#define augerPin 5

// Serial Communication Settings
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPi = false;

char messageFromPi[buffSize] = {0};
float newLeftStepperSpeed = 0.0;
float newRightStepperSpeed = 0.0;
char dir[buffSize] = {0};


unsigned long curMillis;

unsigned long prevReplyToPimillis = 0;
unsigned long replyToPiinterval = 1000;

// Motor Settings
float leftStepperSpeed;
float rightStepperSpeed;

// Servo settings
Servo auger;
byte augerSpeed;
byte augerMin = 0;
byte augerMax = 180;
byte newAugerSpeed = 90;

void setup() {
    Serial.begin(9600);
    leftStepper.setMaxSpeed(20000.0);
    leftStepper.setAcceleration(1000.0);
    rightStepper.setMaxSpeed(20000.0);
    rightStepper.setAcceleration(1000.0);
    auger.attach(augerPin);
    Serial.println("<CASPR Arduino Online>");
}

void loop() {
  curMillis = millis();
  getDataFromPi();
  updateMotors();
  replyToPi();
  moveServo();
}

void getDataFromPi() {

  // receive data from pi and save it into inputBuffer

  if (Serial.available() > 0) {

    char x = Serial.read();

    // the order of these IF clauses is significant

    if (x == endMarker) {
      readInProgress = false;
      newDataFromPi = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }

    if (readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) {
      bytesRecvd = 0;
      readInProgress = true;
    }
  }
}

void parseData() {

  // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(inputBuffer, ",");     // get the first part - the string
  strcpy(messageFromPi, strtokIndx); // copy it to messageFromPi

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  newLeftStepperSpeed = atof(strtokIndx);     // convert this part to an float

  strtokIndx = strtok(NULL, ",");
  newRightStepperSpeed = atof(strtokIndx);     // convert this part to an float

}

void replyToPi() {

  if (newDataFromPi) {
    newDataFromPi = false;
    Serial.print("<Msg ");
    Serial.print(messageFromPi);
    Serial.print(" Speeds ");
    Serial.print(newLeftStepperSpeed);
    Serial.print(" ");
    Serial.print(newRightStepperSpeed);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

void updateMotors() {
  // this illustrates using different inputs to call different functions
  if (strcmp(messageFromPi, "L") == 0) {                    // Left stepper control
    moveLeftStepper();
  } else if (strcmp(messageFromPi, "R") == 0) {             // Right stepper control
    moveRightStepper();
  } else if (strcmp(messageFromPi, "A") == 0) {             // Auger control
    moveServo();
  } else if (strcmp(messageFromPi, "B") == 0) {             // Both stepper control
    moveLeftStepper();
    moveRightStepper();
  } else if (strcmp(messageFromPi, "H") == 0) {             // Halt
//    moveLeftStepper();
//    moveRightStepper();
//    moveServo();
    return;
  }
}


void moveServo() {
  float newAugerSpeed = newLeftStepperSpeed * 90.0;
  if (augerSpeed != newAugerSpeed && newAugerSpeed >= 0.0 && newAugerSpeed <= 2.0) {
    augerSpeed = newAugerSpeed;
    auger.write(augerSpeed);
  }
}

void moveLeftStepper() {
  if (newLeftStepperSpeed != 0) {
    leftStepperSpeed = newLeftStepperSpeed;
    leftStepper.setSpeed(leftStepperSpeed);
    leftStepper.runSpeed();
  }
}

void moveRightStepper() {
  if (newRightStepperSpeed != 0) {
    rightStepperSpeed = newRightStepperSpeed;
    rightStepper.setSpeed(rightStepperSpeed);
    rightStepper.runSpeed();

  }
}
