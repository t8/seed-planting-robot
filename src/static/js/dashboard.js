var speedSlider = document.getElementById("speedController");
var outputSpeed = document.getElementById("outputSpeed");

var directionSlider = document.getElementById("directionControllerX");
var outputDirectionX = document.getElementById("outputDirectionX");

var seedOutputRateSlider = document.getElementById("augerSpeedController");
var outputAugerSpeed = document.getElementById("outputAugerSpeed");
var outputSeedCount = document.getElementById("seedCount");

var haltButton = document.getElementById("haltBtn");

outputSpeed.innerHTML = speedSlider.value; // Display the default slider value
outputDirectionX.innerHTML = directionSlider.value;

$(document).ready(function() {
    let socket = io.connect('http://' + document.domain + ':' + location.port);
    let waitingForResponse = false;
    let halted = false;
    let seedCount = 0;
    
    setInterval(function() {
        socket.emit('checkSeedCount');
    }, 1000);
    
    socket.on('response', function(msg) {
        console.log("Received Confirmation");
        waitingForResponse = false;
        toggleInputs();
    });
    
    socket.on('seed', function(msg) {
        let currentNumberOfSeeds = parseInt(msg.response);
        if (currentNumberOfSeeds > seedCount) {
            seedCount = currentNumberOfSeeds;
            outputSeedCount.innerText = seedCount.toString();
        }
    });

    speedSlider.oninput = function() {
        outputSpeed.innerHTML = this.value;
        updateMotors();
        waitingForResponse = true;
        toggleInputs();
    };

    directionSlider.oninput = function() {
        outputDirectionX.innerHTML = this.value;
        updateMotors();
        waitingForResponse = true;
        toggleInputs();
    };

    seedOutputRateSlider.oninput = function() {
        outputAugerSpeed.innerHTML = this.value;
        updateMotors();
        waitingForResponse = true;
        toggleInputs();
    };

    haltButton.onclick = function () {
        if (halted) {
            halted = false;
            updateMotors();
            waitingForResponse = true;
            haltButton.classList.remove("is-danger");
            haltButton.classList.add("is-warning");
            haltButton.innerText = "HALT";
            speedSlider.disabled = false;
            directionSlider.disabled = false;
        } else {
            halted = true;
            halt();
            waitingForResponse = true;
            haltButton.classList.add("is-danger");
            haltButton.classList.remove("is-warning");
            haltButton.innerText = "RESUME";
            speedSlider.disabled = true;
            directionSlider.disabled = true;
        }
    };

    function updateMotors() {
        console.log("sending motor instructions");
        socket.emit('update',
        {
            stepperSpeed: speedSlider.value,
            stepperAngle: directionSlider.value,
            augerSpeed: seedOutputRateSlider.value
        });
    }

    function halt() {
        socket.emit('halt');
    }
    
    function toggleInputs() {
        if (waitingForResponse === true) {
            speedSlider.disabled = true;
            directionSlider.disabled = true;
            seedOutputRateSlider.disabled = true;
        } else {
            speedSlider.disabled = false;
            directionSlider.disabled = false;
            seedOutputRateSlider.disabled = false;
        }
    }
});