#include "receiver.h"

channel_t right_joystick_x = {
    .raw_ticks = 0
};

channel_t right_joystick_y = {
    .raw_ticks = 0
};

channel_t left_joystick_y = {
    .raw_ticks = 0
};

channel_t left_joystick_x = {
    .raw_ticks = 0
};

channel_t switch_e = {
    .raw_ticks = 0
};

channel_t switch_b = {
    .raw_ticks = 0
};

channel_t switch_c = {
    .raw_ticks = 0
};

channel_t switch_f = {
    .raw_ticks = 0
};

channel_t knob_s1 = {
    .raw_ticks = 0
};

channel_t knob_s2 = {
    .raw_ticks = 0
};

uint32_t time_of_last_update;

void on_update_rc_channels(packed_payload_t* channels) {
    time_of_last_update = to_ms_since_boot(get_absolute_time());

    right_joystick_x.raw_ticks = channels->channel0;
    right_joystick_y.raw_ticks = channels->channel1;
    left_joystick_y.raw_ticks = channels->channel2;
    left_joystick_x.raw_ticks = channels->channel3;
    switch_e.raw_ticks = channels->channel4;
    switch_b.raw_ticks = channels->channel5;
    switch_c.raw_ticks = channels->channel6;
    switch_f.raw_ticks = channels->channel7;
    knob_s1.raw_ticks = channels->channel8;
    knob_s2.raw_ticks = channels->channel9;
}

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin){
    crsf_init(uart_id, tx_pin, rx_pin, on_update_rc_channels);
    time_of_last_update = 0;
}

void receiver_update(){
    crsf_read_incoming_frames();
}

uint8_t receiver_check_if_disconnected(){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());
    if (curr_time - time_of_last_update > RECEIVER_TIMEOUT_MS){
        return 1;
    }
    return 0;
}