#include "Observer.h"


Observer::Observer(float inertia, float damping_coeff, float torque_constant, float phase_resistance, float inductance) {
    A.Fill(0);
    B.Fill(0);
    C.Fill(0);
    D.Fill(0);
    L.Fill(0);
    x_hat.Fill(0);

    L = {1e-6,1e-6,1e-6};

    A = {0, 1, 0,
        0, -damping_coeff/inertia, torque_constant/inertia,
        0, -torque_constant/inductance, -phase_resistance/inductance};

    B = {0, 0, 1/inductance};
    C = {1, 0, 0};  

}

Observer::Observer(BLA::Matrix<n_states,n_states> A,
            BLA::Matrix<n_states,1> B,
            BLA::Matrix<n_states,1> C,
            BLA::Matrix<n_states,1> L) {

}

void Observer::update(float control_input, float sensor_value, float dt) {
    
    // update the disturbance estimate before we update the estimated state
    d_hat = sensor_value - y_hat;

    // update the estimated state
    auto control_update = (B*control_input)*dt;
    auto L_update = L*(sensor_value-y_hat);
    auto time_update = (A*x_hat)*dt;

    // x_hat = A*x_hat + B*control_input + L*(sensor_value - y_hat);
    x_hat += time_update + control_update + L_update;

    // update the sensor estimate
    // BLA::Matrix<n_states, 1, float> y_full = C*x_hat;
    
    // y_hat = y_full(0,0);
    y_hat = x_hat(0);
}

float Observer::getSensorValue() {
    return y_hat;
}
