#ifndef DONUT_ACCEL_H
#define DONUT_ACCEL_H

#include "donut_config.h"

typedef struct {
    float x;
    float y;
} Vector2D;

void accel_init(accelerometer_t* accel_1, accelerometer_t* accel_2, bot_state_t* user_bot_state);

double get_rpm(double right_x_percent, double accel_offset_cm);

double get_fake_rpm(double right_x_percent, double accel_offset_cm);

double get_rpm_2accel(double right_x_percent, double accel_offset_cm);

#endif