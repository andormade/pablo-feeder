#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "ssid";
char pass[] = "password";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  WiFi.setHostname("Catfeeder");

  while (status != WL_CONNECTED) {
    Serial.println("Connecting...");
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  printWifiStatus(&Serial);
}


void loop() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            printHttpHeaders(&client);
            printWifiStatus(&client);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    client.stop();
  }
}

void printHttpHeaders(Stream *Out) {
  Out->println("HTTP/1.1 200 OK");
  Out->println("Content-type:application/json");
  Out->println();
}

void printWifiStatus(Stream *Out) {
  byte mac[6];
  WiFi.macAddress(mac);

  Out->println("{");
  Out->print("  \"status\": "); Out->print(status); Out->println(",");
  Out->print("  \"rssi\": "); Out->print(WiFi.RSSI()); Out->println(",");
  Out->print("  \"localIp\": \""); Out->print(WiFi.localIP()); Out->println("\",");
  Out->print("  \"gatewayIp\": \""); Out->print(WiFi.gatewayIP()); Out->println("\",");
  Out->print("  \"firmwareVersion\": \""); Out->print(WiFi.firmwareVersion()); Out->println("\",");
  Out->print("  \"ssid\": \""); Out->print(WiFi.SSID()); Out->println("\",");
  Out->print("  \"mac\": \"");
  Out->print(mac[5], HEX);
  Out->print(":");
  Out->print(mac[4], HEX);
  Out->print(":");
  Out->print(mac[3], HEX);
  Out->print(":");
  Out->print(mac[2], HEX);
  Out->print(":");
  Out->print(mac[1], HEX);
  Out->print(":");
  Out->print(mac[0], HEX);
  Out->println("\"");
  Out->println("}");
}
