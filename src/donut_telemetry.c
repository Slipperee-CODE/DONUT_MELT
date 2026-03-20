#include "donut_telemetry.h"

uint32_t get_seconds_since_boot(){
    return to_ms_since_boot(get_absolute_time()) / 1000.0;
}

void output_diagnostics(bot_state_t* bot_state){
    printf("\n---------DIAGNOSTICS START-------------\n\n");

    #ifdef TIME_SINCE_BOOT_DIAGNOSTICS
        printf("DIAGNOSTICS AT %d SECS SINCE BOOT \n\n", get_seconds_since_boot());
    #endif

    #ifdef FULL_CONTROLLER_DIAGNOSTICS
        printf("--------FULL CONTROLLER INFO---------\n");
        printf("RIGHT_JOYSTICK_X: %d | ", receiver_get_channel(RIGHT_JOYSTICK_X));
        printf("RIGHT_JOYSTICK_Y: %d \n", receiver_get_channel(RIGHT_JOYSTICK_Y));
        printf("LEFT_JOYSTICK_Y: %d | ", receiver_get_channel(LEFT_JOYSTICK_Y));
        printf("LEFT_JOYSTICK_X: %d \n", receiver_get_channel(LEFT_JOYSTICK_X));
        printf("SWITCH_E: %d | ", receiver_get_channel(SWITCH_E));
        printf("SWITCH_B: %d | ", receiver_get_channel(SWITCH_B));
        printf("SWITCH_C: %d | ", receiver_get_channel(SWITCH_C));
        printf("SWITCH_F: %d \n", receiver_get_channel(SWITCH_F));
        printf("KNOB_S1: %d | ", receiver_get_channel(KNOB_S1));
        printf("KNOB_S2: %d \n\n", receiver_get_channel(KNOB_S2));
    #endif

    #ifdef CRSF_DIAGNOSTICS
        printf("--------CRSF INFO---------\n");
        printf("crsf_link_quality: %d \n", bot_state.crsf_link_quality);
        printf("crsf_rssi: %d \n", bot_state.crsf_rssi);
        printf("crsf_snr: %d \n", bot_state.crsf_snr);
        printf("crsf_tx_power: %d \n\n", bot_state.crsf_tx_power);
    #endif

    #ifdef MOTOR1_DIAGNOSTICS
        printf("--------MOTOR1 TELEMETRY INFO---------\n");
        printf("bidir_m1_erpm: %d | ", bot_state.bidir_m1_erpm);
        printf("bidir_m1_voltage: %d | ", bot_state.bidir_m1_voltage);
        printf("bidir_m1_current: %d \n", bot_state.bidir_m1_current);
        printf("bidir_m1_temperature: %d | ", bot_state.bidir_m1_temperature);
        printf("bidir_m1_status: %d | ", bot_state.bidir_m1_status);
        printf("bidir_m1_stress: %d \n\n", bot_state.bidir_m1_stress);
    #endif

    #ifdef MOTOR2_DIAGNOSTICS
        printf("--------MOTOR2 TELEMETRY INFO---------\n");
        printf("bidir_m2_erpm: %d | ", bot_state.bidir_m2_erpm);
        printf("bidir_m2_voltage: %d | ", bot_state.bidir_m2_voltage);
        printf("bidir_m2_current: %d \n", bot_state.bidir_m2_current);
        printf("bidir_m2_temperature: %d | ", bot_state.bidir_m2_temperature);
        printf("bidir_m2_status: %d | ", bot_state.bidir_m2_status);
        printf("bidir_m2_stress: %d \n\n", bot_state.bidir_m2_stress);
    #endif

    #ifdef OTHER_DIAGNOSTICS
        printf("--------OTHER INFO---------\n");
        printf("is_failsafed: %d \n", bot_state->is_failsafed);
        printf("require_zero_throttle: %d \n", bot_state->require_zero_throttle);
        printf("get_curr_drive_mode: %d \n", drive_get_curr_drive_mode());
        printf("is_killswitch_active: %d \n", donut_is_killswitch_active());
        printf("is_throttle_zero: %d \n", donut_is_throttle_zero());
        printf("left_stick_y_percent: %lf \n", receiver_get_percent_for_channel(LEFT_JOYSTICK_Y));
        printf("right_stick_y_percent: %lf \n\n", receiver_get_percent_for_channel(RIGHT_JOYSTICK_Y));
    #endif

    printf("---------DIAGNOSTICS END-------------\n\n");
}

// this function and send_telemetry work together to switch between different sets of telemetry to send to transmitter 
uint8_t get_telemetry_state(){
    if (receiver_get_channel(SWITCH_F) >= RECEIVER_HIGHEST_CHANNEL_VALUE){
        return TELEMETRY_MOTOR1;
    } else if (receiver_get_channel(SWITCH_F) <= RECEIVER_LOWEST_CHANNEL_VALUE){
        return TELEMETRY_MAIN;
    } else {
        return TELEMETRY_MOTOR2;
    }
}

void send_telemetry(bot_state_t* bot_state){
    switch(get_telemetry_state()){
        case TELEMETRY_MOTOR1:
            printf("SENDING MOTOR1");
            receiver_send_telemetry(1,1,1,1);
            break;
        case TELEMETRY_MOTOR2:
            printf("SENDING MOTOR2");
            receiver_send_telemetry(2,2,2,2);
            break;
        case TELEMETRY_MAIN:
            printf("SENDING MAIN");
            receiver_send_telemetry(bot_state->require_zero_throttle,bot_state->is_failsafed,get_seconds_since_boot(),3);
            break;
    }
}