#include <stdio.h>
#include "../src/donut_config.h"

static bot_state_t bot_state;

int main() {
    stdio_init_all();

    donut_init_bot_state();

    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);

    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    led_init(HEADING_LIGHT_STRIP_PIN, HEADING_LIGHT_STRIP_PIN2);

    sleep_ms(10000);

    while (!receiver_is_channel_near_value(LEFT_JOYSTICK_Y, RECEIVER_LOWEST_CHANNEL_VALUE, 20) || !receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 50)){
        printf("WAITING BEFORE ACTUALLY SPINNING \n");
        printf("receiver_is_channel_near_value left_joystick_y %d \n", receiver_get_channel(LEFT_JOYSTICK_Y));
        printf("receiver_is_channel_near_value switch_e %d \n", receiver_get_channel(SWITCH_E));

        receiver_update();

        led_time_blink(SLOW_BLINK);
    }

    while (receiver_is_channel_near_value(SWITCH_E, RECEIVER_HIGHEST_CHANNEL_VALUE, 50)){ 
        double t = (2000*receiver_get_percent_for_channel(LEFT_JOYSTICK_Y));
        printf("motor throttle being provided %lf \n", t);
        motor_set_throttle_for_all(t);
        receiver_update();

        led_time_blink(FAST_BLINK);
    }

    motor_set_throttle_for_all(0);
    led_set_and_update_state(0);

    printf("FINISHED MOVING THROUGH ALL THROTTLES");

    return 0;
}