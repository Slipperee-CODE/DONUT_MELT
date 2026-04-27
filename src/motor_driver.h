#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "c_pico_dshot.h"
#include "donut_config.h"

void motor_init_all(int dshot_speed, int motor1_pin, PIO motor1_pio, int motor2_pin, PIO motor2_pio, bot_state_t* user_bot_state);

void motor_motor1_set_throttle(uint16_t throttle);

void motor_motor2_set_throttle(uint16_t throttle);

void motor_set_throttle_for_all(uint16_t throttle);

void motor_stop_all();

void motor_update_bot_state();

#endif