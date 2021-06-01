#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "XXXXXX";
const char* password =  "XXXXXXXXX";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";


#include "DHT.h"

#define DHTPIN D7     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define PUB_GPIO2_STATUS "wemos/status"
#define SUB_GPIO2_ACTION "wemos/action"

#define GPIO2_LED D8
 
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
 
void loop() {
    mqttClient.loop();
    float h = dht.readHumidity();
  // Read temperature as Celsius
float t = dht.readTemperature();
  // Read temperature as Fahrenheit
float f = dht.readTemperature(true);
  delay(2000);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\n");
  mqttClient.publish("wemos_hData", String(h).c_str(), true);
  delay(2000);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" C\n");
  mqttClient.publish("wemos_tData", String(t).c_str(), true);
}

void initWifiStation() {

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);    
    Serial.print("\nConnecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
       delay(1000);        
       Serial.print(".");
    }
    Serial.println(String("\nConnected to the WiFi network (") + ssid + ")" );
}
 
void initMQTTClient() {

    // Connecting to MQTT server
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(PubSubCallback);
    while (!mqttClient.connected()) {
        Serial.println(String("Connecting to MQTT (") + mqttServer + ")...");
        if (mqttClient.connect("my-MQTT-Broker", mqttUser, mqttPassword)) {
            Serial.println("MQTT client connected");  
        } else {
            Serial.print("\nFailed with state ");
            Serial.println(mqttClient.state());
            
            if (WiFi.status() != WL_CONNECTED) {
               initWifiStation();
            }
            delay(2000);
        }
    }

    // Declare Pub/Sub topics
    mqttClient.publish(PUB_GPIO2_STATUS, "OFF");
    mqttClient.subscribe(SUB_GPIO2_ACTION);
}

void setup() {

    Serial.begin(115200);

  // GPIO2 is set OUTPUT
    pinMode(GPIO2_LED, OUTPUT);

    initWifiStation();
    initMQTTClient();
     dht.begin();
}
 
void PubSubCallback(char* topic, byte* payload, unsigned int length) {

    String strTopicGpio2Action = SUB_GPIO2_ACTION;
    String strPayload = "";
    String strON = "ON";
    String strOFF = "OFF";

    Serial.print("Topic:");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        strPayload += (char)payload[i];
    }
    Serial.println();
    Serial.println("-----------------------");
 
    if (strTopicGpio2Action == topic) {
        if (strON == strPayload) {
            digitalWrite(GPIO2_LED, LOW);
            mqttClient.publish(PUB_GPIO2_STATUS, "ON");
        } else if (strOFF == strPayload) {
            digitalWrite(GPIO2_LED, HIGH);
            mqttClient.publish(PUB_GPIO2_STATUS, "OFF");
        }
    }

}
