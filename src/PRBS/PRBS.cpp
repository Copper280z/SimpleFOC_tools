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
    float start_target = _motor->target;
    switch(testMode) {
        case prbs::qvoltage_encoder: _motor->torque_controller = TorqueControlType::voltage; break;
        case prbs::ptarget_encoder: break; // do nothing, assume correct torque controller is already set
    }
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
            val = ((float) newbit - 0.5f) * amplitude;
        }

        switch(testMode) {
            case prbs::qvoltage_encoder: update_qvoltage_encoder(val, newbit, start_time) ; break;
            case prbs::ptarget_encoder: update_ptarget_encoder(val, newbit, start_time) ; break;
        }
    }
    // _serial->printf("PBRS done\n");

    _motor->torque_controller = old_controller;
    _motor->target = start_target;
    // SIMPLEFOC_DEBUG("Finished with PBRS");
}

void PRBS::update_qvoltage_encoder(float input_val, uint16_t newbit, uint32_t start_time) {
    _motor->sensor->update();
    float electrical_angle = _motor->electricalAngle();
    _motor->setPhaseVoltage(input_val, 0, electrical_angle);
    _serial->printf("%d,%hd,%.6f\r\n", micros()-start_time, newbit, _motor->shaftAngle());
}

void PRBS::update_ptarget_encoder(float input_val, uint16_t newbit, uint32_t start_time) {
    _motor->target = input_val;
    _motor->move();
    _motor->loopFOC();

    _serial->printf("%d,%.2f,%.3f,%.6f\r\n", micros()-start_time, input_val, _motor->current_sp, _motor->shaftAngle());
}


void PRBS::Execute_args(char* c) {
    size_t length = strlen(c);
    // if (length > 2) {
    //     _serial->printf("Unrecognized syntax recieved %d characters.\nargs:", length);
    _serial->println(c);

    // } else {
    switch (c[0]) {
        case (int) '0': testMode = prbs::Mode::qvoltage_encoder; break;
        case (int) '1': testMode = prbs::Mode::ptarget_encoder; break;
        case (int) 'A':
            float amp_val = strtod(&c[1],NULL);
            if (amp_val < 1e-8f) return;
            amplitude = amp_val;
            return;
            break;

    }

    // }
    Execute();   
    // }
}

void PRBS::setAmp(float a) {
    amplitude = a;
}
