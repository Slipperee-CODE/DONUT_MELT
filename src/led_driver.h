#pragma once

// Used in c file
#include "hardware/gpio.h"

void led_init(int pin);

void led_repeat_blink(int repeats);

void led_time_blink(int millis);