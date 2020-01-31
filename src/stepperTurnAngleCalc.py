# The purpose of this code is to utilize a custom-made sine algorithm to determine which stepper motors need to turn, and to what speed

import math

def calculateStepperSpeedsAtAngle(baseSpeed, angle):
    leftStepperSpeed = 0
    rightStepperSpeed = 0
    radAngle = math.radians(angle)
    sinDeg = math.sin(radAngle)
    if angle < 90:
        leftStepperSpeed = (-1 * baseSpeed / 2 * sinDeg) + baseSpeed
        rightStepperSpeed = baseSpeed / 2 * sinDeg
    elif angle > 90:
        leftStepperSpeed = baseSpeed / 2 * sinDeg
        rightStepperSpeed = (-1 * baseSpeed / 2 * sinDeg) + baseSpeed
    elif angle == 90:
        leftStepperSpeed = baseSpeed
        rightStepperSpeed = baseSpeed
        
    return round(leftStepperSpeed), round(rightStepperSpeed)


#print(calculateStepperSpeedsAtAngle(4000, 130))