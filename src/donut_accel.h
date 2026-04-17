#ifndef DONUT_ACCEL_H
#define DONUT_ACCEL_H

#include "donut_config.h"

void accel_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl);

double get_rpm(double right_x_percent);

double get_fake_rpm(double right_x_percent);

#endif