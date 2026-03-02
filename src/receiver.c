#include "receiver.h"

static uint16_t* _user_channels;
static bot_state_t* _user_bot_state;

void on_rc_channels(const uint16_t channels[16]){
    for (int i = 0; i < 16; i++){
        _user_channels[i] = channels[i];
    }
}

uint16_t receiver_get_channel(uint16_t channel){
    return _user_channels[channel];
}

void on_link_stats(const link_statistics_t link_stats){ // Figure out what to do with this func either in donut_melt.c or here
    _user_bot_state->crsf_link_quality = link_stats.link_quality;
    _user_bot_state->crsf_link_quality = link_stats.rssi;
    _user_bot_state->crsf_snr = link_stats.snr;
    _user_bot_state->crsf_tx_power = link_stats.tx_power;
}

void on_failsafe(const bool failsafe){
    _user_bot_state->is_failsafed = failsafe;
}

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin, int link_quality_threshold, int rssi_threshold, bot_state_t* user_bot_state){
    crsf_set_link_quality_threshold(link_quality_threshold);
    crsf_set_rssi_threshold(rssi_threshold);

    crsf_set_on_rc_channels(on_rc_channels);
    crsf_set_on_link_statistics(on_link_stats);
    crsf_set_on_failsafe(on_failsafe);

    crsf_begin(uart_id, tx_pin, rx_pin);

    _user_bot_state = user_bot_state;
}

void receiver_update(){
    crsf_process_frames();
}
