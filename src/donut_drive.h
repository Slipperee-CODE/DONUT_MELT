#ifndef DONUT_DRIVE_H
#define DONUT_DRIVE_H

// Used in c file
#include "motor_driver.h"
#include "led_driver.h"
#include "donut_config.h"
#include "pico/time.h"

void drive_handle_idle();

void drive_handle_spin();

void drive_handle_tank();

#endif