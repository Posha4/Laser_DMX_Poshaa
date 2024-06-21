var connection;

window.onload = function() {
    connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
    connection.onopen = function () {
        document.getElementById('ws-status').style = 'background-color: green;';
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
            case '!':
                break;
            case '?':
                initPads(value);
                break;
            default:
                console.log('Invalid data');
        }
    };
    connection.onclose = function () {
        console.log('WebSocket connection closed');
        document.getElementById('ws-status').style = 'background-color: red;';
    };
};

function navigate(page){
    switch(page){
        case 'sliders':
            document.getElementById('sliders-container').style.display = 'block';
            document.getElementById('pads-container').style.display = 'none';
            break;
        case 'pads':
            document.getElementById('sliders-container').style.display = 'none';
            document.getElementById('pads-container').style.display = 'block';
            break;
        default:
            console.log('Invalid page');
    }
}

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

function onSliderChange(id, html_id) {
    slidersData.find(slider => slider.id === id).value = document.getElementById(html_id ?? id).value;

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
        visible: false,
        type: 'range'
    },
    {
        id: 'chan1',
        value: 0,
        name : 'Mode',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan2',
        value: 0,
        name: 'Figs',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan3',
        value: 0,
        name: 'Zoom',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan4',
        value: 0,
        name: 'Xond',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan5',
        value: 0,
        name: 'Yond',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan6',
        value: 0,
        name: 'Zond',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan7',
        value: 0,
        name: 'Xmov',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan8',
        value: 0,
        name: 'Ymov',
        visible: true,
        type: 'range'
    },
    {
        id: 'chan9',
        value: 0,
        name: 'Color',
        visible: true,
        type: 'radio',
        options : [
            {
                value: 30,
                name: 'Multicolor rainbow'
            },
            {
                value: 80,
                name: 'Red'
            },
            {
                value: 100,
                name: 'Green'
            },
            {
                value: 130,
                name: 'Blue'
            },
            {
                value: 170,
                name: 'Yellow / Green'
            },
            {
                value: 200,
                name: 'Violet'
            },
            {
                value: 230,
                name: 'Light blue'
            }
        ]
    }
];