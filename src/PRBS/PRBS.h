#pragma once

#include "Arduino.h"
#include "SimpleFOC.h"
#include <inttypes.h>

namespace prbs {
    constexpr uint32_t duration = 250000;
    constexpr uint32_t downsample = 10;
}
class PRBS {
    public:
        PRBS();

        float amplitude = 6.0;
        

        void Execute();
        void Execute(char* c);
        void linkMotor(FOCMotor *motor);
        void linkSerial(Stream *serial);

        void setAmp(float a);
        void reset();


    private:

        FOCMotor *_motor;
        Stream *_serial=&Serial;

};

