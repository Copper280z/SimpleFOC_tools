#pragma once
#include "BasicLinearAlgebra.h"

using namespace BLA;
template <int n_inputs, int n_outputs, int n_states>
class Kalman {
    public:
        Matrix<n_states> x_hat;

        // System Matrix, defines time update behavior
        Matrix<n_states, n_states> F;
        // Input Matrix
        Matrix<n_states, n_inputs> B;
        // Measurement Matrix
        Matrix<n_states, n_outputs> H;
        // Process Noise
        Matrix<n_states, n_states> Q;
        // Sensor Noise, stdev^2
        Matrix<n_outputs, n_outputs> R;
        // Measurement Matrix
        Matrix<n_outputs> y;

        // performs the filter update at a timestep
        float* update(float dt, float[n_outputs] z, float[n_inputs] u);
    private:
        Matrix<n_states> xp_hat;
        Matrix<n_states, n_states> I;
        Matrix<n_states, n_states> P;
        Matrix<n_states, n_states> P_hat;
        Matrix<n_states, n_inputs> K;
}