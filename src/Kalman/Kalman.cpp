// #include "Kalman.h"


// Kalman::Kalman() {
//     F.Fill(0);
//     B.Fill(0);
//     H.Fill(0);
//     Q.Fill(0);
//     R.Fill(0);

//     P = BLA::Eye<n_states, n_states>();
//     P_hat = BLA::Eye<n_states, n_states>();
//     I = BLA::Eye<n_states, n_states>();
// }

// Kalman::Kalman(Matrix<n_states, n_states> _F, Matrix<n_states, n_inputs> _B, Matrix<n_states, n_outputs> _H, Matrix<n_states, n_states> _Q, Matrix<n_outputs, n_outputs> _R) {

//     F=_F;
//     B=_B;
//     H=_H;
//     Q=_Q;    
//     R=_R;

//     P = BLA::Eye<n_states, n_states>();
//     P_hat = BLA::Eye<n_states, n_states>();
//     I = BLA::Eye<n_states, n_states>();

// }

// float* Kalman::update(float dt, float[n_outputs] z, float[n_inputs] u) {
//     xp_hat = xp_hat + (A*x_hat + B*u)*dt;
//     P_hat = F*P*(~F) + Q;
//     S = H*P_hat*(~H) + R;
//     K = P_hat*(~H)*Invert(S);
//     P = (I-K*H)*P_hat;
//     y = z-(H*x_hat);
//     x_hat = xp_hat + K*y;

//     return x_hat.storage;
// }