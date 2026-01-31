#include "receiver.h"

channel_t right_joystick_x = {
    .raw_ticks = 0,
    .channel_id = RIGHT_JOYSTICK_X_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t right_joystick_y = {
    .raw_ticks = 0,
    .channel_id = RIGHT_JOYSTICK_Y_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t left_joystick_y = {
    .raw_ticks = 0,
    .channel_id = LEFT_JOYSTICK_Y_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};


channel_t left_joystick_x = {
    .raw_ticks = 0,
    .channel_id = LEFT_JOYSTICK_X_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t switch_e = {
    .raw_ticks = 0,
    .channel_id = SWITCH_E_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t switch_b = {
    .raw_ticks = 0,
    .channel_id = SWITCH_B_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t switch_c = {
    .raw_ticks = 0,
    .channel_id = SWITCH_C_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t switch_f = {
    .raw_ticks = 0,
    .channel_id = SWITCH_F_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t knob_s1 = {
    .raw_ticks = 0,
    .channel_id = KNOB_S1_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

channel_t knob_s2 = {
    .raw_ticks = 0,
    .channel_id = KNOB_S2_CHANNEL,
    .percent_of_max = 0,
    .is_percent_signed = false
};

uint32_t time_of_last_update;

void update_channel_t_data(channel_t* channel, uint16_t raw_ticks){
    channel->raw_ticks = raw_ticks;
    if (channel->is_percent_signed){
        channel->percent_of_max = (1/1023.5f) * channel->raw_ticks - 1;
    } else {
        channel->percent_of_max = channel->raw_ticks / 2047.0f;
    }
}

void on_update_rc_channels(packed_payload_t* channels) {
    time_of_last_update = to_ms_since_boot(get_absolute_time());

    update_channel_t_data(&right_joystick_x, channels->channel0);
    update_channel_t_data(&right_joystick_y, channels->channel1);
    update_channel_t_data(&left_joystick_y, channels->channel2);
    update_channel_t_data(&left_joystick_x, channels->channel3);
    update_channel_t_data(&switch_b, channels->channel4);
    update_channel_t_data(&switch_c, channels->channel5);
    update_channel_t_data(&switch_e, channels->channel6);
    update_channel_t_data(&switch_f, channels->channel7);
    update_channel_t_data(&knob_s1, channels->channel8);
    update_channel_t_data(&knob_s2, channels->channel9);
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