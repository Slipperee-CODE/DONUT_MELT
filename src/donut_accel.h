#ifndef DONUT_ACCEL_H
#define DONUT_ACCEL_H

#include "donut_config.h"

typedef struct {
    float x;
    float y;
} Vector2D;

void accel_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl, bot_state_t* user_bot_state);

double get_rpm(double right_x_percent, double accel_offset_cm);

double get_fake_rpm(double right_x_percent, double accel_offset_cm);

double get_rpm_diag_accel(double right_x_percent, double accel_offset_cm);

#endif