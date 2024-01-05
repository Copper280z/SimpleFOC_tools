// #include <Arduino.h>
// // #include "../../../BinaryUART.h"
// #include <SerialCommands.h>

// void send_array(Stream *ser, float* arr, int num) {
//     uint32_t num_bytes = sizeof(arr[0])*num;
//     ser->printf("sending %d bytes:", num_bytes);
//     for (int i=0; i<num; i++) {
//         ser->write(arr[i]);
//     }
    
// }

// float array[4096];
// char serial_command_buffer_[32];
// SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

// void cmd_unrecognized(SerialCommands* sender, const char* cmd)
// {
//   sender->GetSerial()->print("Unrecognized command [");
//   sender->GetSerial()->print(cmd);
//   sender->GetSerial()->println("]");
// }

// void cmd_send_arr(SerialCommands* sender) {
//   send_array(&Serial, array, 4096);
// }

// SerialCommand cmd_send_("send", cmd_send_arr);

// void setup() {
//   Serial.begin(115200);
//   serial_commands_.SetDefaultHandler(cmd_unrecognized);
//   serial_commands_.AddCommand(&cmd_send_);

//   for (int i=0; i<4096; i++) {
//     array[i] = (float) i;
//   }

// }

// void loop() {
//   serial_commands_.ReadSerial();

// }
