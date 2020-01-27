import os
import serial
import time
import subprocess
import threading
import time
import json
from flask import Flask, request, render_template, make_response
from functools import wraps, update_wrapper
from datetime import datetime
from flask_socketio import SocketIO, emit

#=====================================

def sendToArduino(sendStr):
  ser.write(sendStr)


#======================================

def recvFromArduino():
  global startMarker, endMarker
  
  ck = ""
  x = "z" # any value that is not an end- or startMarker
  byteCount = -1 # to allow for the fact that the last increment will be one too many
  
  # wait for the start character
  while  ord(x) != startMarker: 
    x = ser.read()
  
  # save data until the end marker is found
  while ord(x) != endMarker:
    if ord(x) != startMarker:
      ck = ck + x 
      byteCount += 1
    x = ser.read()
  
  return(ck)


#============================

def waitForArduino():

   # wait until the Arduino sends 'Arduino Ready' - allows time for Arduino reset
   # it also ensures that any bytes left over from a previous message are discarded
   
    global startMarker, endMarker
    
    msg = ""
    while msg.find("CASPR Arduino Online") == -1:

      while ser.inWaiting() == 0:
        pass
        
      msg = recvFromArduino()

      print msg
      print
      
#======================================

def runTest(td):
  numLoops = len(td)
  waitingForReply = False

  n = 0
  while n < numLoops:

    teststr = td[n]

    if waitingForReply == False:
      sendToArduino(teststr)
      print "Sent from PC -- LOOP NUM " + str(n) + " TEST STR " + teststr
      waitingForReply = True

    if waitingForReply == True:

      while ser.inWaiting() == 0:
        pass
        
      dataRecvd = recvFromArduino()
      print "Reply Received  " + dataRecvd
      n += 1
      waitingForReply = False

      print "==========="

    time.sleep(5)


#======================================

# THE PROGRAM STARTS HERE

#======================================

app = Flask(__name__)
socketio = SocketIO(app)

global stepperSpeed;
global stepperDirection;

@socketio.on('updateSettings')                          # Decorator to catch an event called "my event":
def receive(data):                        # test_message() is the event callback function.
#    print(map(lambda x: x.encode('utf-8'), data))
    fixedData = json.dumps(eval(json.dumps(data)))
    print(fixedData.data.stepperSpeed)
    #    testdata = []
#    testdata.append("<BOTHSTEP," + data['stepperSpeed'] + ",FOR>")
#    runTest(testdata)
    #emit('response', {'data': 'got it!'})      # Trigger a new event called "my response" 

#@app.before_first_request
#def activate_job():
#    def run_job():
#        while True:
#            refreshControls()
#            print("refreshing controls")
#            time.sleep(3)
#
#    thread = threading.Thread(target=run_job)
#    thread.start()

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
    return render_template('dashboard.html', speed = 0, direction = 90)


if __name__ == '__main__':
#    app.run(host='0.0.0.0', port=80, debug=True)
    socketio.run(app)
    # NOTE the user must ensure that the serial port and baudrate are correct
#    serPort = "/dev/ttyACM0"
#    baudRate = 9600
#    ser = serial.Serial(serPort, baudRate)
#    print "Serial port " + serPort + " opened  Baudrate " + str(baudRate)
#    startMarker = 60
#    endMarker = 62
#    waitForArduino()
#
#
#    testData = []
#    testData.append("<LEFTSTEP,3000.0,FOR>")
#    testData.append("<RIGHTSTEP,3000.0,FOR>")
#    testData.append("<HALT,0,ARB>")
#    testData.append("<BOTHSTEP,3000.0,BCK>")
#
#    runTest(testData)
#
#
#    ser.close