#include "donut_drive.h"

// takes into account curr_drive_mode whereas donut_is_throttle_zero does not 
uint8_t drive_is_throttle_zero() {
    if (donut_get_curr_drive_mode() == DRIVE_MODE_MELTY) {
        return donut_is_throttle_zero();
    }

    if (donut_get_curr_drive_mode() == DRIVE_MODE_TANK) {
        return donut_is_throttle_zero() && receiver_is_channel_near_value(RIGHT_JOYSTICK_Y, RECEIVER_MIDDLEST_CHANNEL_VALUE, 300);
    }
}

uint8_t is_close_enough(double num1, double num2, double tolerance) {
    return fabs(num1 - num2) <= tolerance;
}

// goes from rpm -> microseconds per rotation
double rpm_to_upr(double rpm) {
    if (rpm == 0) {
        return 0; 
    }
    return (uint64_t) 60000000.0 / rpm;
}

double percentThrottleToThrottleCommand(double percent_throttle) {
    uint16_t throttle;

    // -1..0 -> 1047..0 AND 0..1 -> 1049..2047
    // 0 percent_throttle or 0.5 percent_throttle becomes a 0 throttle command
    if (percent_throttle > 0) {
        throttle = 1049 + 998*percent_throttle;
    } else if (percent_throttle < 0) {
        throttle = -1047*percent_throttle;
    } else {
        throttle = 0;
    }

    return throttle;
}

void handle_idle(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent) {
    led_repeat_blink(5);
    motor_stop_all();
}

void handle_tank(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent) {
    uint16_t left_throttle = percentThrottleToThrottleCommand(left_y_percent);
    uint16_t right_throttle = percentThrottleToThrottleCommand(right_y_percent);

    #ifndef NO_MOTOR_SPINNING
        motor_motor1_set_throttle(left_throttle);
        motor_motor2_set_throttle(right_throttle);
    #endif
}

void handle_spin_led(uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, uint64_t led_on_us) {
    // time_elapsed_this_rotation_us == 0 is "forward" for the sake of spin led purposes

    // if facing left side or right side of 0 within led_on_us/2 us
    if (time_elapsed_this_rotation_us >= us_per_rotation - led_on_us/2 || 
        time_elapsed_this_rotation_us <= led_on_us/2) {
        led_set_and_update_state(1);
    } else {
        led_set_and_update_state(0);
    }
}

void handle_spin_forward(bot_state_t* bot_state, double left_y_percent, uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, double half_rotation_time, double motor_off_edge_time) {
    // pretty sure this is correct but could def be a source of problems - Cai

    if (time_elapsed_this_rotation_us >= motor_off_edge_time &&
        time_elapsed_this_rotation_us <= half_rotation_time - motor_off_edge_time) {
        handle_tank(bot_state, left_y_percent, 0, 0);
    } else if (time_elapsed_this_rotation_us >= half_rotation_time + motor_off_edge_time &&
        time_elapsed_this_rotation_us <= us_per_rotation - motor_off_edge_time) {
        handle_tank(bot_state, 0, left_y_percent, 0);    
    }
}

// like handle_spin_forwards but motors turn on and off in opposite order as handle_spin_forwards 
void handle_spin_backward(bot_state_t* bot_state, double left_y_percent, uint64_t time_elapsed_this_rotation_us, uint64_t us_per_rotation, double half_rotation_time, double motor_off_edge_time) {
    // pretty sure this is correct but could def be a source of problems - Cai
    
    if (time_elapsed_this_rotation_us >= motor_off_edge_time &&
        time_elapsed_this_rotation_us <= half_rotation_time - motor_off_edge_time) {
        handle_tank(bot_state, 0, left_y_percent, 0);
    } else if (time_elapsed_this_rotation_us >= half_rotation_time + motor_off_edge_time &&
        time_elapsed_this_rotation_us <= us_per_rotation - motor_off_edge_time) {
        handle_tank(bot_state, left_y_percent, 0, 0);    
    }
}

void handle_spin(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent, double (*get_rpm)(double)) {
    static uint8_t no_translation_state_counter = 0;

    double rpm = get_rpm(right_x_percent);
    bot_state->rpm = rpm;

    // keeping track of max rpm
    if (bot_state->rpm > bot_state->max_rpm) {
        bot_state->max_rpm = bot_state->rpm;
    }

    // if we aren't fast enough to translate, spin up as fast as possible (can be bypassed)
    #ifndef BYPASS_MIN_TRANSLATION_RPM
        if (rpm < MIN_TRANSLATION_RPM) {
            handle_tank(bot_state, 1, 1, 0);
            led_set_and_update_state(1);
            return;
        }
    #endif
    
    if (bot_state->this_rotations_start_time_us == -1) {
        bot_state->this_rotations_start_time_us = time_us_64();
    }

    double us_per_rotation = rpm_to_upr(rpm);
    double time_elapsed_this_rotation_us = time_us_64() - bot_state->this_rotations_start_time_us;

    // probably should just make it so that reversing spin direction is another button altogether, not just backwards throttle, oops - Cai
    double led_on_us = fmin(MAX_LED_PERCENT_DURATION, fmax(MIN_LED_PERCENT_DURATION, (int64_t) (2*fabs(left_y_percent - 0.5) * us_per_rotation)));

    double half_rotation_time = (double) us_per_rotation/2;
    double motor_off_edge_time = (half_rotation_time - MOTOR_ON_PERCENT_DURATION*us_per_rotation)/2;

    if (is_close_enough(right_y_percent, 0.5, 0.25)) {
        if (no_translation_state_counter){
            handle_spin_forward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
        } else {
            handle_spin_backward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
        }
    } else {
        handle_spin_forward(bot_state, left_y_percent, time_elapsed_this_rotation_us, us_per_rotation, half_rotation_time, motor_off_edge_time);
    }

    handle_spin_led(time_elapsed_this_rotation_us, us_per_rotation, led_on_us);

    // if we have completed a rotation, get ready for next rotation
    if (time_elapsed_this_rotation_us >= us_per_rotation) {
        time_elapsed_this_rotation_us = 0;
        no_translation_state_counter = 1 - no_translation_state_counter;
        bot_state->this_rotations_start_time_us = time_us_64();
    }

    // printf("SPINNING IS HAPPENING! \n");
}

void drive_update_bot_state(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent, double (*get_rpm)(double)) {
    if(drive_is_throttle_zero()) {
        handle_idle(bot_state, left_y_percent, right_y_percent, right_x_percent);
        return;
    }

    if (donut_get_curr_drive_mode() == DRIVE_MODE_MELTY) {
        led_time_blink(FAST_BLINK);
        handle_spin(bot_state, left_y_percent, right_y_percent, right_x_percent, get_rpm);
        return;
    } 
    
    if (donut_get_curr_drive_mode() == DRIVE_MODE_TANK) {
        led_repeat_blink(3);
        handle_tank(bot_state, left_y_percent, right_y_percent, right_x_percent);
        return;
    }
}
