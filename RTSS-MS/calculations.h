#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include <stdint.h>

float getS_D(uint64_t start, uint64_t end, float D);
float getS_TH(float T, float H);
bool alignmentCheck(float S_D, float S_TH);

#endif