#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

// Used in header file and c file
#include "c_pico_dshot.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "donut_config.h"

void motor_init_all();

void motor_stop_all();

void motor1_send_throttle(uint16_t throttle);

void motor2_send_throttle(uint16_t throttle);

#endif