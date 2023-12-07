#pragma once
#include "arm_math.h"

extern void _sincos(float angle, float *st, float *ct);
// extern float abs(float a);

class AFC {
    public:

    AFC(float k_afc, float h_afc);

    float k_afc=0;
    float h_afc=0;
    float afc_out=0;
    float thetadot=1;
    float afc_cos_int=0;
    float afc_sin_int=0;

    float update(float angle, float err, float Ts);
};