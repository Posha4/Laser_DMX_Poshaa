function loadData(){
    const data = '?'
    console.log('Data: ' + data);
    connection.send(data);
}

function saveData(){
    const dataToJson = JSON.stringify(pads);

    const data = '!' + dataToJson
    console.log('Data: ' + data);
    connection.send(data);
}