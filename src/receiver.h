#ifndef RECEIVER_H
#define RECEIVER_H

// Used in header file and c file
#include <stdint.h>
#include "hardware/uart.h"

// Used in c file
#include "donut_config.h"
#include "crsf.h"

typedef enum {
    RIGHT_JOYSTICK_X,
    RIGHT_JOYSTICK_Y,
    LEFT_JOYSTICK_Y,
    LEFT_JOYSTICK_X,
    SWITCH_E,
    SWITCH_B,
    SWITCH_C,
    SWITCH_F,
    KNOB_S1,
    KNOB_S2
} Channel;

uint16_t receiver_get_channel(uint16_t channel);

void receiver_send_telemetry(uint8_t *data, uint8_t data_length);

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin, int link_quality_threshold, int rssi_threshold, bot_state_t* user_bot_state);

void receiver_update();

#endif