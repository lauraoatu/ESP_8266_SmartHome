#include <ESP8266WiFi.h>
#include "web.h"
#include "DHTesp.h"
#include "ThingSpeak.h" // libraria pt grafice

#define lum 13
#define outlum 14
#define dht_pin 4 //pinul senzorului de temperatura si umiditate
#define temp_pin 12 //pinul releului de temperatura activeaza resou
#define gaz_pin 15
#define water 0
#define tilt 16
#define hum_pin 2

//const char* ssid = "Oatu_Laura";
//const char* password = "12345678"; //


const char* ssid = "iLaura";
const char* password = "OatuLaura"; //

unsigned long myChannelNumber = 1776658;
const char * myWriteAPIKey = "VAPMON6BVH4KQ246";

long lastMillis=0, updateTime=60000;

WiFiServer server(80);
WiFiClient client;
DHTesp dht;

String myStatus = "";
float hum = 0;
float temp =0;
int senzorLum=0;
float tempRef = 31;
int gaz=0;
int relTemp=0;
int relHum=0;
int relLum=0;
int waterLvl=0;
int window_poz = 0;
int humRef= 45;


void setup(void){
  pinMode(outlum, OUTPUT);
  pinMode(lum, INPUT);
  pinMode(temp_pin, OUTPUT);
  pinMode(gaz_pin, INPUT);
  pinMode (tilt, INPUT);
  pinMode(hum_pin, OUTPUT);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  Serial.begin(115200);
  Serial.println("");
  dht.setup(dht_pin, DHTesp::DHT11);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());

}


void loop(void)

{
  //server.handleClient();
  client = server.available();
  if (client) {
    String req = client.readStringUntil('\r');
    if (req.indexOf("/home") != -1){
    //Serial.println("home trimis");
    //Serial.println(req);
    client.flush();
    handleRoot();
  // Match the request
    }
 
  if (req.indexOf("/data") != -1)  {
    //Serial.println("LED OFF");
    client.flush();
    sensor_data();
  }
  if (req.indexOf("/home?") != -1)  {
    updateRef(req);
  }
  }


  checkLight();
  checkDHT();
  gaz=digitalRead(gaz_pin);
  window_poz = digitalRead(tilt);

    if (lastMillis+updateTime <millis()){
    if(WiFi.status() == WL_CONNECTED){
      lastMillis = millis();
      ThingSpeak.setField(1, temp);
      ThingSpeak.setField(2, hum);
      ThingSpeak.setField(3, waterLvl);
      ThingSpeak.setStatus(myStatus);
     int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200){
        Serial.println("Channel update successful.");
      }
      else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
    }
    }
}


void checkLight(){
  senzorLum=digitalRead(lum);
  if(senzorLum==1){
    digitalWrite(outlum, HIGH);
    relLum=1;
  }else{
    digitalWrite(outlum, LOW);
    relLum=0;
  }
}


void checkDHT(){
  temp = dht.getTemperature();
  hum = dht.getHumidity();
  //Serial.println(temp);
 if(temp<tempRef-1){
  digitalWrite(temp_pin, HIGH);
  relTemp=1;
 }else if(temp>tempRef){
  digitalWrite(temp_pin, LOW);
  relTemp=0;
}

if(hum>humRef-1){
  digitalWrite(hum_pin, HIGH);
  relHum=1;
 }else if(hum<=humRef){
  digitalWrite(hum_pin, LOW);
  relHum=0;
}

}
