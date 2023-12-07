#include "SineSweep.h"
#include "Arduino.h"
#include "SimpleFOC.h"

SineSweep::SineSweep() {
    min_frequency=5;
    max_frequency=100;
    amplitude = 0.1;
    steps = 20;
    cycles_per_step = 50;
    _motor = nullptr;
}

void SineSweep::linkMotor(FOCMotor *motor) {
    _motor = motor;
}
void SineSweep::linkSerial(Stream *serial) {
    _serial = serial;
}

void SineSweep::Execute() {
    _serial->printf("Starting Sine Sweep\n");
    _serial->printf("%i Hz to %i Hz\n", (int)min_frequency, (int)max_frequency);
    _serial->printf("%.1f rad amplitude\n", amplitude);    
    _serial->printf("%.1f frequency steps, %.1f cycles per step\n", steps, cycles_per_step);
     
    float starting_pos = _motor->target;
    for (int i=0; i<steps; i++) {
        float freq = min_frequency + (max_frequency - min_frequency)*i/steps;
        // _serial->printf("Frequency: %i\n", (int)freq);

        float motor_setpoint = 0;
        float motor_velocity = 0;
        float time_angle=0;
        float max_time = 1e6*cycles_per_step/freq;
        uint32_t time = _micros();
        float dt = 0;
        float sa = 0;
        uint32_t prev_time = time;

        float max_motor_angle = _motor->sensor->getAngle();
        float min_motor_angle = _motor->sensor->getAngle();

        while (time_angle < cycles_per_step*_2PI) {
            time = _micros();    
            dt = time - prev_time;
            prev_time = time;

            time_angle += (dt/max_time)* cycles_per_step*_2PI;
            motor_setpoint = 0.5*amplitude*_sin(time_angle) + starting_pos;
            // motor_velocity = amplitude * _cos(time_angle);

            float angle = _motor->sensor->getAngle();
            if (angle > max_motor_angle) {
                max_motor_angle = angle;
            } else if (angle < min_motor_angle) {
                min_motor_angle = angle;
            }
            
            _motor->feed_forward_velocity = motor_velocity;
            _motor->target = motor_setpoint;
            _motor->move();
            _motor->loopFOC();
        }
        float resp_amp = max_motor_angle - min_motor_angle;
        float resp_gain = resp_amp/amplitude;

        _serial->printf("%.3f Hz - Gain: %.3f - Amplitude: %.3f\n", freq, resp_gain, resp_amp);
    }
    SIMPLEFOC_DEBUG("Finished with Sine Sweep");
}

void SineSweep::Execute(char* c) {
    Execute();
}

void SineSweep::setMaxFreq(float f) {
    max_frequency = f;
}
void SineSweep::setMinFreq(float f) {
    min_frequency = f;
}
void SineSweep::setAmp(float a) {
    amplitude = a;
}
void SineSweep::setSteps(float s) {
    steps=s;
}
void SineSweep::setCyclesPerStep(float c) {
    cycles_per_step = c;
}
