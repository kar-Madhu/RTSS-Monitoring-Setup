//read README.txt first to understand the calcuations made.

#include <esp_timer.h>
#include <DHT.h>

const int DHTPIN = 23;
#define DHTTYPE DHT11

const int trigger = 27;
const int echo = 25;
const float dist = 25.6;

//set distance according to your setup

volatile int64_t start = 0;
volatile int64_t end = 0;
volatile bool ready = false;

DHT dht(DHTPIN, DHTTYPE);

//using software interrupts to measure time accurately

void IRAM_ATTR timeRec() {
  if (digitalRead(echo)) {
    start = esp_timer_get_time();
  } else {
    end = esp_timer_get_time();
    ready = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  attachInterrupt(digitalPinToInterrupt(echo), timeRec, CHANGE);
  dht.begin();
  Serial.println("Type 'x' to take 10 readings.");
}

void RTSS() {

  //Ultrasonic sensor thingy
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  delay(50);

  float T = dht.readTemperature(); //degree celcius
  float H = dht.readHumidity(); //%age relative humdity


  float time_sec = (end - start) / 1e6; //time in microsec = seconds * 10^-6
  float correction = sqrt(sq(dist) + sq(1.05)); //still in cm
  float calculated_speed = 0.02 * correction / time_sec; //multiplied by 2, cuz two-way trip; divided by 100, cuz cm --> m

  float theoretical_speed = 331.3 + (0.606*T) + (0.0124*H);

  Serial.print(calculated_speed, 2);
  Serial.print(" | ");
  Serial.println(theoretical_speed, 2);

  ready = false;
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "x") {
      for (int i = 0; i < 10; i++) {
        RTSS();
        delay(2000);
      }
    }
  }
}
