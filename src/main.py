import json
import serial
from flask import Flask, render_template, make_response
from functools import wraps, update_wrapper
from datetime import datetime
from flask_socketio import SocketIO, emit
import stepperTurnAngleCalc as stac


def sendToArduino(sendStr):
    ser.write(sendStr.encode('ascii'))


# ============================


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


def sendCommands(td):
  numLoops = len(td)
  waitingForReply = False

  n = 0
  while n < numLoops:

    commandString = td[n]

    if waitingForReply == False:
      sendToArduino(commandString)
      print "Sent from PI -- LOOP NUM " + str(n) + " STR " + commandString
      waitingForReply = True

    if waitingForReply == True:

      while ser.inWaiting() == 0:
        pass
        
      dataRecvd = recvFromArduino()
      print "Reply Received  " + dataRecvd
      n += 1
      waitingForReply = False

      print "==========="


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


# ============================
        
        
app = Flask(__name__)
socketio = SocketIO(app)

global stepperSpeed
global stepperDirection


def parseSocketData(data):
    parsedData = json.loads(json.dumps(eval(json.dumps(data))))
    print(parsedData)
    return parsedData    


@socketio.on('updateSteppers')  # Decorator to catch an event called "updateStepperSpeed":
def receive(data):  # receive() is the event callback function.
    fixedData = parseSocketData(data)
    newSpeed = fixedData['stepperSpeed']
    newAngle = fixedData['stepperAngle']
    leftStepperSpeed, rightStepperSpeed = stac.calculateStepperSpeedsAtAngle(float(newSpeed), float(newAngle))
    command = "<B," + str(leftStepperSpeed) + "," + str(rightStepperSpeed) + ">"
    sendCommands([command])
    emit('response', {'data': 'updated steppers'})  # Trigger a new event called "response"

    
@socketio.on('updateTurn')
def updateTurn(data):
    print("UPDATING TURN")
    fixedData = parseSocketData(data)
    speed = fixedData['stepperSpeed']
    angle = fixedData['stepperAngle']
    leftStepperSpeed, rightStepperSpeed = stac.calculateStepperSpeedsAtAngle(float(speed), float(angle))
    print(leftStepperSpeed)
    str1 = ("<LEFTSTEP," + str(leftStepperSpeed) + ",FOR>")
    str2 = ("<RIGHTSTEP," + str(rightStepperSpeed) + ",FOR>")
    commands = [str1, str2]
    sendCommands(commands)
    emit('response', {'data': 'updated stepper speed'})  # Trigger a new event called "response"
    

@socketio.on('halt')
def haltEverything():
    sendCommands(["<H,0,0>"])
    emit('response', {'data': 'halting now'})


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
    ser = serial.Serial(serPort, baudRate)
    "Serial port " + serPort + " opened  Baudrate " + str(baudRate)

    startMarker = 60
    endMarker = 62

    waitForArduino()
    socketio.run(app, host='0.0.0.0', port=80)