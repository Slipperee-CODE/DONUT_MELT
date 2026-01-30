#ifndef RECEIVER_H
#define RECEIVER_H

// Used in header file and c file
#include <stdint.h>
#include <stdbool.h>
#include "hardware/uart.h"

// Used in c file
#include "pico/time.h"
#include "crsf.h"
#include "donut_config.h"

typedef struct {
    volatile uint16_t raw_ticks;
    uint8_t channel_id;
    volatile double percent_of_max;
    bool is_percent_signed; 
} channel_t;

channel_t left_joystick_x;
channel_t left_joystick_y;
channel_t right_joystick_x;
channel_t right_joystick_y;
channel_t switch_b;
channel_t switch_c;
channel_t switch_e;
channel_t switch_f;
channel_t knob_s1;
channel_t knob_s2;

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin);

uint8_t receiver_check_if_disconnected();

#endif