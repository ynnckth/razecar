#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

const char* ssid     = "RazeCar_Yannick";
const char* password = "123456789";

const int MOTOR_A_1 = 23;
const int MOTOR_A_2 = 22;
const int MOTOR_B_1 = 32;
const int MOTOR_B_2 = 33;

int leftMotorDirection = 0;
int rightMotorDirection = 0;
bool isDirty = false;

void setup()
{
  Serial.begin(115200);

  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  
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

  server.on("/motors/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requested to drive LEFT");
    Serial.println(request->getParam(0)->value());

    if (request->getParam(0)->value() == "true") {
      leftMotorDirection = 1;
    } else {
      leftMotorDirection = 0;
    }
    isDirty = true;
    request->send(200);
  });

  server.on("/motors/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requested to drive RIGHT");
    Serial.println(request->getParam(0)->value());

    if (request->getParam(0)->value() == "true") {
      rightMotorDirection = 1;
    } else {
      rightMotorDirection = 0;
    }
    isDirty = true;
    request->send(200);
  });

  // This should be the last registered pipeline step since it's the costliest one
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.begin();
}

void loop()
{
  if (!isDirty) {
    return;
  }

  if (leftMotorDirection == 1) {
    digitalWrite(MOTOR_A_1, HIGH);
    digitalWrite(MOTOR_A_2, LOW);
  } else if (leftMotorDirection == 0) {
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, LOW);
  }

  if (rightMotorDirection == 1) {
      digitalWrite(MOTOR_B_1, HIGH);
      digitalWrite(MOTOR_B_2, LOW);
  } else if (rightMotorDirection == 0) {
      digitalWrite(MOTOR_B_1, LOW);
      digitalWrite(MOTOR_B_2, LOW);
  }

  isDirty = false;
}
