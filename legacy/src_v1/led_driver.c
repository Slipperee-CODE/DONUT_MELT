#include "led_driver.h"

uint8_t LED_PIN;
uint32_t prev_saved_time;

void led_init(uint8_t pin){
    LED_PIN = pin;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    prev_saved_time = to_ms_since_boot(get_absolute_time());
}

void led_time_blink(int millis){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_saved_time <= millis){
        gpio_put(LED_PIN, 1);
        return;
    }

    if (curr_time - prev_saved_time <= 2*millis){
        gpio_put(LED_PIN, 0);
        return;
    }

    prev_saved_time = to_ms_since_boot(get_absolute_time());
}