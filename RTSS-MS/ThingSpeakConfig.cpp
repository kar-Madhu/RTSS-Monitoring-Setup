#include "ThingSpeakConfig.h"
#include "WiFiConfig.h"
#include <WiFi.h>
#include <ThingSpeak.h>

//Setting up WiFi and connecting with ThingSpeak

WiFiClient client;

void setupWiFi() {
  unsigned long startAttemptTime = millis();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Failed. Check credentials.");
    return;
  }

  Serial.println("\nWiFi connected.");
  ThingSpeak.begin(client);
}

//Uploading data to ThingSpeak

void pushToThingSpeak(float T, float H, float S_TH, float S_D){
  ThingSpeak.setField(1, T);
  ThingSpeak.setField(2, H);
  ThingSpeak.setField(3, S_TH);
  ThingSpeak.setField(4, S_D);

  int x = ThingSpeak.writeFields(channelID, writeAPIkey);
  if (x == 200) {
    Serial.println("Data uploaded to ThingSpeak");
  } else {
    Serial.printf("Upload failed. Error: %d\n", x);
  }
}