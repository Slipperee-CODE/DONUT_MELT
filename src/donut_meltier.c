#include "donut_config.h"

static bot_state_t bot_state;

void donut_init_bot_state() {
    bot_state.is_failsafed = 1;
    bot_state.require_zero_throttle = 1;

    bot_state.this_rotations_start_time_us = -1;

    bot_state.max_rpm = 0;
    bot_state.rpm = 0;
}

#ifdef LIE_ABOUT_INPUT
    uint8_t donut_is_throttle_zero() { 
        return 1;
    }

    uint8_t donut_is_killswitch_active() {
        return 0;
    }

    uint8_t donut_get_curr_drive_mode() {
        return DRIVE_MODE_MELTY;
    }
#else 
    uint8_t donut_is_throttle_zero() { 
        return receiver_is_channel_near_value(LEFT_JOYSTICK_Y, RECEIVER_MIDDLEST_CHANNEL_VALUE, 300);
    }

    uint8_t donut_is_killswitch_active() {
        return receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 50);
    }

    uint8_t donut_get_curr_drive_mode() {
        if (receiver_is_channel_near_value(SWITCH_C, RECEIVER_HIGHEST_CHANNEL_VALUE, 50)) {
            return DRIVE_MODE_TANK;
        }
        return DRIVE_MODE_MELTY;
    }
#endif

// takes 0..1 to -1..1 (cubed for smoothing)
double input_remapping(double input) {
    input = input * 2; // 0..2
    input = input - 1; // -1..1
    return pow(input, 3); // still -1..1 but smoothed now
}

void when_flashing_motors() {
    led_repeat_blink(3);
}

void init_bot_systems() {
    stdio_init_all();

    donut_init_bot_state();

    #ifndef LIE_ABOUT_INPUT
        receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);
        accel_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    #endif
    
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    led_init(HEADING_LIGHT_STRIP_PIN, HEADING_LIGHT_STRIP_PIN2);

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
}

// a just connected or just powered on bot starts here, this is also where failsafed bots go
void when_failsafe_on() {
    motor_stop_all();

    bot_state.require_zero_throttle = 1;

    if (bot_state.is_failsafed == 0 && donut_is_throttle_zero()){
        bot_state.require_zero_throttle = 0;
    }

    led_time_blink(SLOW_BLINK);
}

#ifdef LIE_ABOUT_INPUT
    void when_failsafe_off() {
        drive_update_bot_state(
            &bot_state, 
            input_remapping(1),
            input_remapping(0),
            input_remapping(0),
            get_fake_rpm
        );
    }
#else
    void when_failsafe_off() {
        drive_update_bot_state(
            &bot_state, 
            input_remapping(receiver_get_percent_for_channel(LEFT_JOYSTICK_Y)), 
            input_remapping(receiver_get_percent_for_channel(RIGHT_JOYSTICK_Y)), 
            input_remapping(receiver_get_percent_for_channel(RIGHT_JOYSTICK_X)),
            get_rpm
        );
    }
#endif

void always() {
    #ifndef LIE_ABOUT_INPUT
        #ifdef OUTPUT_DIAGNOSTICS
            telemetry_output_diagnostics(&bot_state);
        #endif

        #ifdef SHOULD_SEND_TELEMETRY_TO_TRANSMITTER
            telemetry_send_telemetry(&bot_state);
        #endif

        receiver_update();
    #endif

    watchdog_update(); // keep watchdog happy
}

int main() {
    #ifdef FLASHING_MOTORS
        while(1){
            when_flashing_motors();
        }
    #endif

    init_bot_systems();

    while(1){
        if (bot_state.is_failsafed || bot_state.require_zero_throttle || donut_is_killswitch_active()){
            when_failsafe_on();
        } else {
            when_failsafe_off();
        }

        always();
    }

    return 0;
}
