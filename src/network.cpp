#include <Arduino.h>
#include <AutoConnect.h>

#include "motor.h"

#define AP_NAME "TEMP-REG"
#define AP_PASSWORD ""

ESP8266WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig acConfig;

//---------------------------------------------------------------------------------
String localIP() {
  return WiFi.localIP().toString();
}

//---------------------------------------------------------------------------------
void handleNotFound() {
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += Server.uri();
  message += "\nMethod: ";
  message += (Server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += Server.args();
  message += "\n\n";
  message += "source code: https://github.com/kzub/distilreg\n\n";
  Server.send(200, "text/plain", message);
}

//------------------------------------------------------------------------------
void sendMotorPosition () {
  String response = "{\"position\":";
  response += motorPosition;
  response += "}\n";
  Server.send(200, "applicaion/json", response);
}

void rootPage() {
  sendMotorPosition();
}

//------------------------------------------------------------------------------
void onRotate() {
  String steps = Server.arg("steps");
  int stepsInt = steps.toInt();
  motorTurn(stepsInt);
  Serial.printf("Rotate: %s, current:%ld\n", steps.c_str(), motorPosition);
  sendMotorPosition();
}

//------------------------------------------------------------------------------
void networkSetup() {
  acConfig.title = AP_NAME;
  acConfig.apid = AP_NAME;
  acConfig.psk = AP_PASSWORD;
  acConfig.hostName = acConfig.apid;
  acConfig.autoReconnect = true;
  acConfig.reconnectInterval = 10;

  Portal.config(acConfig);
  Portal.onNotFound(handleNotFound);

  Server.on("/", rootPage);
  Server.on("/rotate", onRotate);

  if (Portal.begin()) { // will freeze here until connected
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  Serial.println("network ok");
}

//------------------------------------------------------------------------------
void networkLoop() {
  Portal.handleClient();
}
