#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "donut_config.h"
#include "hardware/gpio.h"

void led_init(uint8_t pin, uint8_t pin2);

void led_set_and_update_state(uint8_t state);

void led_time_blink(uint32_t millis);

void led_repeat_blink(uint8_t repeats);

#endif