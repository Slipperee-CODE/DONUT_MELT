#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

// Used in header file and c file
#include "c_pico_dshot.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "donut_config.h"

void motor_init_all(int dshot_speed, int motor1_pin, PIO motor1_pio, int motor2_pin, PIO motor2_pio, bot_state_t* user_bot_state);

void motor_stop_all();

void motor_send_all_starting_zero_command();

void motor_motor1_send_throttle(uint16_t throttle);

void motor_motor2_send_throttle(uint16_t throttle);

void motor_update_bot_state();

#endif