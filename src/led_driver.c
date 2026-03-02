#include "led_driver.h"

uint8_t LED_PIN;
uint8_t led_state;

void led_init(uint8_t pin){
    LED_PIN = pin;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    led_state = 0;
}

void led_time_blink(int millis){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time % millis == 0){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;
    }
}