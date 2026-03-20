#ifndef LED_DRIVER_H
#define LED_DRIVER_H

// Used in header file and c file
#include <stdint.h>

// Used in c file
#include "hardware/gpio.h"
#include "donut_config.h"

void led_init(uint8_t pin);

void led_reset();

void led_time_blink_on_off(uint32_t millis_on, uint32_t millis_off);

void led_time_blink(uint32_t millis);

void led_repeat_blink(uint8_t repeats);

#endif