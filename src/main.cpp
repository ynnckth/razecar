#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

const char* ssid     = "RazeCar_Yannick";
const char* password = "123456789";

const int LEFT_MOTOR_IN_1 = 23;
const int LEFT_MOTOR_IN_2 = 22;
const int RIGHT_MOTOR_IN_1 = 32;
const int RIGHT_MOTOR_IN_2 = 33;

int leftMotorDirection = 0;
int rightMotorDirection = 0;
bool isDirty = false;

void setup()
{
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  pinMode(LEFT_MOTOR_IN_1, OUTPUT);
  pinMode(LEFT_MOTOR_IN_2, OUTPUT);
  pinMode(RIGHT_MOTOR_IN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_IN_2, OUTPUT);
  
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
    leftMotorDirection = request->getParam(0)->value() == "true" ? 1 : 0;
    isDirty = true;
    request->send(200);
  });

  server.on("/motors/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requested to drive RIGHT");
    Serial.println(request->getParam(0)->value());
    rightMotorDirection = request->getParam(0)->value() == "true" ? 1 : 0;
    isDirty = true;
    request->send(200);
  });

  // This should be the last registered pipeline step since it's the costliest one
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.begin();

  digitalWrite(BUILTIN_LED, LOW);
  delay(500);
  digitalWrite(BUILTIN_LED, HIGH);
  delay(500);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
  if (!isDirty) {
    return;
  }

  if (leftMotorDirection == 1) {
    digitalWrite(LEFT_MOTOR_IN_1, HIGH);
    digitalWrite(LEFT_MOTOR_IN_2, LOW);
  } else if (leftMotorDirection == 0) {
    digitalWrite(LEFT_MOTOR_IN_1, LOW);
    digitalWrite(LEFT_MOTOR_IN_2, LOW);
  }

  if (rightMotorDirection == 1) {
      digitalWrite(RIGHT_MOTOR_IN_1, HIGH);
      digitalWrite(RIGHT_MOTOR_IN_2, LOW);
  } else if (rightMotorDirection == 0) {
      digitalWrite(RIGHT_MOTOR_IN_1, LOW);
      digitalWrite(RIGHT_MOTOR_IN_2, LOW);
  }

  isDirty = false;
}
