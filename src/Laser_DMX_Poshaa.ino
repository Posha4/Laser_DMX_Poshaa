#include <Arduino.h>
#include <ESPDMX.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>


#include <ESP8266mDNS.h>


#include <WebSocketsServer.h>

// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
#include <FS.h>

#include "WebSockets4WebServerSecure.h"

static const char tls_key[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIIC/DCCAeSgAwIBAgIQXoOzPdQTXZhN674sd44k2zANBgkqhkiG9w0BAQsFADAR
MQ8wDQYDVQQDDAZyZ2JkbXgwHhcNMjQwNjI0MjAzNjUzWhcNMjUwNjI0MjA1NjUz
WjARMQ8wDQYDVQQDDAZyZ2JkbXgwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQCzNBDFsRsEBOZ5JxTDvC3SD2iS8cNKZnC5HPLXaYVfXEMDYLXPHOwaoTtw
QAGIbl9DYFRjHpLifmrOFdDfJQqdCf8s27U4fGRQ0lUjBvQZaxbhT1eqX/m3KPtN
cJ49XjaT74l9wH5BozrNqsV1ssPm9iuPHQlpekhBkkJqkjrE2E/QoJkkSgEHCQsR
g1hSK1jGPvYyuCRkZSmfmX/poUEk+w7oHZDmyt0LoxPuvQt9m9VmKJraWitrNijZ
R4v/c2384hbitgR7MPPLTpfDsGxNbd2bsfF/KYVE/m1cwE1LVq1rnPPMqQZwRasv
8qCa9F9z+U4gYXUbdAamS7cYlZUFAgMBAAGjUDBOMA4GA1UdDwEB/wQEAwIFoDAd
BgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwHQYDVR0OBBYEFPgKgG1UpxQI
Z5Nn3R/IRcNsB8ijMA0GCSqGSIb3DQEBCwUAA4IBAQCwz4xe7f1WemM060WWzV9z
n6nfMtMrA+P43ctIykbvDZ/AFVEFeXyj2nWEfCeZkarb0BxlGNI6+6uf2Q8KeLB3
vjaXc5t6r3ZbAhQb6dd2DwHfIPlSpZ/nBx4ljUet83M37A8dwVHjCCAokKr+4ZTW
qpn5v7lrh1LY6pWN5uLsYALErMFnIm6m9hw/m+fJPVg59gayQ2JF69MwXQqvMR5X
53kaUkq+IWIfwoD0cCaSRgKt+AvR4wI6MSq5oUwTqIdBrJRJce0+72qi/i/Hg9b1
cPGArElCV2V5X5m4CQLz3hge/n8uxIm9yKp6wvycpGtJHBwMheN2wXQkKmJO84X5
-----END PRIVATE KEY-----
)EOF";

static const char tls_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIC/DCCAeSgAwIBAgIQXoOzPdQTXZhN674sd44k2zANBgkqhkiG9w0BAQsFADAR
MQ8wDQYDVQQDDAZyZ2JkbXgwHhcNMjQwNjI0MjAzNjUzWhcNMjUwNjI0MjA1NjUz
WjARMQ8wDQYDVQQDDAZyZ2JkbXgwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQCzNBDFsRsEBOZ5JxTDvC3SD2iS8cNKZnC5HPLXaYVfXEMDYLXPHOwaoTtw
QAGIbl9DYFRjHpLifmrOFdDfJQqdCf8s27U4fGRQ0lUjBvQZaxbhT1eqX/m3KPtN
cJ49XjaT74l9wH5BozrNqsV1ssPm9iuPHQlpekhBkkJqkjrE2E/QoJkkSgEHCQsR
g1hSK1jGPvYyuCRkZSmfmX/poUEk+w7oHZDmyt0LoxPuvQt9m9VmKJraWitrNijZ
R4v/c2384hbitgR7MPPLTpfDsGxNbd2bsfF/KYVE/m1cwE1LVq1rnPPMqQZwRasv
8qCa9F9z+U4gYXUbdAamS7cYlZUFAgMBAAGjUDBOMA4GA1UdDwEB/wQEAwIFoDAd
BgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwEwHQYDVR0OBBYEFPgKgG1UpxQI
Z5Nn3R/IRcNsB8ijMA0GCSqGSIb3DQEBCwUAA4IBAQCwz4xe7f1WemM060WWzV9z
n6nfMtMrA+P43ctIykbvDZ/AFVEFeXyj2nWEfCeZkarb0BxlGNI6+6uf2Q8KeLB3
vjaXc5t6r3ZbAhQb6dd2DwHfIPlSpZ/nBx4ljUet83M37A8dwVHjCCAokKr+4ZTW
qpn5v7lrh1LY6pWN5uLsYALErMFnIm6m9hw/m+fJPVg59gayQ2JF69MwXQqvMR5X
53kaUkq+IWIfwoD0cCaSRgKt+AvR4wI6MSq5oUwTqIdBrJRJce0+72qi/i/Hg9b1
cPGArElCV2V5X5m4CQLz3hge/n8uxIm9yKp6wvycpGtJHBwMheN2wXQkKmJO84X5
-----END CERTIFICATE-----
)EOF";

DMXESPSerial dmx;

ESP8266WiFiMulti wiFiMulti;
// WebSocketsServer webSocket = WebSocketsServer(81);


BearSSL::ESP8266WebServerSecure server(443);
WebSockets4WebServerSecure webSocket_secure;

void websocket_handle_event(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {

        case WStype_DISCONNECTED:
            Serial.println("Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("Client connected!");
            // send message to client
            // webSocket.sendTXT(num, "!Connected");
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

                Serial.println("DMX data received");
            }

            if (payload[0] == '!'){

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

                //transform the payload to a request for all clients
                payload[0] = '?';
                // webSocket.broadcastTXT(payload, length);
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
    // webSocket.sendTXT(num, resp);
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


    if (MDNS.begin("rgbdmx"))
    {
        Serial.println("MDNS responder started");
        MDNS.addService("http", "tcp", 80);
    }

    MDNS.addService("https", "tcp", 443);
    MDNS.addService("wss", "tcp", 443);

    setup_http();

    dmx.init(10); // initialize with bus length

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
}

void setup_http(void) {
    BearSSL::X509List *tls_chain;

    tls_chain = new BearSSL::X509List(tls_cert);
    //tls_chain->append(tls_cert_intermediate);
    //tls_chain->append(tls_cert_root);
    server.getServer().setRSACert(tls_chain, new BearSSL::PrivateKey(tls_key));

    server.on("/", http_handle_root); 
    server.onNotFound(http_handle_404);

    // And here is how we make the websocket pass through the actual server, so we can (re)use SSL and all that
    server.addHook(webSocket_secure.hookForWebserver("/socket", websocket_handle_event));

    server.begin();
    Serial.println("HTTPS server started");
}

void http_handle_root(void) {
    Serial.println("HTTP request received");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", "<!DOCTYPE html><html><body>Some HTML page where you can interact with the websocket</body></html>");
    // The above HTML should contain some JavaScript for connecting to the socket, like this: new WebSocket('wss://' + document.location.host + '/socket');
    // The '/socket' at the end corresponds to the value from the myweb_server.addHook() call
}

void http_handle_404(void) {
    Serial.println("HTTP 404 request received");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "close");
    server.send(404, "text/html", "<!DOCTYPE html><html><body>Some HTML page indicating a 404 error</body></html>");
}

void http_handle_root2() {
    String path = server.uri();
    Serial.println("Requested URL: " + path);

    if (SPIFFS.exists(path)) {
        File file = SPIFFS.open(path, "r");
        size_t sent = server.streamFile(file, getContentType(path));
        file.close();
    } else {
        server.send(404, "text/plain", "File Not Found");
    }
}

String getContentType(String filename) {
    if (filename.endsWith(".html") || filename.endsWith(".htm")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".gif")) return "image/gif";
    else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".xml")) return "text/xml";
    else if (filename.endsWith(".pdf")) return "application/pdf";
    else if (filename.endsWith(".zip")) return "application/zip";
    return "application/octet-stream"; // Default content type
}

void loop()
{
    server.handleClient();
    webSocket_secure.loop();
    dmx.update();
    MDNS.update();
}
