#pragma once

#include "Arduino.h"
#include "SimpleFOC.h"


class SineSweep {
    public:
        SineSweep();
        
        float min_frequency=10;
        float max_frequency=200;
        float amplitude = 0.5;
        float steps = 50;
        float cycles_per_step = 50;

        void Execute();
        void Execute(char* c);
        void linkMotor(FOCMotor *motor);
        void linkSerial(Stream *serial);

        void setMaxFreq(float f);
        void setMinFreq(float f);
        void setAmp(float a);
        void setSteps(float s);
        void setCyclesPerStep(float c);


    private:
        FOCMotor *_motor;
        Stream *_serial=&Serial;

};

