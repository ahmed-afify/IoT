#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

// Set your access point network credentials
const char* ssid = "kareem";
const char* password = "kareem1234";

enum led {onn, off};
char data;
led state=off;
String timee= "11:30:00";
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);
void turnon(){
   s.write("o");
   state=onn;
   pinMode(2,HIGH);
    
  server.send(200, "text/html", SendHTML(true));
  Serial.print("Led On\n");
  
}
void getTime(){
  Serial.print("Getting time\n");
  timee="";
  bool received=false;
  prnt();
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
      pinMode(2,LOW);
   server.send(200, "text/html",SendHTML(state==onn));
     
 }
 void prnt(){
   s.write("t");
 }
void turnoff(){
   s.write("f");
   state=off;
   pinMode(2,LOW);
   server.send(200, "text/html", SendHTML(false));
   Serial.print("Led Off\n");
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  s.begin(9600);
  // Setting the ESP as an access point
  // Remove the password parameter, if you want the AP (Access Point) to be open
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/on",turnon);
  server.on("/off",turnoff);
  server.on("/time",getTime);
   server.on("/",turnoff);
  
 
  
  
  // Start server
  server.begin();
}
void loop(){
  server.handleClient();
}
String SendHTML(uint8_t led){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>LED Control</title>\n";
  ptr+="<style>body{margin:0;font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,\"Helvetica Neue\",Arial,\"Noto Sans\",sans-serif,\"Apple Color Emoji\",\"Segoe UI Emoji\",\"Segoe UI Symbol\",\"Noto Color Emoji\";font-size:1rem;font-weight:400;line-height:1.5;color:#c11675;text-align:left;background-color:#121212}button{border-radius:0}button:focus{outline:1px dotted;outline:5px auto -webkit-focus-ring-color}button,input,optgroup,select,textarea{color: #fff;margin:0;font-family:inherit;font-size:inherit;line-height:inherit}button,input{overflow:visible}button,select{text-transform:none}.btn-primary{color: #fff;background-color: #121212;border-radius: 7px;border-color: #c11675;margin: 7px 5px;}.btn-primary:hover{color:#fff;background-color:#c11675;border-color:#c11675}.btn-primary.focus,.btn-primary:focus{box-shadow:0 0 0 .2rem rgba(38,143,255,.5)} .bg-dark{    background-color: #121212!important; border-radius: 7px; margin: 4px; border-color: #c11675;} h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:.5rem}.h1,.h2,.h3,.h4,.h5,.h6,h1,h2,h3,h4,h5,h6{margin-bottom:.5rem;font-weight:500;line-height:1.2}.h1,h1{font-size:2.5rem} .container{width:100%;padding-right:15px;padding-left:15px;margin-right:auto;margin-left:auto} .shadow-none{box-shadow:none!important}</style>";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr+= "<div class=\"container\">";
  ptr +="<h1>IoT Application</h1>\n";
  ptr +="<p>Click to switch LED on and off.</p>\n";
  ptr +="<form method=\"get\">\n";
  if(led)
  ptr +="<input type=\"button\" class= \"btn-primary\" value=\"LED OFF\" onclick=\"window.location.href='/off'\">\n";
  else
  ptr +="<input type=\"button\" class= \"btn-primary\" value=\"LED ON\" onclick=\"window.location.href='/on'\">\n";
  ptr +="</form>\n";
  ptr +="<form method=\"get\">\n";
 
  ptr +="<input type=\"button\" class= \"btn-primary\" value=\"Get Time\" onclick=\"window.location.href='/time'\">\n";

  ptr +="</form>\n";
  ptr +="<textarea rows=\"1\" cols=\"10\" readonly class= \"bg-dark shadow-none\" name=\"comment\" form=\"usrform\">";
  ptr += timee;
  ptr +="</textarea>";
  ptr+= "</div>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
