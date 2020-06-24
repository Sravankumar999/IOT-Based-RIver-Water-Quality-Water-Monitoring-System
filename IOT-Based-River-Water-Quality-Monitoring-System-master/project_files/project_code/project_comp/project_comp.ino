#include "WiFi.h"
#include "PubSubClient.h"
#include <OneWire.h>
#include <DallasTemperature.h>
 
//-------- Customise these values -----------
const char* ssid = "harshith";
const char* password = "harshith4567";

#define ONE_WIRE_BUS 2
 
#define ORG "e9rymb"
#define DEVICE_TYPE "project"
#define DEVICE_ID "12345678"
#define TOKEN "123456789"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;

void setup() {
  pinMode(35,INPUT);
  pinMode(A0,INPUT);
   
    sensors.begin();
 Serial.begin(115200);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
  

PublishData();

}



void PublishData(){

   float value = analogRead(35);
  float a = value*(5.0/4095.0);
  Serial.println(a);
  
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);

 int sensorValue = analogRead(A0);
 float voltage = sensorValue * (5.0/ 4095.0)+2;
 Serial.println(voltage); 
 
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"voltage\":";
  payload += a;
  payload+="," "\"temp\":";
   payload += Celcius;
   payload+="," "\"ph\":";
   payload += voltage;
  
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
