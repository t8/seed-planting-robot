#include <Servo.h>

/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */
// Define motor connections:

#define leftStepperDirPin 6
#define leftStepperStepPin 7

#define rightStepperDirPin 8
#define rightStepperStepPin 9

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
float motorSpeedFraction = 0.0; // fraction of motor velocity to move
char dir[buffSize] = {0};


unsigned long curMillis;

unsigned long prevReplyToPimillis = 0;
unsigned long replyToPiinterval = 1000;

// Motor Settings
byte leftStepperSpeed;
byte rightStepperSpeed;
const byte minStepperSpeed = 2000;
const byte maxStepperSpeed = 500;

// Servo settings
Servo auger;
byte augerSpeed = 90;
byte augerMin = 0;
byte augerMax = 180;
byte newAugerSpeed = augerMin;

void setup() {
  Serial.begin(9600);

  // Declare pins as output:
  pinMode(leftStepperStepPin, OUTPUT);
  pinMode(leftStepperDirPin, OUTPUT);

  pinMode(rightStepperStepPin, OUTPUT);
  pinMode(rightStepperDirPin, OUTPUT);

  auger.attach(augerPin);
  Serial.println("<CASPR Arduino Online>");
}

void loop() {
//  Serial.println("yo mama");
//  delay(50);
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
  motorSpeedFraction = atof(strtokIndx);     // convert this part to an float

  strtokIndx = strtok(NULL, ",");
  strcpy(dir, strtokIndx);     // convert this part to a string

}

void replyToPi() {

  if (newDataFromPi) {
    newDataFromPi = false;
    Serial.print("<Msg ");
    Serial.print(messageFromPi);
    Serial.print(" Speed ");
    Serial.print(motorSpeedFraction);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

void updateMotors() {
  // this illustrates using different inputs to call different functions
  if (strcmp(messageFromPi, "LEFTSTEP") == 0) {
    if(strcmp(dir, "FOR") == 0) {
      // Set the spinning direction CW/CCW:
      digitalWrite(leftStepperDirPin, HIGH);
    } else if (strcmp(dir, "BCK") == 0) {
      digitalWrite(leftStepperDirPin, LOW);
    }
    moveLeftStepper();
  } else if (strcmp(messageFromPi, "RIGHTSTEP") == 0) {
    if(strcmp(dir, "FOR") == 0) {
      // Set the spinning direction CW/CCW:
      digitalWrite(rightStepperDirPin, HIGH);
    } else if (strcmp(dir, "BCK") == 0) {
      digitalWrite(rightStepperDirPin, LOW);
    }
    moveRightStepper();
  } else if (strcmp(messageFromPi, "AUGER") == 0) {
    moveServo();
  } else if (strcmp(messageFromPi, "BOTHSTEP") == 0) {
    if(strcmp(dir, "FOR") == 0) {
      // Set the spinning direction CW/CCW:
      digitalWrite(leftStepperDirPin, HIGH);
      digitalWrite(rightStepperDirPin, HIGH);
    } else if (strcmp(dir, "BCK") == 0) {
      digitalWrite(leftStepperDirPin, LOW);
      digitalWrite(rightStepperDirPin, LOW);
    }
    moveLeftStepper();
    moveRightStepper();
  } else if (strcmp(messageFromPi, "HALT") == 0) {
    moveLeftStepper();
    moveRightStepper();
    moveServo();
  }
}


void moveServo() {
  float newAugerSpeed = motorSpeedFraction * 90.0;
  if (augerSpeed != newAugerSpeed && newAugerSpeed >= 0.0 && newAugerSpeed <= 2.0) {
    augerSpeed = newAugerSpeed;
    auger.write(augerSpeed);
  }
}

void moveLeftStepper() {
  if (motorSpeedFraction != 0) {
    float newStepperSpeed = motorSpeedFraction;
    if (newStepperSpeed != leftStepperSpeed) {
      leftStepperSpeed = newStepperSpeed;
    }
    digitalWrite(leftStepperStepPin, HIGH);
    delayMicroseconds(leftStepperSpeed);
    digitalWrite(leftStepperStepPin, LOW);
    delayMicroseconds(leftStepperSpeed);
  }
}

void moveRightStepper() {
  if (motorSpeedFraction != 0) {
    float newStepperSpeed = motorSpeedFraction;
    if (newStepperSpeed != rightStepperSpeed) {
      rightStepperSpeed = newStepperSpeed;
    }
    digitalWrite(rightStepperStepPin, HIGH);
    delayMicroseconds(rightStepperSpeed);
    digitalWrite(rightStepperStepPin, LOW);
    delayMicroseconds(rightStepperSpeed);
  }
}
