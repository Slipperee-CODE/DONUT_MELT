#include <stdio.h>
#include "stdint.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define LIE_ABOUT_UPR // lets us bypass using accelerometer readings if defined
#define NO_MOTOR_SPINNING // prevents motors from spinning up if defined
#define BYPASS_MIN_TRANSLATION_RPM // lets us bypass MIN_TRANSLATION_RPM if defined

#define MELTY_DRIVE_MELTY_LED_ONLY
// #define MELTY_DRIVE_ONLY
// #define TANK_DRIVE_ONLY

#define RUNNING_A_TEST

#include "../src/donut_config.h"
#include "../src/donut_drive.h"
#include "../src/led_driver.h"

static bot_state_t bot_state;

void repeat_update_bot_state_call_for_ms(bot_state_t* bot_state_ptr, double left_y_percent, double right_y_percent, double right_x_percent, uint32_t ms){
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + ms){
        drive_update_bot_state(bot_state_ptr, left_y_percent, right_y_percent, right_x_percent);
    }
}

void repeat_led_repeat_blink_call_for_ms(uint32_t ms){
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + ms){
        led_repeat_blink(2);
    }
}

void test_melty_led_only(){
    // led should just stay on if BYPASS_MIN_TRANSLATION_RPM is not defined for rpms under MIN_TRANSLATION_RPM from donut_config.h
    // definitely should define BYPASS_MIN_TRANSLATION_RPM at some point to test LED pulsing for very slow spins 

    // checking to see if 0 rpm breaks anything (it shouldn't)
    drive_set_fake_curr_upr_from_rpm(0);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.5, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // changing throttle should change the amount of time the LED is on for
    drive_set_fake_curr_upr_from_rpm(20);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // this LED should be on just as long as the previous one because lower bound for LED on time is MIN_LED_PERCENT_DURATION from donut_config.h 
    drive_set_fake_curr_upr_from_rpm(20);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.25, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // this led should be on for a long time (not full time though because upper bound for LED on time is MAX_LED_PERCENT_DURATION from donut_config.h )
    drive_set_fake_curr_upr_from_rpm(20);
    repeat_update_bot_state_call_for_ms(&bot_state, 1, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // this LED should be on just as long as the previous one because upper bound for LED on time is MAX_LED_PERCENT_DURATION
    drive_set_fake_curr_upr_from_rpm(20);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.75, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // pretending to spin faster so LED should toggle states more frequently
    drive_set_fake_curr_upr_from_rpm(40);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // pretending to spin really fast so LED should toggle states way more frequently
    drive_set_fake_curr_upr_from_rpm(500);
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // continue testing here
}

void test_melty_drive_only(){
    // low rpm so we can actually tell what's happening
    drive_set_fake_curr_upr_from_rpm(20);

    // low throttle, spin and translate backwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0.1, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);
    
    // low throttle, spin and translate forwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0.85, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // higher throttle, spin and translate backwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.6, 0.1, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);
    
    // same higher throttle, spin and translate forwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.6, 0.85, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // continue testing here
}

void test_tank_drive_only(){
    // rpm shouldn't matter for this test but doing this just in case fake_curr_upr needs to be initialized to a value
    drive_set_fake_curr_upr_from_rpm(0);

    // both motors backwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0.1, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);
    
    // both one motor backwards, one motor forwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.1, 0.6, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // both one motor forwards, one motor backwards (should be swapped from above)
    repeat_update_bot_state_call_for_ms(&bot_state, 0.6, 0.1, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);
    
    // both motors forwards
    repeat_update_bot_state_call_for_ms(&bot_state, 0.7, 0.7, 0, 10000);
    repeat_led_repeat_blink_call_for_ms(5000);

    // continue testing here
}

int main() {
    stdio_init_all();
    donut_init_bot_state();

    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);
    led_init(HEADING_LIGHT_STRIP_PIN);

    #ifdef MELTY_DRIVE_MELTY_LED_ONLY
        test_melty_led_only();
    #endif

    #ifdef MELTY_DRIVE_ONLY
        test_melty_drive_only();
    #endif

    #ifdef TANK_DRIVE_ONLY
        test_tank_drive_only();
    #endif

    return 0;
}