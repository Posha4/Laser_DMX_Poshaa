
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

function onSliderChange(id, position) {
    if(position > 0)
        slidersData[position - 1].value = document.getElementById(id_array[position]).value

    if(document.getElementById('auto-apply-checkbox').checked)
        sendData()
}

function sendData(){
    for (i = 0; i < id_array.length; i++) {
        var a = parseInt(document.getElementById(id_array[i]).value).toString(16);
        if (a.length < 2) {
            a = '0' + a;
        }
        values[i] = a;
    }
    sendVars();
}

function sendVars(id) {
    var data = '#' + values;
    console.log('Data: ' + data);
    connection.send(data);
}

var slidersData = [
    {
        id: 'chan1',
        value: 0,
        name : 'Mode'
    },
    {
        id: 'chan2',
        value: 0,
        name: 'Figs'
    },
    {
        id: 'chan3',
        value: 0,
        name: 'Zoom'
    },
    {
        id: 'chan4',
        value: 0,
        name: 'Xond'
    },
    {
        id: 'chan5',
        value: 0,
        name: 'Yond'
    },
    {
        id: 'chan6',
        value: 0,
        name: 'Zond'
    },
    {
        id: 'chan7',
        value: 0,
        name: 'Xmov'
    },
    {
        id: 'chan8',
        value: 0,
        name: 'Ymov'
    },
    {
        id: 'chan9',
        value: 0,
        name: 'Color'
    }
];

function savePreset(){
    var preset = [];
    for(var i = 0; i < slidersData.length; i++){
        preset.push(slidersData[i].value);
    }
    console.log('Preset: ' + preset);
}