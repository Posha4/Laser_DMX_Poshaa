
var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
};
id_array = new Array("addr", "chan1", "chan2", "chan3", "chan4", "chan5", "chan6", "chan7", "chan8", "chan9");
values = new Array(id_array.length);

function prepareVar(id, position) {
    for (i = 0; i < id_array.length; i++) {
        var a = parseInt(document.getElementById(id_array[i]).value).toString(16);
        if (a.length < 2) {
            a = '0' + a;
        }
        values[i] = a;
    }
    sendVars();
}

function blackoutToogle() {
    var action = document.querySelector(".tgl-btn");
    action.classList.toggle("active");

    if (action.classList.contains("active")) {
        var data = '#01,00,00,00,00,00,00,00,00,00';
        console.log('Data: ' + data);
        connection.send(data);
        console.log("Blackout");
    } else {
        // annyang.abort();
        button.value = "OFF";
        prepareVar('chan1', 1);
        console.log("Stopped");
    }
}


function sendVars() {
    var data = '#' + values;
    console.log('Data: ' + data);
    connection.send(data);
}