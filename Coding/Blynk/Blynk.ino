
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>

#include "DHT.h"
#define DHTPIN D7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#include <BlynkSimpleEsp8266.h>


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "xxxxx";
char pass[] = "xxxxx";

int LED = D8; // Define LED as an Integer (whole numbers) and pin D8 on Wemos D1 Mini Pro

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxx";


BlynkTimer timer;


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  
  delay (2000);
}

void setup()
{
  // Debug console
   // initialize sensor
 Serial.begin(115200);
  dht.begin();
 pinMode(LED, OUTPUT); //Set the LED (D8) as an output
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
}

void loop()
{
  Blynk.run();
  timer.run();
  sendSensor();
}

// This function will be called every time button Widget
// in Blynk app writes values to the Virtual Pin V3
BLYNK_WRITE(V3) {
 int pinValue = param.asInt(); // Assigning incoming value from pin V3 to a variable
 if (pinValue == 1) {
    digitalWrite(LED, LOW); // Turn fan on.
  } else {
    digitalWrite(LED, HIGH); // Turn fan off.
 }
}
