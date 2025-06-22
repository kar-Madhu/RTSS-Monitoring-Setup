#ifndef SENSORS_H
#define SENSORS_H

void setupSensors();
void ultrasonic();
float getT();
float getH();
extern const int echo;

#endif