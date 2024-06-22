/* This example joins two useful libraries for ESP8266 WiFi Module
 * Websockets library for interfacing from either web interface or
 * some external application and DMX library for controlling
 * devices on a DMX bus.
 *
 * Install these libraries:
 * https://github.com/Links2004/arduinoWebSockets
 * https://github.com/Rickgg/ESP-Dmx
 *
 * Web interface allow one to set the channel and control an RGB
 * light, assuming channels for cors follow one after eachother
 *
 * How to use:
 * 1) Change WiFi settings accordingly to connect to your network
 * 2) Connect an RS485 driver chip to GPIO02( D4 on Nodemcu 1.0)
 * 3) Check out the serial output for IP or
 * visit http://rgbdmx.local if your device supports mDNS and
 * you are in the same local network

Copyright Institute IRNAS Rače 2016 - info@irnas.eu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <Arduino.h>
#include <ESPDMX.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

DMXESPSerial dmx;

ESP8266WiFiMulti wiFiMulti;
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {

        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Client connected!");
            // send message to client
            webSocket.sendTXT(num, "!Connected");
            sendConfig(num);

            break;
        case WStype_TEXT:
        {
            // # is the start for this data
            if (payload[0] == '#')
            {
                // data received is comma separated, thats why we do pEnd+1 to start next value
                char *pEnd;
                uint32_t address = strtol((const char *)&payload[1], &pEnd, 16);
                uint32_t chan1 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan2 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan3 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan4 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan5 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan6 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan7 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan8 = strtol((const char *)pEnd + 1, &pEnd, 16);
                uint32_t chan9 = strtol((const char *)pEnd + 1, &pEnd, 16);

                // write to DMX bus
                dmx.write(address, chan1);
                dmx.write(address + 1, chan2);
                dmx.write(address + 2, chan3);
                dmx.write(address + 3, chan4);
                dmx.write(address + 4, chan5);
                dmx.write(address + 5, chan6);
                dmx.write(address + 6, chan7);
                dmx.write(address + 7, chan8);
                dmx.write(address + 8, chan9);
                dmx.update();
            }

            if (payload[0] == '!'){

                int jsonLength = length-1;

                Serial.println("====== Writing to SPIFFS file =========");

                File f = SPIFFS.open("/data.json", "w");
                if (!f) {
                    Serial.println("file open failed");
                    return;
                }

                char json[jsonLength-1];
                for (int i = 0; i < jsonLength; i++){
                    json[i] = payload[i+1];
                    Serial.print(json[i]);
                }

                f.write(json, jsonLength);

                Serial.println("");
                f.close();

                for (uint8_t i = 0; i < webSocket.connectedClients(); i++) {
                   sendConfig(i);
                }
            }

            if (payload[0] == '?'){
               sendConfig(num);  
            }
            break;
        }
       
    }
}

void sendConfig(uint8_t num){
    File f = SPIFFS.open("/data.json", "r");

    if (!f) {
        Serial.println("file open failed");
    }

    Serial.println("====== Reading from SPIFFS file =========");

    String json = f.readString();
    Serial.print("?");
    Serial.println(json);

    

    f.close();

    String resp =  String("?" + json);
    webSocket.sendTXT(num, resp);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // connect to WiFi
    WiFi.hostname("rgbdmx");
    WiFi.mode(WIFI_STA);

    wiFiMulti.addAP("Scotty", "Tatatoum12345");
    wiFiMulti.addAP("The Kave", "Tatatoum");
    wiFiMulti.addAP("Bbox-8292DA0D","EF3ACE1F7D37FDA35556313D114564");

    Serial.println("Connecting ...");
    int i = 0;
    while (wiFiMulti.run() != WL_CONNECTED) {
        delay(1000);
        Serial.print(++i); Serial.print(' ');
    }

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    if (MDNS.begin("rgbdmx"))
    {
        Serial.println("MDNS responder started");
        MDNS.addService("http", "tcp", 80);
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("GET /");
        request->send(SPIFFS, "/index.html", "text/html"); 
    });

    server.on("/*", HTTP_GET, [](AsyncWebServerRequest *request){
        String path = request->url();
        Serial.println("Requested URL: " + path);

        if(SPIFFS.exists(path)){
            request->send(SPIFFS, path);
        } else {
            request->send(404, "text/plain", "File Not Found");
        }
    });

    server.begin();
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

    dmx.init(10); // initialize with bus length

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}

void loop()
{
    webSocket.loop();
    dmx.update();
    MDNS.update();
}
