# serial communication

import time
import serial

ser = serial.Serial(
    port='/dev/ttyACM0',  # Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
# counter = 0
s = [0]

# while 1:
#     # ser.write("Write counter: %d \n" % counter)
#     # time.sleep(1)
#     # counter += 1
#     if ser.in_waiting > 0:
#         line = ser.readLine()
#         print(line)

while True:
    read_serial = ser.readline()
    s[0] = str(ser.readline())
    print(s[0])
    print read_serial
