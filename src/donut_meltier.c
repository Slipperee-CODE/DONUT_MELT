#include "donut_config.h"
#include "donut_drive.h"
#include "donut_telemetry.h"
#include "H3LIS331DL.h"
#include "led_driver.h"
#include "motor_driver.h"
#include "receiver.h"

static bot_state_t bot_state;

void donut_init_bot_state(){
    bot_state.is_failsafed = 1;
    bot_state.require_zero_throttle = 1;

    bot_state.this_rotations_start_time_us = -1;

    bot_state.max_rpm = 0;
    bot_state.rpm = 0;
}

// need to make this function and is_killswitch_active accesible from donut_drive.c somehow
uint8_t donut_is_throttle_zero(){ 
    return receiver_is_channel_near_value(LEFT_JOYSTICK_Y, (uint16_t) ((double) (RECEIVER_HIGHEST_CHANNEL_VALUE + RECEIVER_LOWEST_CHANNEL_VALUE) / 2), 10);
}

uint8_t donut_is_killswitch_active(){
    return receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 10);
}

void when_flashing_motors(){
    led_repeat_blink(3);
}

void init_bot_systems(){
    stdio_init_all();

    donut_init_bot_state();

    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);

    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    led_init(HEADING_LIGHT_STRIP_PIN);

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
}

// a just connected or just powered on bot starts here, this is also where failsafed bots go
void when_failsafe_on(){
    motor_stop_all();

    bot_state.require_zero_throttle = 1;

    if (bot_state.is_failsafed == 0 && donut_is_throttle_zero()){
        bot_state.require_zero_throttle = 0;
    }

    led_time_blink(SLOW_BLINK);
}

void when_failsafe_off(){
    drive_update_bot_state(
        &bot_state, 
        receiver_get_percent_for_channel(LEFT_JOYSTICK_Y), 
        receiver_get_percent_for_channel(RIGHT_JOYSTICK_Y), 
        receiver_get_percent_for_channel(RIGHT_JOYSTICK_X)
    );

    // led_time_blink(FAST_BLINK);
}

void always(){
    #ifdef OUTPUT_DIAGNOSTICS
        telemetry_output_diagnostics(&bot_state);
    #endif

    #ifdef SHOULD_SEND_TELEMETRY_TO_TRANSMITTER
        telemetry_send_telemetry(&bot_state);
    #endif

    // motor_update_bot_state();

    receiver_update();

    watchdog_update(); // keep watchdog happy
}

#ifndef RUNNING_A_TEST
    int main(){
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
#endif