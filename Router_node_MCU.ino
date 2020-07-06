#include <FS.h> 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>

const char* ssid = "";
const char* password = "";
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

ESP8266WebServer server(80);
SoftwareSerial NodeMCU(D2,D3);
void setup() {
    Serial.begin(250000);
    NodeMCU.begin(4800);
    pinMode(D2,INPUT);
    pinMode(D3,OUTPUT);

   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid,password); 
   // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
server.begin(); 
server.on("/", fileindex);
server.on("/index.html", fileindex);
server.on("/bootstrap.min.css", bootstrap);
server.on("bootstrap.min.css", bootstrap);
server.on("/popper.min.js", popper);
server.on("/bootstrap.min.js", bootstrapmin);
server.on("bootstrap.min.js", bootstrapmin);


//NEW

SPIFFS.begin(); 
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


}

void loop() {
server.handleClient();

  String path_us1 ="/user1";
  //String path_us2 ="/user_2";
  FirebaseObject object = Firebase.get(path_us1);
  //String Name = object.getString("name");
  String Name = object.getString("name");
  String Time = object.getString("time");
  String area = object.getString("area");
  //Serial.println(area);
  String hours = getValue(Time, ':', 0);
  String mins = getValue(Time, ':', 1);
  Time = hours+mins;
  //Serial.println(Time);
  int TIME = Time.toInt();
Serial.print("Time Sent to master Arduino :");
  Serial.println(TIME);
  NodeMCU.print(TIME);
  delay(250);
  //NodeMCU.println(Mins);
  //delay(2500);
while(NodeMCU.available())
{
  Firebase.set("/user1","1");
  }
delay(50);

}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void fileindex()
{
  File file = SPIFFS.open("/index.html.gz", "r"); 
  size_t sent = server.streamFile(file, "text/html");
}
void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r"); 
  size_t sent = server.streamFile(file, "text/css");
}
void popper()
{
  File file = SPIFFS.open("/popper.min.js.gz", "r"); 
  size_t sent = server.streamFile(file, "application/javascript");
}
void bootstrapmin()
{
  File file = SPIFFS.open("/bootstrap.min.js.gz", "r"); 
  size_t sent = server.streamFile(file, "application/javascript");
}
