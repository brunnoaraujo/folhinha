#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define SENSER_BIT    9      

int ldr;
double temp;

const long interval = 1000;
unsigned long previousMillis = 0;


OneWire oneWire(D4);
DallasTemperature sensors(&oneWire);

std::unique_ptr<ESP8266WebServer> server;

void handleRoot() {
  server->send(200, "text/plain", "hello from esp8266!");
  sensors.setResolution(SENSER_BIT);
}


void setup() {
  pinMode(5, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(5, LOW);
  Serial.begin(115200);
  
  WiFiManager wifiManager;
  
  wifiManager.resetSettings(); //reset saved settings

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("FazLabs");
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();


  Serial.println("connected...yeey :)");
  
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));

  server->on("/", handleRoot);

  server->on("/inline", []() {
    server->send(200, "text/plain", "this works as well");
  });
  
  server->on("/on", []() {
    digitalWrite(5, 1);
    digitalWrite(LED_BUILTIN,1);  
    server->send(200, "text/plain", "On");
  });
  
    server->on("/off", []() {
    digitalWrite(5, 0);
    digitalWrite(LED_BUILTIN, 0);
    server->send(200, "text/plain", "Off");
  });

  server->begin();
  
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void getValues(){
  sensors.requestTemperatures();              
  temp = sensors.getTempCByIndex(0);
  ldr = analogRead(A0);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    getValues();
    String luminosidade = String(ldr);
    String temperatura = String(temp);
    HTTPClient http;
    http.begin("http://192.168.0.101:9090/?ldr="+luminosidade+"&temp="+temperatura);
    http.GET();
    http.end();
    }
  
  server->handleClient();
  
}




