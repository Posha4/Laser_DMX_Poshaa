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
    const id = makeid(20);
    _createPad(id, preset, colors[Math.floor(Math.random() * colors.length)]);
}

function _createPad(id, values, color){
    var pad = {
        id: id,
        values: values,
        color: color
    };

    pads.push(pad);
    
    const clone = document.importNode(padTemplate, true);

    clone.querySelector('.pad').setAttribute('id', id);

    clone.querySelector('.pad-body').style.backgroundColor = color;
    clone.querySelector('.pad-body').setAttribute('onclick', `onPadClick('${id}')`);

    clone.querySelector("button[name='deletePreset']").setAttribute('onclick', `deletePreset('${id}')`);
    clone.querySelector("button[name='changeColorPreset']").setAttribute('onclick', `changeColorPreset('${id}')`);

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
    document.getElementById(id).remove();
}

function changeColorPreset(id){
    const pad = pads.find(pad => pad.id === id);
    pad.color = colors[Math.floor(Math.random() * colors.length)];
    document.getElementById(id).querySelector('.pad-body').style.backgroundColor = pad.color;
}

function initPads(json){
    if(json === ''){
        return;
    }

    pads = [];
    padContainer.innerHTML = '';

    const jsonParsed = JSON.parse(json);
    jsonParsed.forEach(pad => {
        _createPad(pad.id, pad.values, pad.color);
    });
    
}