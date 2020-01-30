var speedSlider = document.getElementById("speedController");
var outputSpeed = document.getElementById("outputSpeed");

var directionSlider = document.getElementById("directionControllerX");
var outputDirectionX = document.getElementById("outputDirectionX");

var directionSwitch = document.getElementById("directionControllerY");
var outputDirectionY = document.getElementById("outputDirectionY");

var haltButton = document.getElementById("haltBtn");

outputSpeed.innerHTML = speedSlider.value; // Display the default slider value
outputDirectionX.innerHTML = directionSlider.value;

$(document).ready(function() {
    let socket = io.connect('http://' + document.domain + ':' + location.port);
    
    socket.on('response', function(msg) {
        console.log("Received Confirmation");
    });
    // Update the current slider value (each time you drag the slider handle)
    speedSlider.oninput = function() {
      outputSpeed.innerHTML = this.value;
        updateStepperSpeed();
    };

    directionSlider.oninput = function() {
      outputDirectionX.innerHTML = this.value;
        updateStepperSpeed();
    };

    haltButton.onclick = function () {
        halt();
    };

    function updateStepperSpeed() {
        console.log("sending motor instructions");
        socket.emit('updateStepperSpeed',
        {
            stepperSpeed: speedSlider.value,
            stepperDirection: directionSlider.value
        });
    }

    function halt() {
        socket.emit('halt');
    }
});