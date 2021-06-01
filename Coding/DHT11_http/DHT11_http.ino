
#include <DHT.h> // DHT.h library
#include <ESP8266WiFi.h> // ESP8266Wifi.h library

#define DHTPIN D7         // This is the Pin you are connecting the sensor to on the Wemos
#define DHTTYPE DHT11     // This can be changed if your are using a different DHT sensor

const char* ssid     = "xxxxxxxx";      // Change to be your WiFi networks name
const char* password = "xxxxxxxx";         // Change to be your WiFi password
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "XXXXXXXXXX";   // Change the XXXXXXX to your ThingSpeak writeAPI

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  // initialize sensor
  Serial.begin(115200); //May change to 9600
  dht.begin();
   delay(1000);
   
 // Connect to WiFi network  
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

   if (isnan(humidity) || isnan(temperature)) {
    return;
   }

 // make TCP connections
   WiFiClient client;
   const int httpPort = 80;
   if (!client.connect(host, httpPort)) {
    return; 
   }

   String url = "/update?key=";
   url+=writeAPIKey;
   url+="&field1=";   
   url+=String(temperature);
   url+="&field2=";
   url+=String(humidity);
   url+="\r\n";

   // Request to the server
   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
   Serial.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
     delay(1000);           

}
