const padContainer = document.getElementById('pad-container');
const padTemplate = document.getElementById('pad-template').content;

const colors = ["AliceBlue", "AntiqueWhite", "Aqua", "Aquamarine", "Azure", "Beige", "Bisque", "Black", "BlanchedAlmond", "Blue", "BlueViolet", "Brown", "BurlyWood", "CadetBlue", "Chartreuse", "Chocolate", "Coral", "CornflowerBlue", "Cornsilk", "Crimson", "Cyan",  "DarkBlue", "DarkCyan", "DarkGoldenRod", "DarkGray", "DarkGrey", "DarkGreen", "DarkKhaki", "DarkMagenta", "DarkOliveGreen", "Darkorange", "DarkOrchid", "DarkRed", "DarkSalmon", "DarkSeaGreen", "DarkSlateBlue", "DarkSlateGray", "DarkSlateGrey", "DarkTurquoise", "DarkViolet", "DeepPink", "DeepSkyBlue", "DimGray", "DimGrey", "DodgerBlue", "FireBrick", "FloralWhite", "ForestGreen", "Fuchsia", "Gainsboro", "GhostWhite", "Gold", "GoldenRod", "Gray", "Grey", "Green", "GreenYellow", "HoneyDew", "HotPink", "IndianRed", "Indigo", "Ivory", "Khaki", "Lavender", "LavenderBlush", "LawnGreen", "LemonChiffon", "LightBlue", "LightCoral", "LightCyan", "LightGoldenRodYellow", "LightGray", "LightGrey", "LightGreen", "LightPink", "LightSalmon", "LightSeaGreen", "LightSkyBlue", "LightSlateGray", "LightSlateGrey", "LightSteelBlue", "LightYellow", "Lime", "LimeGreen", "Linen", "Magenta", "Maroon", "MediumAquaMarine", "MediumBlue", "MediumOrchid", "MediumPurple", "MediumSeaGreen", "MediumSlateBlue", "MediumSpringGreen", "MediumTurquoise", "MediumVioletRed", "MidnightBlue", "MintCream", "MistyRose", "Moccasin", "NavajoWhite", "Navy", "OldLace", "Olive", "OliveDrab", "Orange", "OrangeRed", "Orchid", "PaleGoldenRod", "PaleGreen", "PaleTurquoise", "PaleVioletRed", "PapayaWhip", "PeachPuff", "Peru", "Pink", "Plum", "PowderBlue", "Purple", "RebeccaPurple", "Red", "RosyBrown", "RoyalBlue", "SaddleBrown", "Salmon", "SandyBrown", "SeaGreen", "SeaShell", "Sienna", "Silver", "SkyBlue", "SlateBlue", "SlateGray", "SlateGrey", "Snow", "SpringGreen", "SteelBlue", "Tan", "Teal", "Thistle", "Tomato", "Turquoise", "Violet", "Wheat", "WhiteSmoke", "Yellow", "YellowGreen"];

var pads = [];

function savePreset(){
    var preset = [];
    for(var i = 0; i < slidersData.length; i++){
        preset.push(slidersData[i].value);
    }
    createPad(preset);
}

function createPad(preset){

    if(pads.length >= 30){
        alert('Max number of presets reached (30).');
        return;
    }

    let name = prompt("Enter the preset name:");
    if (name == null || name == "") {
        return;
    }

    const id = makeid(20);
    _createPad(id, preset, name, colors[Math.floor(Math.random() * colors.length)]);

    saveData();
}

function _createPad(id, values, name, color){
    var pad = {
        id: id,
        values: values,
        name: name,
        color: color
    };

    pads.push(pad);
    
    const clone = document.importNode(padTemplate, true);

    clone.querySelector('.pad').setAttribute('id', id);

    clone.querySelector('.pad-body').style.backgroundColor = color;
    clone.querySelector('.pad-body').setAttribute('onclick', `onPadClick('${id}')`);

    clone.querySelector("button[name='deletePreset']").setAttribute('onclick', `deletePreset('${id}')`);
    clone.querySelector("button[name='changeColorPreset']").setAttribute('onclick', `changeColorPreset('${id}')`);

    clone.querySelector('.pad-name').textContent = name;

    padContainer.appendChild(clone);
}

function onPadClick(id){
    const pad = pads.find(pad => pad.id === id);
    const values = [];

    pad.values.forEach((value, index) => {
        values.push({
            id: slidersData[index].id,
            value: value
        });
    })
    sendData(values);
}

function deletePreset(id){
    const pad = pads.find(pad => pad.id === id);
    const index = pads.indexOf(pad);
    pads.splice(index, 1);

    saveData();
}

function changeColorPreset(id){
    const pad = pads.find(pad => pad.id === id);
    pad.color = colors[Math.floor(Math.random() * colors.length)];

    saveData();
}

function initPads(json){
    if(json === ''){
        return;
    }

    pads = [];
    padContainer.innerHTML = '';

    const jsonParsed = JSON.parse(json);
    jsonParsed.forEach(pad => {
        _createPad(pad.id, pad.values, pad.name, pad.color);
    });
    toggleEditPad(true);
}

var showEditPad = false;
function toggleEditPad(keep = false){

    if((showEditPad && !keep) || (keep && !showEditPad)){
        document.querySelectorAll('.pad .pad-actions').forEach(element => {
            element.style.display = 'none';
        })
    } else {
        document.querySelectorAll('.pad .pad-actions').forEach(element => {
            element.style.display = 'flex';
        })
    }

    if(!keep)
        showEditPad = !showEditPad;
}