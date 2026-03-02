#ifndef DONUT_DRIVE_H
#define DONUT_DRIVE_H

// Used in c file
#include "motor_driver.h"
#include "led_driver.h"
#include "donut_config.h"
#include "pico/time.h"

#include <stdio.h>

void drive_handle_idle();

void drive_handle_spin(double throttle);

void drive_handle_tank(double left_y, double right_y);

#endif