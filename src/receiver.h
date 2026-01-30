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

extern channel_t left_joystick_x;
extern channel_t left_joystick_y;
extern channel_t right_joystick_x;
extern channel_t right_joystick_y;
extern channel_t switch_b;
extern channel_t switch_c;
extern channel_t switch_e;
extern channel_t switch_f;
extern channel_t knob_s1;
extern channel_t knob_s2;

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin);

uint8_t receiver_check_if_disconnected();

#endif