#ifndef DONUT_DRIVE_H
#define DONUT_DRIVE_H

// Used in c file
#include "motor_driver.h"
#include "led_driver.h"
#include "receiver.h"
#include "receiver.h"
#include "donut_config.h"
#include "pico/time.h"
#include "math.h"
#include "H3LIS331DL.h"

#include <stdio.h>

uint8_t drive_get_curr_drive_mode();

void drive_update_bot_state(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent);

#endif