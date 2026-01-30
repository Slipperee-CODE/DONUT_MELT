#include "receiver.h"


channel_t right_joystick_x = {
    .raw_ticks = 0,
    .channel_id = RIGHT_JOYSTICK_X_CHANNEL,
    .is_percent_signed = false
};

channel_t right_joystick_y = {
    .raw_ticks = 0,
    .channel_id = RIGHT_JOYSTICK_Y_CHANNEL,
    .is_percent_signed = false
};

channel_t left_joystick_x = {
    .raw_ticks = 0,
    .channel_id = LEFT_JOYSTICK_X_CHANNEL,
    .is_percent_signed = false
};

channel_t left_joystick_y = {
    .raw_ticks = 0,
    .channel_id = LEFT_JOYSTICK_Y_CHANNEL,
    .is_percent_signed = false
};

channel_t switch_e = {
    .raw_ticks = 0,
    .channel_id = SWITCH_E_CHANNEL,
    .is_percent_signed = false
};

channel_t switch_b = {
    .raw_ticks = 0,
    .channel_id = SWITCH_B_CHANNEL,
    .is_percent_signed = false
};

channel_t switch_c = {
    .raw_ticks = 0,
    .channel_id = SWITCH_C_CHANNEL,
    .is_percent_signed = false
};

channel_t switch_f = {
    .raw_ticks = 0,
    .channel_id = SWITCH_F_CHANNEL,
    .is_percent_signed = false
};

channel_t knob_s1 = {
    .raw_ticks = 0,
    .channel_id = KNOB_S1_CHANNEL,
    .is_percent_signed = false
};

channel_t knob_s2 = {
    .raw_ticks = 0,
    .channel_id = KNOB_S2_CHANNEL,
    .is_percent_signed = false
};

uint32_t time_of_last_update;

void update_channel_t_data(channel_t* channel, const uint16_t* channels){
    channel->raw_ticks = channels[channel->channel_id];
    if (channel->is_percent_signed){
        channel->percent_of_max = channel->raw_ticks / 2047.0f;
    } else {
        channel->percent_of_max = (1/1023.5f) * channel->raw_ticks - 1;
    }
}

void on_update_rc_channels(const uint16_t channels[16]) {
    time_of_last_update = to_ms_since_boot(get_absolute_time());

    update_channel_t_data(&left_joystick_x, channels);
    update_channel_t_data(&left_joystick_y, channels);
    update_channel_t_data(&right_joystick_x, channels);
    update_channel_t_data(&right_joystick_y, channels);
    update_channel_t_data(&switch_b, channels);
    update_channel_t_data(&switch_c, channels);
    update_channel_t_data(&switch_e, channels);
    update_channel_t_data(&switch_f, channels);
    update_channel_t_data(&knob_s1, channels);
    update_channel_t_data(&knob_s2, channels);
}

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin){
    crsf_init(uart_id, tx_pin, rx_pin, on_update_rc_channels);
}

uint8_t receiver_check_if_disconnected(){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());
    if (curr_time - time_of_last_update > RECEIVER_TIMEOUT_MS){
        time_of_last_update = curr_time;
        return 1;
    }
    return 0;
}