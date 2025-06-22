#include "WiFiConfig.h"
#include "sensors.h"
#include "calculations.h"
#include "ThingSpeakConfig.h"

#define DISTANCE_CM 25.6                                          //change distance according to your setup
volatile uint64_t start = 0;
volatile uint64_t end = 0;
volatile bool echo_received = false;


//ISR function to measure Δt in uSeconds
void IRAM_ATTR timeRec(){
  if (digitalRead(echo) == HIGH) {                              
    start = esp_timer_get_time();                                  //starts measuring time with the first edge
  } else {
    end = esp_timer_get_time();                                    //end
    echo_received = true;
  }
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing RTSS-MS");
  
  setupSensors();                                                   //Sets up DHT 11 and HC-SR04
  setupWiFi();
  attachInterrupt(digitalPinToInterrupt(echo), timeRec, CHANGE);    //Assigining ISR funcn. to echo pin

  Serial.println("!!RTSS-MS READY!!");                              //Starts displaying on serial monitor data regardless WiFi connected or not
}

void loop(){
  float T = getT();                                                 //Fetch real-time temperature readings in °C from DHT11
  float H = getH();                                                 //Fetch real-time humdity readings in %age relative humidity from DHT11

  if (T < 0 || H < 0) {                                             //Displays message if sensor(s) are not wired properly
    Serial.println("Sensor(s) not detected.");
    delay(10000);
    return;
  }

  ultrasonic();                                                      //Let HC-SR04 send and receive an ultrasonic echo

  float S_TH = getS_TH(T, H);                                        //Fetch realt-time speed of sound based on Temperature and Humdity using Wong-Embleton Empirical Scale Formula
  float S_D = getS_D(start, end, DISTANCE_CM);                       //Fetch realt-time speed of sound based on distance from reflecting surface and time of flight

  Serial.printf("\nT: %.3f °C, H: %.3f %%\n", T, H);                 //Printing Temperature & Humidity readings in real time
  Serial.printf("S(T,H): %.3f m/s, S(D,Δt): %.3f m/s\n", S_TH, S_D); //Printing calculated S(T,H) and S(D,Δt)

  if (alignmentCheck(S_D, S_TH)){                                    //Misalignment check, to avoid uploading wrong data to ThingSpeak 
    Serial.println("Alignment good. Uploading to ThingSpeak...");
    pushToThingSpeak(T, H, S_TH, S_D);                               //Uploading to ThingSpeak
  } else {
    Serial.println("Misalignment detected — skipping upload.");
  }

  delay(15001);                                                       //ThingSpeak free tier update rate limit - 15sec; you can change/remove if you have bought a license
}