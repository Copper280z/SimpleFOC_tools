#include "PRBS.h"
#include "Arduino.h"
#include "SimpleFOC.h"

PRBS::PRBS() {
    _motor = nullptr;
}

void PRBS::linkMotor(FOCMotor *motor) {
    _motor = motor;
}
void PRBS::linkSerial(Stream *serial) {
    _serial = serial;
}

void PRBS::Execute() {
    // _serial->printf("Starting PRBS plant identification\n");
    
    TorqueControlType old_controller = _motor->torque_controller;
    _motor->torque_controller = TorqueControlType::voltage;

    uint16_t lfsr=7654;

    uint32_t start_time = micros();
    float val = 0;
    uint16_t newbit = 0;
    // run motor
    for (int i=0; i<prbs::duration; i++) {
        if (i%prbs::downsample == 0) {
            // 2047 position prbs
            newbit = (((lfsr >> 5) ^ (lfsr >> 7)) & 1);
            lfsr = ((lfsr >> 1) | newbit<<15);
            
            // 32767 position prbs
            // uint16_t newbit = (((lfsr >> 14) ^ (lfsr >> 13)) & 1);
            // lfsr = ((lfsr << 1) | newbit) & 0x7fff;

            val = ((float) newbit - 0.5f) * amplitude;
        }

        _motor->feedforward_voltage.q = val;
        _motor->loopFOC();
        _serial->printf("%d,%hd,%.6f\r\n", micros()-start_time, newbit, _motor->shaftAngle());
        // _serial->printf("%d,%.2f,%d,%d\r\n", micros()-start_time, val, newbit, lfsr);
    }
    // _serial->printf("PBRS done\n");

    _motor->torque_controller = old_controller;
    // SIMPLEFOC_DEBUG("Finished with PBRS");
}

void PRBS::Execute(char* c) {
    Execute();
}

void PRBS::setAmp(float a) {
    amplitude = a;
}
