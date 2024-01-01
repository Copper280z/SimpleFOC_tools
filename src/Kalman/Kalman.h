#pragma once
#pragma GCC optimize (3)
#include "BasicLinearAlgebra.h"

// using namespace BLA;
template <int n_inputs, int n_outputs, int n_states, typename T>
class Kalman {
    public:
        BLA::Matrix<n_states, 1, T> x_hat;

        // System Matrix, defines time update behavior
        BLA::Matrix<n_states, n_states, T> F;
        // Input Matrix
        BLA::Matrix<n_states, n_inputs, T> B;
        // Measurement Matrix
        BLA::Matrix<n_inputs, n_states, T> H;
        // Process Noise
        BLA::Matrix<n_states, n_states, T> Q;
        // Sensor Noise, stdev^2
        BLA::Matrix<n_outputs, n_outputs, T> R;
        BLA::Matrix<n_outputs, n_outputs, T> S;
        BLA::Matrix<n_outputs, n_outputs, T> Sinv;
        // Measurement Matrix
        BLA::Matrix<n_outputs, 1, T> y;

        // constructor
        Kalman() {
            F.Fill(0);
            B.Fill(0);
            H.Fill(0);
            Q.Fill(0);
            R.Fill(0);

            // work around for bug in BLA::Eye
            P = BLA::Zeros<n_states, n_states, T>();
            P_hat = BLA::Zeros<n_states, n_states, T>();
            I = BLA::Zeros<n_states, n_states, T>();

            for (int i=0; i<n_states; i++){
                P(i,i) = 1;
                P_hat(i,i) = 1;
                I(i,i) = 1;
            }
        }

        Kalman(BLA::Matrix<n_states, n_states, T> _F, BLA::Matrix<n_states, n_inputs, T> __B, BLA::Matrix<n_states, n_outputs, T> _H, BLA::Matrix<n_states, n_states, T> _Q, BLA::Matrix<n_outputs, n_outputs, T> _R) {

            F=_F;
            B=__B;
            H=_H;
            Q=_Q;    
            R=_R;
            
            // work around for bug in BLA::Eye
            P = BLA::Zeros<n_states, n_states, T>();
            P_hat = BLA::Zeros<n_states, n_states, T>();
            I = BLA::Zeros<n_states, n_states, T>();

            for (int i=0; i<n_states; i++){
                P(i,i) = 1;
                P_hat(i,i) = 1;
                I(i,i) = 1;
            }

        }

        // performs the filter update at a timestep
        void update(float dt, BLA::Matrix<n_outputs,1,T> z, BLA::Matrix<n_inputs,1,T> u) {
            auto Fd = I+(F*dt);
            auto Bd = B*dt;
            xp_hat = Fd*x_hat + Bd*u;
            P_hat = Fd*P*(~Fd) + Q;
            S = H*P_hat*(~H) + R;

            K = P_hat*(~H)*BLA::Inverse(S);
            auto KH = -K*H;
            for (int i=0;i<n_states;i++){
                KH(i,i)+=1;
            }
            P = KH*P_hat;
            // P = (I-K*H)*P_hat;
            y = z-(H*x_hat);
            x_hat = xp_hat + K*y;
            // return x_hat;
        };
    private:
        BLA::Matrix<n_states, 1, T> xp_hat;
        BLA::Matrix<n_states, n_states, T> I;
        BLA::Matrix<n_states, n_states, T> P;
        BLA::Matrix<n_states, n_states, T> P_hat;
        BLA::Matrix<n_states, n_inputs, T> K;
};