var connection;

window.onload = function() {
    connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
    connection.onopen = function () {
        connection.send('Connect ' + new Date());
    };
    connection.onerror = function (error) {
        console.log('WebSocket Error ', error);
    };
    connection.onmessage = function (e) {
        console.log('Server: ', e.data);

        const id = e.data.substring(0, 1);
        const value = e.data.substring(1, e.data.length);

        switch (id) {
            case '?':
                initPads(value);
                break;
            default:
                console.log('Invalid data');
        }
    };
};

function makeid(length) {
    let result = '';
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    const charactersLength = characters.length;
    let counter = 0;
    while (counter < length) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
      counter += 1;
    }
    return result;
}

function onSliderChange(id) {
    slidersData.find(slider => slider.id === id).value = document.getElementById(id).value;

    if(document.getElementById('auto-apply-checkbox').checked){
        sendData(slidersData)
    }
}

function sendCurrentData(){
    sendData(slidersData);
}

function sendData(data){
    var values = [];
    for (i = 0; i < data.length; i++) {
        var a = parseInt(data[i].value).toString(16);
        if (a.length < 2) {
            a = '0' + a;
        }
        values[i] = a;
    }
    sendVars(values);
}

function sendVars(values) {
    var data = '#' + values;
    console.log('Data: ' + data);
    connection.send(data);
}

var slidersData = [
    {
        id: 'addr',
        value: 0,
        name: 'Address',
        visible: false
    },
    {
        id: 'chan1',
        value: 0,
        name : 'Mode',
        visible: true
    },
    {
        id: 'chan2',
        value: 0,
        name: 'Figs',
        visible: true
    },
    {
        id: 'chan3',
        value: 0,
        name: 'Zoom',
        visible: true
    },
    {
        id: 'chan4',
        value: 0,
        name: 'Xond',
        visible: true
    },
    {
        id: 'chan5',
        value: 0,
        name: 'Yond',
        visible: true
    },
    {
        id: 'chan6',
        value: 0,
        name: 'Zond',
        visible: true
    },
    {
        id: 'chan7',
        value: 0,
        name: 'Xmov',
        visible: true
    },
    {
        id: 'chan8',
        value: 0,
        name: 'Ymov',
        visible: true
    },
    {
        id: 'chan9',
        value: 0,
        name: 'Color',
        visible: true
    }
];