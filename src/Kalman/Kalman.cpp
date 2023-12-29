#include "Kalman.h"

float* Kalman::update(float dt, float[n_outputs] z, float[n_inputs] u) {
    xp_hat = xp_hat + (A*x_hat + B*u)*dt;
    P_hat = F*P*(~F) + Q
    S = H*P_hat*(~H) + R
    K = P_hat*(~H)*Invert(S)
    P = (I-K*H)*P_hat
    y = z-(H*x_hat)
    x_hat = xp_hat + K*y

    return x_hat.storage;
}