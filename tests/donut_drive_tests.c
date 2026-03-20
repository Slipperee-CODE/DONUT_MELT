#include <stdio.h>
#include "stdint.h"
#include "pico/stdlib.h"
#include "pico/time.h"

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

#define MELTY_DRIVE_MELTY_LED_ONLY
// #define MELTY_DRIVE_ONLY
// #define TANK_DRIVE_ONLY

void test_melty_led_only(){
    while (1){
        drive_update_bot_state(&bot_state, 1, 1, 0);
    }

    // continue testing here
}

void test_melty_drive_only(){
    repeat_update_bot_state_call_for_ms(&bot_state, 1, 1, 1, 1000);

    // continue testing here
}

void test_tank_drive_only(){
    repeat_update_bot_state_call_for_ms(&bot_state, 1, 1, 1, 1000);

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