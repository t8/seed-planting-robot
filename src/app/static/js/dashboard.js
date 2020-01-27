var speedSlider = document.getElementById("speedController");
var outputSpeed = document.getElementById("outputSpeed");

var directionSlider = document.getElementById("directionControllerX");
var outputDirectionX = document.getElementById("outputDirectionX");

var directionSwitch = document.getElementById("directionControllerY");
var outputDirectionY = document.getElementById("outputDirectionY");

outputSpeed.innerHTML = speedSlider.value; // Display the default slider value
outputDirectionX.innerHTML = directionSlider.value;

$(document).ready(function() {
    var socket = io.connect('http://' + document.domain + ':' + location.port);
    
    socket.on('response', function(msg) {
        console.log("Received Confirmation");
    });
    // Update the current slider value (each time you drag the slider handle)
    speedSlider.oninput = function() {
      outputSpeed.innerHTML = this.value;
        updateSettings();
    }

    directionSlider.oninput = function() {
      outputDirectionX.innerHTML = this.value;
        updateSettings();
    }

    function updateSettings() {
        socket.emit('updateSettings',
        {
            stepperSpeed: speedSlider.value,
            stepperDirection: directionSlider.value
        });
    });


// Client Side Javascript to receive numbers.
//$(document).ready(function(){
//    // start up the SocketIO connection to the server - the namespace 'test' is also included here if necessary
//    var socket = io.connect('http://' + document.domain + ':' + location.port + '/test');
//    // this is a callback that triggers when the "my response" event is emitted by the server.
//    socket.on('my response', function(msg) {
//        $('#log').append('<p>Received: ' + msg.data + '</p>');
//    });
//    //example of triggering an event on click of a form submit button
//    $('form#emit').submit(function(event) {
//        socket.emit('my event', {data: $('#emit_data').val()});
//        return false;
//    });
//});

//directionSwitch.onchange = function() {
//    console.log("Clicked");
//    if(directionSwitch.checked) {
//        outputDirectionY = "Forwards";
//    } else {
//        outputDirectionY = "Backwards";   
//    }
//}