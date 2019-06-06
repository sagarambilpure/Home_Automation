#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//const char* ssid = "Kossine Pluto";
//const char* password = "##K##51n123#pluto123##";
const char* ssid = "Sagar";
const char* password = "sameer123";
IPAddress ip(192, 168, 0,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);


ESP8266WebServer server(80);
int realValues(String s){
if (s=="1")
return 4;
else if (s=="2")
return 5;
}
void handleRoot() { 
 server.send(200, "text/html", "<h1>You are connected</h1>"); 
} 


void handleLight(){
  int t=realValues(server.arg("led"));
  digitalWrite(t,!digitalRead(t));
  Serial.print("led");
  
  /*
  while(t.indexOf(",")!=-1)
  {
    int x=t.indexOf(",");
    Serial.println("v - " + t.substring(0,x));
    t=t.substring(x+1);
    Serial.println("t - " + t);
  }*/
  
  handleRoot();
}

void handleFan(){
  String t=server.arg("fan");
  
  if(t.indexOf(",")!=-1)
  {
     int x=t.indexOf(",");
     int number=realValues(t.substring(0,x));
     t=t.substring(x+1);
     
     x=t.indexOf(",");
     int value=t.substring(0,x).toInt();
     t=t.substring(x+1);

     analogWrite(number,value);
  }

  handleRoot();
}

void setup(void){
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  //WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  WiFi.config(ip,gateway,subnet); 
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot); 
  server.on("/lights",handleLight);
  server.on("/fans",handleFan);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
