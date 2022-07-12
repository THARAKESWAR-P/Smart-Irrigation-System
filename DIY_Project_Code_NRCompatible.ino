#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

//DHT sensor
DHT dht(DHTPIN, DHTTYPE);

//Ultrasonic Sensor
int Trig = 9;
int Echo = 8;
int Duration;
float Distance;


//moisture sensor
int water_count = 0;

//relay
const int relay = 3;

void setup() {
  Serial.begin(9600);

  //relay
  pinMode(relay, OUTPUT);
  
  //DHT11
  dht.begin();

  //Ultrasonic Sensor
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}

void loop() {
  
  //Check temperature, humidity, and soil humidity once every 5 seconds.
  checkWater();
  checkMoisture();
  checkDHT11();
  delay(5000);
}

  
//Ultrasonic Sensor
void checkWater(){
  //Check the water level in the bucket.
  digitalWrite(Trig,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  Duration = pulseIn(Echo,HIGH);
  if (Duration > 0){
    Distance = Duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    Serial.print(Distance);
    Serial.print(",");
  }
}

//moisture sensor
void checkMoisture()
{
  //Measure soil humidity
  int moisture = analogRead(A0);
  Serial.print(moisture);
  Serial.print(",");
  if(moisture > 600)
  {
    water_count++;
    if(water_count == 5)
    { //To wait for the water to go through the pot.
      digitalWrite(relay, HIGH);
      delay(2000);
      digitalWrite(relay, LOW);
      delay(4000); 
      water_count = 0;
    }
  }
  if(moisture <= 600)
  {
    digitalWrite(relay, HIGH); //This should be low but strangely enough, when it was set to LOW then the relay was switching on and when its set to HIGH its switching off
    delay(500);
  }
}

//DHT11
void checkDHT11(){
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Get data from DHT11 sensor
  float h = dht.readHumidity();   //%
  float t = dht.readTemperature();    //Celsius
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  float hic = dht.computeHeatIndex(t, h, false);    //false here, is used so that heat index is calculated in Celsius

  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.println(hic);
}
