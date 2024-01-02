#include "BinaryUART.h"

void send_array(Stream *ser, float* arr, int num) {
    uint32_t num_bytes = sizeof(arr[0])*num;
    ser->printf("sending %d bytes:", num_bytes);
    for (int i=0; i<num; i++) {
        ser->write(arr[i]);
    }
    
}

