#include <Servo.h>             // Servo library dependency
#include <AccelStepper.h>      // Stepper motor library dependencys

// Define stepper driver GPIO pins
AccelStepper leftStepper(AccelStepper::DRIVER, 7, 6);
AccelStepper rightStepper(AccelStepper::DRIVER, 9, 8);

// Define auger GPIO pins
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

// Counter to monitor interpret/response speed
unsigned long curMillis;

unsigned long prevReplyToPimillis = 0;
unsigned long replyToPiinterval = 1000;

// Stepper motor Settings
float leftStepperSpeed;
float rightStepperSpeed;

// Servo settings
Servo auger;
byte augerSpeed;
byte augerMin = 0;
byte augerMax = 180;
byte newAugerSpeed = 90;

void setup() {
  // Initiate serial communication signal with baud rate @ 9600
    Serial.begin(9600);
    leftStepper.setMaxSpeed(20000.0);
    leftStepper.setAcceleration(1000.0);
    rightStepper.setMaxSpeed(20000.0);
    rightStepper.setAcceleration(1000.0);
    auger.attach(augerPin);
    // Inform Pi that the Arduino is online and ready to interpret signals
    Serial.println("<CASPR Arduino Online>");
}

// Controls how servos and stepper motors update their speed:
void loop() {
  // Start monitoring uptime
  curMillis = millis();
  // Checking to see if pi has any new signals
  getDataFromPi();
  // Update motors to work with the new signal (if there is one)
  updateMotors();
  // Respond to the pi to verify that the message was received
  replyToPi();
  // Move the servo to work with the new signal (if there is one)
  moveServo();
}

void getDataFromPi() {

  // receive data from pi and save it into inputBuffer
  if (Serial.available() > 0) {

    char x = Serial.read();

    // the order of these IF clauses is significant
    // looking at start and end markers to determine signal payload
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
  // Tell pi that the message was received
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

// this illustrates using different inputs from the pi to call different functions
void updateMotors() {
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
    return;
  }
}

// Move the servo if a command was received
void moveServo() {
  float newAugerSpeed = newLeftStepperSpeed / 100.0 * 180.0;    // Defaulting to newLeftStepperSpeed because it is the first input from the pi
  augerSpeed = newAugerSpeed;
  auger.write(augerSpeed);
}

// Move the left stepper motor if a command was received
void moveLeftStepper() {
  if (newLeftStepperSpeed != 0) {
    leftStepperSpeed = -newLeftStepperSpeed;
    leftStepper.setSpeed(leftStepperSpeed);
    leftStepper.runSpeed();
  }
}

// Move the right stepper motor if a command was received
void moveRightStepper() {
  if (newRightStepperSpeed != 0) {
    rightStepperSpeed = -newRightStepperSpeed;
    rightStepper.setSpeed(rightStepperSpeed);
    rightStepper.runSpeed();

  }
}
