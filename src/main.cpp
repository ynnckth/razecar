#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

const char* ssid     = "RazeCar_Yannick";
const char* password = "123456789";

void setup()
{
  Serial.begin(115200);
  
  // Make sure the file system is initialized
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Create an access point
  WiFi.softAP(ssid, password);

  // Print our own IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/motors/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requested to drive LEFT");
    Serial.println(request->getParam(0)->value());
    request->send(200);
  });

  server.on("/motors/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requested to drive RIGHT");
    Serial.println(request->getParam(0)->value());
    request->send(200);
  });

  server.begin();
}

void loop()
{

}
