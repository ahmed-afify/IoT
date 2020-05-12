#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

enum led {onn, off};
char data;
led state=off;
String timee= "11:30:23";
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
void turnon(){
   s.write("o");
  Serial.print("Led On");
  
}
void getTime(){
  timee="";
  bool received=false;
    while (!received)
  {
    if(s.available()>0){
    data=s.read();
    timee=timee+data;
    if(data=='\n')
    received=true;
    Serial.println(data);
    }
  }
 }
 void prnt(){
   s.write("t");
 }
void turnoff(){
   s.write("f");
   Serial.print("Led Off");
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  s.begin(9600);
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    
      turnon();
      state=onn;
      pinMode(2,HIGH);
     
    
    request->send_P(200, "text/plain", "On");
    
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    
      turnoff();
      state=off;
      pinMode(2,LOW);
    
    request->send_P(200, "text/plain","off");
  });
   server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
    

      state=off;
      pinMode(2,LOW);
      prnt();
      getTime();
  
  
    request->send_P(200, "text/plain",timee.c_str());
  });
 
  
  
  // Start server
  server.begin();
}
void loop(){
  
}
