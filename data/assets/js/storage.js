function importData() {

    if(document.getElementById('file-import-data').files.length == 0){
        alert('No file selected');
        return;   
    }

    var data = document.getElementById('file-import-data').files[0];
    var reader = new FileReader();
    reader.onload = onReaderLoad;
    reader.readAsText(data);
}

function onReaderLoad(event){

    try {
        JSON.parse(event.target.result);
    } catch (e) {
        alert('Invalid file')
        return;
    }

    const data = '!' + event.target.result;
    connection.send(data);

    alert('Data imported');
}