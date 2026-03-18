#include "donut_drive.h"

uint8_t drive_get_curr_drive_mode(){
    if (receiver_is_channel_near_value(SWITCH_C, RECEIVER_HIGHEST_CHANNEL_VALUE, 10)){
        return DRIVE_MODE_TANK;
    }
    return DRIVE_MODE_MELTY;
}

uint16_t get_throttle_from_channel(Channel channel){
    return (uint16_t) (2000 * receiver_get_percent_for_channel(channel));
}

void drive_update_bot_state(bot_state_t* bot_state){
    uint16_t left_throttle = get_throttle_from_channel(LEFT_JOYSTICK_Y);
    uint16_t right_throttle = get_throttle_from_channel(RIGHT_JOYSTICK_Y);

    if (is_throttle_zero()){
        handle_idle(bot_state, left_throttle, right_throttle);
        return;
    }

    if (get_curr_drive_mode() == DRIVE_MODE_MELTY){
        handle_spin(bot_state, left_throttle);
    } else {
        drive_handle_tank(bot_state, left_throttle, right_throttle);
    }
}

void handle_idle(bot_state_t* bot_state, uint16_t left_throttle, uint16_t right_throttle){
    led_repeat_blink(5);
    motor_stop_all();
}

void handle_spin(bot_state_t* bot_state, uint16_t throttle){
    led_repeat_blink(2);
    motor_motor1_send_throttle(throttle);
    motor_motor2_send_throttle(throttle);
}

void drive_handle_tank(bot_state_t* bot_state, uint16_t left_throttle, uint16_t right_throttle){
    led_repeat_blink(3);
    motor_motor1_send_throttle(left_throttle);
    motor_motor2_send_throttle(right_throttle);
}