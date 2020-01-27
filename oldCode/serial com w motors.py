import serial
ser = serial.Serial('/dev/ttyUSB0', 9600)

def turnonleftmotor:
    ser.write("turn on left motor")
def turnonrightmotor:
    ser.write("turn on right motor")
def turnoffleftmotor:
    ser.write("turn off left motor")
def turnoffrightmotor:
    ser.write("turn off right motor")
def turnonallmotors:
    ser.write("turn on all motors")
def turnoffallmotors:
    ser.write("turn off all motors")

