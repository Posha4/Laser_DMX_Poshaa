const char INDEX_HTML[] PROGMEM = R"=====(
<html>
   <head>
      <script>
        var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
        connection.onopen = function() {
            connection.send('Connect ' + new Date());
        };
        connection.onerror = function(error) {
            console.log('WebSocket Error ', error);
        };
        connection.onmessage = function(e) {
            console.log('Server: ', e.data);
        };
        id_array = new Array("addr", "chan1", "chan2", "chan3", "chan4", "chan5", "chan6", "chan7", "chan8", "chan9");
        values = new Array(id_array.length);

        function prepareVar(id, position) {
            for (i = 0; i < id_array.length; i++) {
                var a = parseInt(document.getElementById(id_array[i]).value).toString(16);
                if (a.length < 2) {
                    a = '0' + a;
                }
                values[i] = a;
            }
            sendVars();
        }

        function blackoutToogle() {
          var action = document.querySelector(".tgl-btn");
          action.classList.toggle("active");
          
          if (action.classList.contains("active")) {
            var data = '#01,00,00,00,00,00,00,00,00,00';
            console.log('Data: ' + data);
            connection.send(data);
            console.log("Blackout");
          } else {
            // annyang.abort();
            button.value = "OFF";
            prepareVar('chan1',1);
            console.log("Stopped");
          }
        }


        function sendVars() {
            var data = '#' + values;
            console.log('Data: ' + data);
            connection.send(data);
        }
      </script>
   </head>

   <body>
      Laser Control:<br/>
      <br/>
      <link href='http://fonts.googleapis.com/css?family=Roboto+Mono' rel='stylesheet' type='text/css'>
      <form> Starting address: <input id="addr" type="number" value="1" placeholder="1" min="0" max="512" step="1" onchange="prepareVar('addr',0);;" /><br/>
        mode: <input id="chan1" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan1',1);" oninput="this.form.chan1_in.value=this.value" /><input id="chan1_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan1',1);" value="0" oninput="this.form.chan1.value=this.value" /><br/>
        figs: <input id="chan2" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan2',2);" oninput="this.form.chan2_in.value=this.value" /><input id="chan2_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan2',2);" value="0" oninput="this.form.chan2.value=this.value" /><br/>
        zoom: <input id="chan3" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan3',3);" oninput="this.form.chan3_in.value=this.value" /><input id="chan3_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan3',3);" value="0" oninput="this.form.chan3.value=this.value" /><br/>
        Xond: <input id="chan4" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan4',4);" oninput="this.form.chan4_in.value=this.value" /><input id="chan4_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan4',4);" value="0" oninput="this.form.chan4.value=this.value" /><br/>
        Yond: <input id="chan5" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan5',5);" oninput="this.form.chan5_in.value=this.value" /><input id="chan5_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan5',5);" value="0" oninput="this.form.chan5.value=this.value" /><br/>
        Zond: <input id="chan6" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan6',6);" oninput="this.form.chan6_in.value=this.value" /><input id="chan6_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan6',6);" value="0" oninput="this.form.chan6.value=this.value" /><br/>
        Xmov: <input id="chan7" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan7',7);" oninput="this.form.chan7_in.value=this.value" /><input id="chan7_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan7',7);" value="0" oninput="this.form.chan7.value=this.value" /><br/>
        Ymov: <input id="chan8" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan8',8);" oninput="this.form.chan8_in.value=this.value" /><input id="chan8_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan8',8);" value="0" oninput="this.form.chan8.value=this.value" /><br/>
        colo: <input id="chan9" type="range" value="0" min="0" max="255" step="1" onchange="prepareVar('chan9',9);" oninput="this.form.chan9_in.value=this.value" /><input id="chan9_in" type="number" min="0" max="255" step="1" onchange="prepareVar('chan9',9);" value="0" oninput="this.form.chan9.value=this.value" /><br/>
        <li class="tg-list-item">
          <h4>Blackout</h4>
          <input class="tgl tgl-skewed" id="cb3" type="checkbox"/>
          <label class="tgl-btn" data-tg-off="OFF" data-tg-on="ON" onclick="blackoutToggle()"></label>
        </li>
      </form>
   </body>
</html>
<style>
  body {
    font-family: 'Roboto Mono', sans-serif;
    min-height: 100%;
    text-align:center;
  }

		button {
			color: #ffffff;
			background-color: #2d63c8;
			font-size: 19px;
			border: 1px solid #2d63c8;
			padding: 15px 50px;
			cursor: pointer
		}
		button:hover {
			color: #2d63c8;
			background-color: #ffffff;
		}

    ul,
    li {
      list-style: none;
      margin: 0;
      padding: 0;
    }

    .tg-list {
      text-align: center;
      display: flex;
      align-items: center;
    }

    .tg-list-item {
      margin: 0 2em;
    }

    h2 {
      color: #777;
    }

    h4 {
      color: #999;
    }

    .tgl {
      display: none;
      
      // add default box-sizing for this scope
      &,
      &:after,
      &:before,
      & *,
      & *:after,
      & *:before,
      & + .tgl-btn {
        box-sizing: border-box;
        &::selection {
          background: none;
        }
      }
      
      + .tgl-btn {
        outline: 0;
        display: block;
        width: 4em;
        height: 2em;
        position: relative;
        cursor: pointer;
        user-select: none;
        &:after,
        &:before {
          position: relative;
          display: block;
          content: "";
          width: 50%;
          height: 100%;
        }
        
        &:after {
          left: 0;
        }
        
        &:before {
          display: none;
        }
      }
      
      &:checked + .tgl-btn:after {
        left: 50%;
      }
    }

    .tgl-skewed {
      + .tgl-btn {
        overflow: hidden;
        backface-visibility: hidden;
        transition: all 1s ease;
        font-family: sans-serif;
        background: #888;
        &:after,
        &:before {
          display: inline-block;
          transition: all .2s ease;
          width: 100%;
          text-align: center;
          position: absolute;
          line-height: 2em;
          font-weight: bold;
          color: #fff;
          text-shadow: 0 1px 0 rgba(0,0,0,.4);
        }
        
        &:after {
          left: 100%;
          content: attr(data-tg-on);
        }
        
        &:before {
          left: 0;
          content: attr(data-tg-off);
        }
        
        &:active {
          background: #888;
          &:before {
            left: -10%;
          }
        }
      }
      
      &:checked + .tgl-btn {
        background: #86d993;
        &:before {
          left: -100%;
        }

        &:after {
          left: 0;
        }

        &:active:after {
          left: 10%;
        }
      }
    }
</style>
)=====";