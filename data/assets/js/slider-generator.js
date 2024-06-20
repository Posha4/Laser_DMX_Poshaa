const sliderContainer = document.getElementById('slider-container');
const sliderTemplate = document.getElementById('slider-template').content;

for(var i = 0; i < slidersData.length; i++) {
    const clone = document.importNode(sliderTemplate, true);

    clone.querySelector('.slider-name').textContent = slidersData[i].name;

    clone.querySelector("input[type='range']").setAttribute('id', slidersData[i].id);
    clone.querySelector("input[type='range']").setAttribute('onchange', `onSliderChange('${slidersData[i].id}');`);
    clone.querySelector("input[type='range']").setAttribute('oninput', `this.form.${slidersData[i].id}_in.value=this.value;onSliderChange('${slidersData[i].id}',${i+1});`);

    clone.querySelector("input[type='number']").setAttribute('id', `${slidersData[i].id}_in`);
    clone.querySelector("input[type='number']").setAttribute('onchange', `onSliderChange('${slidersData[i].id}');`);
    clone.querySelector("input[type='number']").setAttribute('oninput', `this.form.${slidersData[i].id}.value=this.value`);

    if(slidersData[i].visible == false){
        clone.querySelector('.slider-input-container').style.display = 'none';
    }

    sliderContainer.appendChild(clone);
}