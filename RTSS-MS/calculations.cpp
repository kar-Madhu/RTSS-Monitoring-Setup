#include "calculations.h"
#include <math.h>
#include <Arduino.h>

const int D = 25.6;

float getS_D(uint64_t start, uint64_t end, float D){
    double time = 0.5*(end - start)/1e6;               //calculation of Δt, based on ISR function
    float D_corrected = sqrt(sq(D) + sq(1.3))/100;     //Correction using Pythagoras formula, offset between transmitter and recieiver is ~2.6cm 

    return D_corrected/time;                           // speed = distance/time
}

float getS_TH(float T, float H){

    //Wong-Embleton Empirical Scale calculation

    float factor = (H / 100.0) * (
        0.000966 +
        0.000072 * T +
        0.0000018 * T * T +
        0.000000072 * T * T * T +
        0.000000000065 * T * T * T * T
    );

    return (331.3 + 0.606 * T) * (1 + factor);
}

bool alignmentCheck(float S_D, float S_TH){            //Difference in readings is exponential if alignment is wrong, either it will be close to ~0.2% or straight up 60%
    return fabs(S_TH - S_D) <= 10;                     //So we'll keep a difference of 10m/s as a safe standard, anything beyond that means alignment error.
}
