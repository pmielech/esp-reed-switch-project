#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#define Reed 12
int sensState = 0;
//network connection
const char* ssid = "Redmi Note 7";
const char* password = "password";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readReedSensor() {  
  if (isnan(sensState)) {    
    Serial.println("Failed to read from reed sensor!");
    return "";
  }
  else {
    if (digitalRead(Reed) == LOW) {
      //sensState = !sensState;
      Serial.println("Closed");
      delay(200);
    }
    else {
      Serial.println("Opened");
    }
    return String(digitalRead(Reed));
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  pinMode(Reed, INPUT_PULLUP);

  SPIFFS.begin(true);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/door", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readReedSensor().c_str());
  });

  // Start server
  server.begin();
}
void loop(){
}