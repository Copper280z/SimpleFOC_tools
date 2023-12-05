
#ifndef _OBSERVER_H
#define _OBSERVER_H

#include <BasicLinearAlgebra.h>

constexpr int n_states = 3;

//Luenberger Observer
class Observer {
    public:
    
        // <rows, cols>
        BLA::Matrix<n_states,n_states, float> A;
        BLA::Matrix<n_states,1, float> B;
        BLA::Matrix<n_states,1, float> C;
        BLA::Matrix<n_states,1, float> D;
        BLA::Matrix<n_states,1, float> L;
    
        BLA::Matrix<n_states,1, float> x_hat;
        float d_hat=0;
        float y_hat=0;

        Observer(float J, float b, float K, float R, float L);

        Observer(BLA::Matrix<n_states,n_states> A, 
                BLA::Matrix<n_states,1> B, 
                BLA::Matrix<n_states,1> C, 
                BLA::Matrix<n_states,1> L);

        // u and y
        void update(float control_input, float sensor_value, float dt);

        float getSensorValue();

};
#endif
