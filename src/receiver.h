#ifndef RECEIVER_H
#define RECEIVER_H

// Used in header file and c file
#include <stdint.h>
#include "hardware/uart.h"

// Used in c file
#include "donut_config.h"
#include "crsf.h"
#include "math.h"

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

uint16_t receiver_get_channel(Channel channel);

double receiver_get_percent_for_channel(Channel channel);

uint8_t receiver_is_channel_near_value(Channel channel, uint16_t value, uint16_t tolerance);

void receiver_send_telemetry(uint16_t diag1, uint16_t diag2, uint32_t diag3, uint8_t diag4);

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin, int link_quality_threshold, int rssi_threshold, bot_state_t* user_bot_state);

void receiver_update();

#endif