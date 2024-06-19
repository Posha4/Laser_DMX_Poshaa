const container = document.getElementById('slider-container');
const template = document.getElementById('slider-template').content;

for(var i = 0; i < slidersData.length; i++) {
    const clone = document.importNode(template, true);

    clone.querySelector('.slider-name').textContent = slidersData[i].name;


    clone.querySelector("input[type='range']").setAttribute('id', `chan${i+1}`);
    clone.querySelector("input[type='range']").setAttribute('onchange', `onSliderChange('chan${i+1}',${i+1});`);
    clone.querySelector("input[type='range']").setAttribute('oninput', `this.form.chan${i+1}_in.value=this.value`);

    clone.querySelector("input[type='number']").setAttribute('id', `chan${i+1}_in`);
    clone.querySelector("input[type='number']").setAttribute('onchange', `onSliderChange('chan${i+1}',${i+1});`);
    clone.querySelector("input[type='number']").setAttribute('oninput', `this.form.chan${i+1}.value=this.value`);

    container.appendChild(clone);
}