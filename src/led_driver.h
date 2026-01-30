#pragma once

// Used in header file and c file
#include <stdint.h>

// Used in c file
#include "hardware/gpio.h"
#include "donut_config.h"

void led_init(uint8_t pin);

void led_repeat_blink(uint8_t repeats);

void led_time_blink(int millis);