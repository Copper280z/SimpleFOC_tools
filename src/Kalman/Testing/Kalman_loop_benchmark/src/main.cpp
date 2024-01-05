// #include <Arduino.h>
// #include "../../../Kalman.h"

// Kalman<1,1,4, float> Kf = Kalman<1,1,4, float>();

// void setup() {
//   Serial.begin(115200);

//   // mass of load, kg
//   float M2 = 0.8;
//   // friction for load
//   float b2 = 0.1;
//   // spring rate of belt, guess 10 N stretches 0.5mm, N/m - gates says 18000 lb/in/in for 1" wide belt, some paper says 55N/mm
//   float Ks = 45000;
//   // friction internal to belt
//   float b3 = 1;
//   // radius of belt pulley, m
//   float r = 0.005;
//   // inertia of motor
//   float J = 5e-5;
//   // friction in motor
//   float b1 = 2e-4;
//   // motor constant
//   float Km = 0.041;
//   // // phase resistance
//   // float R = 5.6;
//   // // phase inductance
//   // float L = 0.0045;

//   Kf.F = {0.0f,      1.0f,              0.0f,          0.0f,
//         -Ks/M2,  -(b2+b3)/M2,    Ks*r/M2,    (b3*r)/M2,
//         0.0f,       0.0f,              0.0f,          1.0f,
//         Ks*r/J,  (b3*r)/J,       -Ks*r*r/J,  (-b1-b3*r*r)/J};

//   Kf.B = {0, 0, 0, Km/J};
//   Kf.H = {0, 0, 1, 0};
//   Kf.Q = {0.000001, 0, 0, 0,
//          0, 0.00001, 0, 0,
//          0, 0, 0.1, 0,
//          0, 0, 0, 0.00001};
//   Kf.R = { pow( 2.0f*3.14f/(pow(2.0f,14.0f)), 2.0f)};

//   Serial.println("Starting Kalman Filter Benchmark!");
// }

// void loop() {
//   BLA::Matrix<1> z = {0.25f};
//   BLA::Matrix<1> u = {0.0f};

//   uint32_t t_prev = micros();
//   uint32_t t_start = micros();
//   for (int i = 0; i<1000; i++) {
//     uint32_t t_now = micros();
//     float dt = (t_now-t_prev) * (float) 1e-6;
//     Kf.update(dt, z, u);
//     t_prev = t_now;
//   }
//   uint32_t t_finish = micros();
//   float loop_time = (t_finish-t_start)*(float)1e-6;
//   Serial.printf("Kalman Filter Loop rate: %.3fkHz - %.3fms per iteration - ", 1/loop_time, loop_time);
//   Serial.printf("x_hat: %.3f, %.3f, %.3f, %.3f\n", Kf.x_hat(0),Kf.x_hat(1),Kf.x_hat(2),Kf.x_hat(3));


// }
