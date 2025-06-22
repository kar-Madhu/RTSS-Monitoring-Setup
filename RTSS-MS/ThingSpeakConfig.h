#ifndef THING_SPEAK_CONFIG_H
#define THING_SPEAK_CONFIG_H

void setupWiFi();
void pushToThingSpeak(float T, float H, float S_TH, float S_D);

#endif