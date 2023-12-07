#include "AFC.h"

AFC::AFC(float k, float h) {
    k_afc = k;
    h_afc = h;
}

float AFC::update(float angle, float err, float Ts) {

    float ct;
    float st;
    _sincos(h_afc*angle, &st, &ct);

    if (abs(thetadot)>1e-3) {
        afc_cos_int += Ts * err * k_afc*ct/thetadot;
        afc_sin_int += Ts * err * k_afc*st/thetadot;
    }
    afc_out = thetadot*(st*afc_sin_int + ct*afc_cos_int);

    return afc_out;
}