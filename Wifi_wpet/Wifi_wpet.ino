
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <ESP8266mDNS.h>

#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(D4);
DallasTemperature sensors(&oneWire);

// Replace with your network credentials
const char* ssid = "WD-40";
const char* password = "mirki2137";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String host = "pdpwpetantek.000webhostapp.com";
String ip;
String redirectUrl;



String readBME280Temperature() {
  
  // Read temperature as Celsius (the default)
  sensors.requestTemperatures();
  
  float t = sensors.getTempCByIndex(0);
  
  if (isnan(t)) {    
    Serial.println("Failed to read temperature!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  sensors.begin();

  bool status; 
  // default settings
  // (you can also pass in a Wire library object like &Wire2)

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  ip = WiFi.localIP().toString();
  Serial.println(ip);

  
  
  redirectUrl = "http://" + host + "/static/test.html?title=" + "&ip=" + ip;
  Serial.print("It will redirect to: ");
  Serial.println(redirectUrl);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBME280Temperature().c_str());
  });
  
  


  // Start server
  server.begin();
  
}
 
void loop(){
  
}
