import os
import time
import json
from flask import Flask, render_template, make_response
from functools import wraps, update_wrapper
from datetime import datetime
from flask_socketio import SocketIO, emit
# from serialCommunicationCommands import * as sCommands
import serial


def sendToArduino(sendStr):
    ser.write(sendStr)


# ======================================

def recvFromArduino():
    global startMarker, endMarker

    ck = ""
    x = "z"  # any value that is not an end- or startMarker
    byteCount = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    while ord(x) != startMarker:
        x = ser.read()

    # save data until the end marker is found
    while ord(x) != endMarker:
        if ord(x) != startMarker:
            ck = ck + x
            byteCount += 1
        x = ser.read()

    return (ck)


# ============================

def waitForArduino():
    # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded

    global startMarker, endMarker

    msg = ""
    while msg.find("CASPR Arduino Online") == -1:

        while ser.inWaiting() == 0:
            pass

        msg = recvFromArduino()
        print(msg)


app = Flask(__name__)
socketio = SocketIO(app)

global stepperSpeed
global stepperDirection


@socketio.on('updateStepperSpeed')  # Decorator to catch an event called "my event":
def receive(data):  # test_message() is the event callback function.
    #    print(map(lambda x: x.encode('utf-8'), data))
    fixedData = json.loads(json.dumps(eval(json.dumps(data))))
    newSpeed = float(fixedData['stepperSpeed'])
    print(fixedData["stepperSpeed"])
    command = "<BOTHSTEP," + str(newSpeed) + ",FOR>"
    sendToArduino(command)
    emit('response', {'data': 'got it!'})  # Trigger a new event called "my response"


@socketio.on('halt')
def haltEverything():
    sendToArduino("<HALT,0,A>")
    emit('response', {'data': 'halting now!'})


def nocache(view):
    @wraps(view)
    def no_cache(*args, **kwargs):
        response = make_response(view(*args, **kwargs))
        response.headers['Last-Modified'] = datetime.now()
        response.headers['Cache-Control'] = 'no-store, no-cache, must-revalidate, post-check=0, pre-check=0, max-age=0'
        response.headers['Pragma'] = 'no-cache'
        response.headers['Expires'] = '-1'
        return response

    return update_wrapper(no_cache, view)


@app.route('/')
@nocache
def home():
    return render_template('dashboard.html', speed=0, direction=90, seedOutput=1, seedOutputRate=2)


if __name__ == '__main__':
    serPort = "/dev/ttyACM0"
    baudRate = 9600
    SOCKET_SSL_KEY_FILE = 'key.pem'
    SOCKET_SSL_CERT_FILE = 'cert.pem'
    ser = serial.Serial(serPort, baudRate)
    "Serial port " + serPort + " opened  Baudrate " + str(baudRate)

    startMarker = 60
    endMarker = 62

    waitForArduino()
    socketio.run(app, host='0.0.0.0', port=80)