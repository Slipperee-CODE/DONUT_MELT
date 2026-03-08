#include "receiver.h"

static uint16_t _user_channels[16];
static bot_state_t* _user_bot_state;
static crsf_instance _user_crsf_instance;

void on_rc_channels(const uint16_t channels[16]){
    for (int i = 0; i < 16; i++){
        _user_channels[i] = channels[i];
    }
}

uint16_t receiver_get_channel(uint16_t channel){
    return _user_channels[channel];
}

void on_link_stats(const link_statistics_t link_stats){
    _user_bot_state->crsf_link_quality = link_stats.link_quality;
    _user_bot_state->crsf_link_quality = link_stats.rssi;
    _user_bot_state->crsf_snr = link_stats.snr;
    _user_bot_state->crsf_tx_power = link_stats.tx_power;
}

void on_failsafe(const bool failsafe){
    _user_bot_state->is_failsafed = failsafe;
}

void receiver_send_telemetry(uint16_t diag1, uint16_t diag2, uint32_t diag3, uint8_t diag4){
    crsf_telem_set_battery_data(&_user_crsf_instance, 3, 2, 1, 50);
    //crsf_telem_set_battery_data(&_user_crsf_instance, diag1, diag2, diag3, diag4);
}

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin, int link_quality_threshold, int rssi_threshold, bot_state_t* user_bot_state){
    crsf_init(&_user_crsf_instance);

    crsf_set_link_quality_threshold(&_user_crsf_instance, link_quality_threshold);
    crsf_set_rssi_threshold(&_user_crsf_instance, rssi_threshold);

    crsf_set_on_rc_channels(&_user_crsf_instance, on_rc_channels);
    crsf_set_on_link_statistics(&_user_crsf_instance, on_link_stats);
    crsf_set_on_failsafe(&_user_crsf_instance, on_failsafe);

    crsf_begin(&_user_crsf_instance, uart_id, tx_pin, rx_pin);

    _user_bot_state = user_bot_state;
}

void receiver_update(){
    crsf_process_frames(&_user_crsf_instance);
    //printf("RECEIVER UPDATING");
}
