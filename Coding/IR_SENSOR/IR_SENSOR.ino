int LED = 14; // Use the onboard Wemos LED
int isObstaclePin = 4;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE



void setup() {
  pinMode(LED, OUTPUT);//Declare LED as output
  pinMode(isObstaclePin, INPUT);//Declare input
  Serial.begin(115200);//setup baud rate
}



void loop() {
  //if IR sensor send LOW signal, LED ON
  isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW)
  {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(LED, HIGH);
  }
  //if IR sensor send HIGH signal, LED OFF
  else
  {
    Serial.println("clear");
    digitalWrite(LED, LOW);
  }
  delay(200);
}
