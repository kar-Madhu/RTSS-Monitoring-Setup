#include "sensors.h"
#include <DHT.h>

//Set GPIO pins here. You can even change DHT11 to DHT22 if using one without medelling with the entire codeflow
#define DHT_Type DHT11
const int dht_pin = 5;
const int trigger = 26;
const int echo = 27;

DHT dht(dht_pin, DHT_Type);


//Setting up DHT and HC-SR04
void setupSensors(){
  dht.begin();
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(echo, INPUT);
}

//get T and H from DHT

float getT(){
  float T = dht.readTemperature();
  if(isnan(T)) return -1;
  return T;
}

float getH(){
  float H = dht.readHumidity();
  if(isnan(H)) return -1;
  return H;
}

//Transmit-Receive thingy on ultrasonic sensor
void ultrasonic(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
}