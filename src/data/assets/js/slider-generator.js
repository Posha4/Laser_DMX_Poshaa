const sliderContainer = document.getElementById('slider-container');
const sliderTemplate = document.getElementById('slider-template').content;
const radioSliderTemplate = document.getElementById('radio-slider-template').content;

for(var i = 0; i < slidersData.length; i++) {

    if(slidersData[i].type == 'range'){
        const clone = document.importNode(sliderTemplate, true);

        clone.querySelector('.slider-name').textContent = slidersData[i].name;
    
        clone.querySelector("input[type='range']").setAttribute('id', slidersData[i].id);
        clone.querySelector("input[type='range']").setAttribute('name', slidersData[i].id);
        clone.querySelector("input[type='range']").setAttribute('onchange', `onSliderChange('${slidersData[i].id}');`);
        clone.querySelector("input[type='range']").setAttribute('oninput', `this.form.${slidersData[i].id}_in.value=this.value;onSliderChange('${slidersData[i].id}');`);
    
        clone.querySelector("input[type='number']").setAttribute('id', `${slidersData[i].id}_in`);
        clone.querySelector("input[type='number']").setAttribute('onchange', `onSliderChange('${slidersData[i].id}');`);
        clone.querySelector("input[type='number']").setAttribute('oninput', `this.form.${slidersData[i].id}.value=this.value`);

        if(slidersData[i].options){
            if(slidersData[i].options.min)
                clone.querySelector("input[type='range']").setAttribute('min', slidersData[i].options.min);
            if(slidersData[i].options.max)
                clone.querySelector("input[type='range']").setAttribute('max', slidersData[i].options.max);
            if(slidersData[i].options.value)
                clone.querySelector("input[type='range']").setAttribute('value', slidersData[i].options.value);
            if(slidersData[i].options.step)
                clone.querySelector("input[type='range']").setAttribute('step', slidersData[i].options.step);
        }
    
        if(slidersData[i].visible == false){
            clone.querySelector('.slider-input-container').style.display = 'none';
        }
    
        sliderContainer.appendChild(clone);
    }

    if(slidersData[i].type == 'radio'){
        const clone = document.importNode(radioSliderTemplate, true);
        clone.querySelector('.slider-name').textContent = slidersData[i].name;

        for(var j = 0; j < slidersData[i].options.length; j++){

            const radioWrapper = document.createElement('div');
            radioWrapper.className = "slider-radio-wrapper";

            const option = slidersData[i].options[j];
            const radioInput = document.createElement('input');
            radioInput.id = `radio-${slidersData[i].id}-${option.value}`
            radioInput.type = 'radio';
            radioInput.name = slidersData[i].id;
            radioInput.value = option.value;
            radioInput.setAttribute('onclick', `onSliderChange('${slidersData[i].id}', 'radio-${slidersData[i].id}-${option.value}');`);

            const radioLabel = document.createElement('label');
            radioLabel.htmlFor = `radio-${slidersData[i].id}-${option.value}`
            radioLabel.textContent = option.name;

            radioWrapper.appendChild(radioInput);
            radioWrapper.appendChild(radioLabel);

            clone.querySelector(".slidercontainer").appendChild(radioWrapper)
        }

        sliderContainer.appendChild(clone);
    }
   
}


function changeSliderOrientation(){
    var width = window.innerWidth;
    if(width < 1000){
        this.document.querySelectorAll('.slider[orient-dynamic=true]').forEach(slider => {
            slider.setAttribute('orient', 'horizontal')
        })
    } else{
        this.document.querySelectorAll('.slider[orient-dynamic=true]').forEach(slider => {
            slider.setAttribute('orient', 'vertical')
        })
    }
}

changeSliderOrientation();

window.addEventListener('resize', function(event) {
   changeSliderOrientation();
}, true);

