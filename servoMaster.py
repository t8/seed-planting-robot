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


def setServoAngle(servo, angle):
    assert 30 <= angle <= 150
    pwm = GPIO.PWM(servo, 50)
    pwm.start(8)
    dutyCycle = angle / 18. + 3.
    pwm.ChangeDutyCycle(dutyCycle)
    sleep(0.5)
    pwm.stop()


def controlHorizonal(direction, length):
    r = 1
    angle = (math.pi * r * length) / 180
    if direction == "left":
        setServoAngle(pan, angle)
        setServoAngle(tilt, -angle)
    elif direction == "right":
        setServoAngle(pan, -angle)
        setServoAngle(tilt, angle)


# if __name__ == '__main__':
#     for i in range(30, 160, 15):
#         setServoAngle(pan, i)
#         setServoAngle(tilt, i)
#         setServoAngle(rotate, i)
#         print("MISSSYYYYY")
#
#     for i in range(150, 30, -15):
#         setServoAngle(pan, i)
#         setServoAngle(tilt, i)
#         setServoAngle(rotate, i)
#         print("LEAHHHHHH")
#
#     setServoAngle(pan, 100)
#     setServoAngle(tilt, 90)
#     setServoAngle(rotate, 80)

controlHorizonal("left", 2)
GPIO.cleanup()
