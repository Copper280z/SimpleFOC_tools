#pragma once

#include "Arduino.h"
#include "SimpleFOC.h"
#include <inttypes.h>

namespace prbs {
    constexpr uint32_t duration = 250000;
    constexpr uint32_t downsample = 10;

    enum Mode {
        qvoltage_encoder,
        qvoltage_qcurrent,
        dvoltage_dcurrent,
        qcurrent_encoder,
        dcurrent_encoder,
        ptarget_encoder,
    };

}
class PRBS {
    public:
        PRBS();

        float amplitude = 6.0;

        void Execute();
        void Execute_args(char* c);
        void linkMotor(FOCMotor *motor);
        void linkSerial(Stream *serial);

        void setAmp(float a);
        void reset();

        prbs::Mode testMode = prbs::qvoltage_encoder;

        // the following functions write the random input to the hardware
        // and print data output on the linked serial port
        // naming convention is update_input_output
        void update_qvoltage_encoder(float input_val, uint16_t newbit, uint32_t start_time);
        void update_ptarget_encoder(float input_val, uint16_t newbit, uint32_t start_time);

    private:

        FOCMotor *_motor;
        Stream *_serial=&Serial;

};

