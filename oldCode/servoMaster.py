#!/usr/bin/env python
#
#  Pan Tilt Servo Control
#  Execute with parameter ==> sudo python3 servoCtrl.py <pan_angle> <tilt_angle>
#
#  MJRoBot.org 01Feb18

from time import sleep
import RPi.GPIO as GPIO
import math

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

pan = 27  # blue => PAN
tilt = 17  # green => TILT
rotate = 22  # yellow => ROTATE

GPIO.setup(tilt, GPIO.OUT)
GPIO.setup(pan, GPIO.OUT)
GPIO.setup(rotate, GPIO.OUT)


def setServoAngle(servo, angle, duration=0.5):
    pwm = GPIO.PWM(servo, 50)
    pwm.start(8)
    # dutyCycle = angle
    dutyCycle = angle / 18. + 3.
    # dutyCycle = angle * 5. / 9.
    pwm.ChangeDutyCycle(dutyCycle)
    sleep(duration)
    pwm.stop()


def controlHorizonal(direction, length):
    r = 8
    angle = ((length * 1000) * math.pi) / (r * 180)
    print(angle)
    if direction == "left":
        for i in range(0, int(angle), 1):
            setServoAngle(pan, i)
            setServoAngle(rotate, -i)
    elif direction == "right":
        for i in range(0, int(angle), 1):
            setServoAngle(pan, -i)
            setServoAngle(rotate, i)


if __name__ == '__main__':
    for i in range(90, 160, 15):
        setServoAngle(pan, i, 100)
        setServoAngle(tilt, i)
        setServoAngle(rotate, i)
        print("MISSSYYYYY")

    for i in range(150, 30, -15):
        setServoAngle(pan, i, 10)
        setServoAngle(tilt, i)
        setServoAngle(rotate, i)
        print("LEAHHHHHH")

    setServoAngle(pan, 100)
    setServoAngle(tilt, 90)
    setServoAngle(rotate, 80)
#

# setServoAngle(tilt, 90)
# setServoAngle(pan, 90)
# setServoAngle(rotate, 90)

# controlHorizonal("left", 1)
GPIO.cleanup()
