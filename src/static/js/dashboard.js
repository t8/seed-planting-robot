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
    let waitingForResponse = false;
    let halted = false;
    
    socket.on('response', function(msg) {
        console.log("Received Confirmation");
        waitingForResponse = false;
        toggleInputs();
    });

    speedSlider.oninput = function() {
        outputSpeed.innerHTML = this.value;
        updateStepperSpeed();
        waitingForResponse = true;
        toggleInputs();
    };

    directionSlider.oninput = function() {
        outputDirectionX.innerHTML = this.value;
        updateStepperSpeed();
        waitingForResponse = true;
        toggleInputs();
    };

    haltButton.onclick = function () {
        if (halted) {
            halted = false;
            updateStepperSpeed();
            waitingForResponse = true;
            haltButton.classList.remove("is-danger");
            haltButton.classList.add("is-warning");
            haltButton.innerHTML = "HALT";
            speedSlider.disabled = false;
            directionSlider.disabled = false;
        } else {
            halted = true;
            halt();
            waitingForResponse = true;
            haltButton.classList.add("is-danger");
            haltButton.classList.remove("is-warning");
            haltButton.innerHTML = "RESUME";
            speedSlider.disabled = true;
            directionSlider.disabled = true;
        }
    };

    function updateStepperSpeed() {
        console.log("sending motor instructions");
        socket.emit('updateSteppers',
        {
            stepperSpeed: speedSlider.value,
            stepperAngle: directionSlider.value
        });
    }

    function halt() {
        socket.emit('halt');
    }
    
    function toggleInputs() {
        if (waitingForResponse = true) {
            speedSlider.disabled = true;
            directionSlider.disabled = true;
        } else {
            speedSlider.disabled = false;
            directionSlider.disabled = false;
        }
    }
});