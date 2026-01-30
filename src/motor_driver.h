#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

// Used in header file and c file
#include "c_pico_dshot.h"
#include "hardware/pio.h"

extern DShotEncoderInstance* MOTOR1;
extern DShotEncoderInstance* MOTOR2;

void motor_init_all(int motor1_pin, pio_hw_t* motor1_pio, int motor2_pin, pio_hw_t* motor2_pio);

void motor_stop_all();

void motor_send_throttle(DShotEncoderInstance* motor, double t);

#endif