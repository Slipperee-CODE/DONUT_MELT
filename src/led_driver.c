#include "led_driver.h"

uint8_t LED_PIN;

void led_init(uint8_t pin){
    LED_PIN = pin;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_time_blink(int millis){
    static uint32_t prev_time = 0;
    static uint8_t led_state = 0;

    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= millis){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;

        prev_time = curr_time;
    }
}