#pragma once

// Used in c file
#include "motor_driver.h"
#include "pico/time.h"

void drive_handle_idle();

void drive_handle_spin();

void drive_handle_tank();