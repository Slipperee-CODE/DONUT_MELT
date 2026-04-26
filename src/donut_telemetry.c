#include "donut_telemetry.h"

uint32_t get_seconds_since_boot() {
    return to_ms_since_boot(get_absolute_time()) / 1000.0;
}

void telemetry_output_diagnostics(bot_state_t* bot_state) {
    printf("\n---------DIAGNOSTICS START-------------\n\n");

    #ifdef TIME_SINCE_BOOT_DIAGNOSTICS
        printf("DIAGNOSTICS AT %d SECS SINCE BOOT \n\n", get_seconds_since_boot());
    #endif

    #ifdef ACCEL_DIAGNOSTICS
        printf("rpm: %d | ", bot_state->rpm);
        printf("accel_g_value: %d | ", bot_state->accel_g_value);
        printf("accel_offset_cm: %lf \n\n", bot_state->accel_offset_cm);
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
uint8_t get_telemetry_state() {
    if (receiver_get_channel(SWITCH_F) >= RECEIVER_HIGHEST_CHANNEL_VALUE){
        return TELEMETRY_MOTOR1;
    } else if (receiver_get_channel(SWITCH_F) <= RECEIVER_LOWEST_CHANNEL_VALUE){
        return TELEMETRY_MAIN;
    } else {
        return TELEMETRY_MOTOR2;
    }
}

void telemetry_send_telemetry(bot_state_t* bot_state) {
    switch(get_telemetry_state()){
        case TELEMETRY_MOTOR1:

            #ifdef LIE_ABOUT_RPM
                int areWeLyingAboutRPM = 1*10; // the *10 is so it shows up as 1.0 on the transmitter
            #else
                int areWeLyingAboutRPM = 0;
            #endif

            receiver_send_telemetry(areWeLyingAboutRPM,bot_state->accel_g_value,bot_state->rpm,1);
            break;
        case TELEMETRY_MOTOR2:
            // multiplying by 10 to get the numbers to show up as 1.0, 2.0, etc. on the transmitter
            receiver_send_telemetry(donut_get_curr_drive_mode()*10,donut_is_killswitch_active()*10,bot_state->max_rpm,2);
            break;
        case TELEMETRY_MAIN:
            receiver_send_telemetry(bot_state->require_zero_throttle*10,bot_state->is_failsafed*10,(uint32_t) (bot_state->accel_offset_cm*10000),3);
            break;
    }
}
