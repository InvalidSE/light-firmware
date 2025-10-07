#define LED_PIN 13

#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

WebSocketsClient webSocket;
JsonDocument doc;
char *message;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
  case WStype_DISCONNECTED:
    Serial.println("Websocket Disconnected");
    break;
  case WStype_CONNECTED:
    Serial.println("Websocket Connected");
    webSocket.sendTXT("Hola");
    break;
  case WStype_TEXT:
    message = (char *)payload;
    deserializeJson(doc, message);

    Serial.println("Received: " + String((char *)payload));
    break;
  }
}

void enableLed() {
  digitalWrite(LED_PIN, HIGH);
}

void disableLed() {
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
    
  WiFiManager wm;

  bool res;
  res = wm.autoConnect("ChiikawaLight"); 
  // res = wm.autoConnect("ChiikawaLight","higrace<3"); 

  if(!res) {
      Serial.println("not connected D:");
      ESP.restart();
  } 
  else {
      Serial.println("connected :D");
  }

  webSocket.begin("invalidse-wifi-lights.host.qrl.nz", 80);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}
