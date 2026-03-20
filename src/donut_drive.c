#include "donut_drive.h"

uint8_t drive_get_curr_drive_mode(){
    if (receiver_is_channel_near_value(SWITCH_C, RECEIVER_HIGHEST_CHANNEL_VALUE, 10)){
        return DRIVE_MODE_TANK;
    }
    return DRIVE_MODE_MELTY;
}

void handle_idle(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent){
    led_repeat_blink(5);
    motor_stop_all();
}

// gets microseconds per rotation
// lies about upr depending on what right_x_percent is and what LEFT_RIGHT_HEADING_CONTROL_DIVISOR is to adjust direction
uint64_t get_adjusted_upr(double right_x_percent){
    // get rpm from accel data along correct axis, WILL PROBABLY NEED TO CHANGE THE AXIS LATER
    double x_gs = accelerometer_get_x();

    // mapping [0,1] -> [-1,1]
    right_x_percent = (right_x_percent - 0.5) * 2;

    double effective_radius_in_cm = ACCEL_MOUNT_RADIUS_CM + (ACCEL_MOUNT_RADIUS_CM * right_x_percent * LEFT_RIGHT_HEADING_CONTROL_DIVISOR);

    double rpm = fabs(get_accel_force_g() - ACCEL_ZERO_G_OFFSET) * 89445.0f;
    rpm = rpm / effective_radius_in_cm;
    rpm = sqrt(rpm);

    uint64_t upr = (uint64_t) (60000000/rpm);
    return upr;
}

// goes from microseconds per rotation -> rpm
uint16_t upr_to_rpm(uint64_t upr){
    return (uint16_t) (60000000 / upr);
}

void handle_spin_led(uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, uint64_t led_on_us){
    // time_elapsed_this_rotation_us == 0 is "forward" for the sake of spin led purposes

    // if facing left side or right side of 0 within led_on_us/2 us
    if (time_elapsed_this_rotation_us >= us_per_rotation - led_on_us/2 || 
        time_elapsed_this_rotation_us <= led_on_us/2) {
        led_set_and_update_state(1);
    } else {
        led_set_and_update_state(0);
    }
}

void handle_spin_forward(bot_state_t* bot_state, double left_y_percent, uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, double half_rotation_time, double motor_off_edge_time){

    // fix this: it might not work as intended, it's attempting to turn on motors only on right side of "forward"/0 direction
    if (time_elapsed_this_rotation_us >= motor_off_edge_time &&
        time_elapsed_this_rotation_us <= half_rotation_time - motor_off_edge_time){
        handle_tank(bot_state, left_y_percent, 0, 0);
    } else if (time_elapsed_this_rotation_us >= half_rotation_time + motor_off_edge_time &&
        time_elapsed_this_rotation_us <= us_per_rotation - motor_off_edge_time){
        handle_tank(bot_state, 0, left_y_percent, 0);    
    }
}

void handle_spin_backward(bot_state_t* bot_state, double left_y_percent, uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, double half_rotation_time, double motor_off_edge_time){

    // fix this: this needs to be fixed to actually go backwards
    // would need to turn motors on only on left side of "forward"/0 direction
    if (time_elapsed_this_rotation_us >= motor_off_edge_time &&
        time_elapsed_this_rotation_us <= half_rotation_time - motor_off_edge_time){
        handle_tank(bot_state, left_y_percent, 0, 0);
    } else if (time_elapsed_this_rotation_us >= half_rotation_time + motor_off_edge_time &&
        time_elapsed_this_rotation_us <= us_per_rotation - motor_off_edge_time){
        handle_tank(bot_state, 0, left_y_percent, 0);    
    }
}

void handle_spin(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent){
    static uint8_t no_translation_state_counter = 0;

    // don't do any real calculations or attempt translating if we haven't reached a good rpm yet
    uint64_t us_per_rotation = get_adjusted_upr(right_x_percent);
    bot_state->rpm = upr_to_rpm(us_per_rotation);

    // keeping track of max rpm
    if (bot_state->rpm > bot_state->max_rpm){
        bot_state->max_rpm = bot_state->rpm;
    }

    // if we aren't fast enough to translate, spin up as fast as possible
    if (upr_rpm(us_per_rotation) < MIN_TRANSLATION_RPM){
        handle_tank(bot_state, 1, 1, 0);
        led_set_and_update_state(1);
        return;
    }
    
    if (bot_state->this_rotations_start_time_us == -1){
        bot_state->this_rotations_start_time_us = time_us_64();
    }

    uint64_t time_elapsed_this_rotation_us = time_us_64() - bot_state->this_rotations_start_time_us;
    uint64_t led_on_us = fmin(MAX_LED_PERCENT_DURATION, fmax(MIN_LED_PERCENT_DURATION, (int64_t) (left_y_percent * us_per_rotation)));

    double half_rotation_time = (double) us_per_rotation/2;
    double motor_off_edge_time = (half_rotation_time - MOTOR_ON_PERCENT_DURATION*us_per_rotation)/2;

    if (right_y_percent > 0.75) {
        handle_spin_forward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
    } else if (right_y_percent < 0.25) {
        handle_spin_backward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
    } else {
        if (no_translation_state_counter){
            handle_spin_forward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
        } else {
            handle_spin_backward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
        }
    }

    handle_spin_led(time_elapsed_this_rotation_us, us_per_rotation, led_on_us);

    // if we have completed a rotation, get ready for next rotation
    if (time_elapsed_this_rotation_us >= us_per_rotation) {
        time_elapsed_this_rotation_us = 0;
        no_translation_state_counter = 1 - no_translation_state_counter;
    }
}

void handle_tank(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent){
    // fixing ramping such that (0.5,1] increases throttle in one direction and [0.5,0] increases throttle in the other
    if (left_y_percent < 0.5) {
        left_y_percent = 0.499 - left_y_percent; // doing 0.499 so 0.0 is fullest throttle possible
    } 

    if (right_y_percent < 0.5) {
        right_y_percent = 0.499 - right_y_percent;
    } 

    motor_motor1_set_throttle((uint16_t) 2000*left_y_percent);
    motor_motor2_set_throttle((uint16_t) 2000*right_y_percent);
}

void drive_update_bot_state(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent){
    if (donut_is_throttle_zero()){
        handle_idle(bot_state, left_y_percent, right_y_percent, right_x_percent);
        return;
    }
    
    if (drive_get_curr_drive_mode() == DRIVE_MODE_MELTY){
        handle_spin(bot_state, left_y_percent, right_y_percent, right_x_percent);
    } else {
        led_repeat_blink(3);

        // restricting allowed standard tank drive values so that robot can be more controlled hopefully
        left_y_percent = fmin(0.5 + TANK_DRIVE_THROTTLE_MAX_REGISTERED_DEVIATION_FROM_CENTER, fmax(0.5 - TANK_DRIVE_THROTTLE_MAX_REGISTERED_DEVIATION_FROM_CENTER, left_y_percent));
        right_y_percent = fmin(0.5 + TANK_DRIVE_THROTTLE_MAX_REGISTERED_DEVIATION_FROM_CENTER, fmax(0.5 - TANK_DRIVE_THROTTLE_MAX_REGISTERED_DEVIATION_FROM_CENTER, right_y_percent));

        handle_tank(bot_state, left_y_percent, right_y_percent, right_x_percent);
    }
}