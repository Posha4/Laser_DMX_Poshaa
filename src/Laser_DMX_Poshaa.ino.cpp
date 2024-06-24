# 1 "C:\\Users\\cleme\\AppData\\Local\\Temp\\tmpr0fksff5"
#include <Arduino.h>
# 1 "C:/Users/cleme/Documents/PlatformIO/Projects/240618-190049-esp32dev/src/Laser_DMX_Poshaa.ino"
# 33 "C:/Users/cleme/Documents/PlatformIO/Projects/240618-190049-esp32dev/src/Laser_DMX_Poshaa.ino"
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
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void sendConfig(uint8_t num);
void setup();
void loop();
#line 51 "C:/Users/cleme/Documents/PlatformIO/Projects/240618-190049-esp32dev/src/Laser_DMX_Poshaa.ino"
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {

        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Client connected!");

            webSocket.sendTXT(num, "!Connected");
            sendConfig(num);

            break;
        case WStype_TEXT:
        {

            if (payload[0] == '#')
            {

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

                Serial.println("writing to file");

                f.write(payload + 1, length - 1);

                Serial.println("");

                Serial.println("file written");

                Serial.println("");
                f.close();


                payload[0] = '?';
                webSocket.broadcastTXT(payload, length);
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

    String resp = String("?" + json);
    webSocket.sendTXT(num, resp);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();


    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }


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

    dmx.init(9);

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