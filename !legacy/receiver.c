#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "receiver.h"
#include "donut_config.h"
#include "crsf.h"

channel_t right_stick_x = {
    .raw_ticks = 0,
    .channel_id = 0,
    .is_percent_signed = false
};

channel_t right_stick_y = {
    .raw_ticks = 0,
    .channel_id = 1,
    .is_percent_signed = false
};

channel_t left_stick_y = {
    .raw_ticks = 0,
    .channel_id = 2,
    .is_percent_signed = false
};

channel_t left_stick_x = {
    .raw_ticks = 0,
    .channel_id = 3,
    .is_percent_signed = false
};

void update_channel_t_data(channel_t* channel, const uint16_t* channels){
    channel->raw_ticks = channel->raw_ticks + 1; //channels[channel->channel_id];
    if (channel->is_percent_signed){
        channel->percent_of_max = channel->raw_ticks / 2047.0f;
    } else {
        channel->percent_of_max = (1/1023.5f) * channel->raw_ticks - 1;
    }
}

void on_rc_channels(const uint16_t channels[16]) {
    //printf("channels changed! \n");
    
    update_channel_t_data(&right_stick_x, channels);
    update_channel_t_data(&right_stick_y, channels);
    update_channel_t_data(&left_stick_y, channels);
    update_channel_t_data(&left_stick_x, channels);
}

void on_link_stats(const link_statistics_t link_stats) {
    //printf("linked stats triggered! \n");
}

void on_failsafe(const bool failsafe) {
    //printf("failsafe triggered! \n");
}

void receiver_setup(){
    crsf_set_link_quality_threshold(CRSF_LINK_QUALITY_THRESHOLD);
    crsf_set_rssi_threshold(CRSF_RSSI_THRESHOLD);

    crsf_set_on_rc_channels(on_rc_channels);
    crsf_set_on_link_statistics(on_link_stats);
    crsf_set_on_failsafe(on_failsafe);

    crsf_begin(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN); //this function sets up GPIOs for uart
}