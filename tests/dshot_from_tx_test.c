#include <stdio.h>
#include "stdint.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define RUNNING_A_TEST

#include "../src/donut_config.h"
#include "../src/motor_driver.h"
#include "../src/receiver.h"
#include "../src/led_driver.h"

static bot_state_t bot_state;

int main() {
    stdio_init_all();

    donut_init_bot_state();

    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);

    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    while (!receiver_is_channel_near_value(LEFT_JOYSTICK_Y, RECEIVER_LOWEST_CHANNEL_VALUE, 10) && !receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 10)){
        printf("WAITING FOR THROTTLE TO BE 0 AND SWITCH_E TO BE HIGH \n");
        receiver_update();

        led_time_blink(SLOW_BLINK);
    }

    while (receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 10)){
        motor_set_throttle_for_all((uint16_t) (2000*receiver_get_percent_for_channel(LEFT_JOYSTICK_Y)));
        receiver_update();

        led_time_blink(FAST_BLINK);
    }

    motor_set_throttle_for_all(0);

    printf("FINISHED MOVING THROUGH ALL THROTTLES");

    return 0;
}