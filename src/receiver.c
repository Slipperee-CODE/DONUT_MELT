#include "receiver.h"

void on_rc_channels(const uint16_t channels[16]){ // Figure out what to do with this func either in donut_melt.c or here

}

void on_link_stats(const link_statistics_t link_stats){ // Figure out what to do with this func either in donut_melt.c or here

}

void on_failsafe(const bool failsafe){ // Figure out what to do with this func either in donut_melt.c or here
    
}

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin, int link_quality_threshold, int rssi_threshold){
    crsf_set_link_quality_threshold(link_quality_threshold);
    crsf_set_rssi_threshold(rssi_threshold);

    crsf_set_on_rc_channels(on_rc_channels);
    crsf_set_on_link_statistics(on_link_stats);
    crsf_set_on_failsafe(on_failsafe);

    crsf_begin(uart_id, tx_pin, rx_pin);
}

void receiver_update(){
    crsf_process_frames();
}