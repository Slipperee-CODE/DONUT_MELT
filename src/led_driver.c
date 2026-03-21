#include "led_driver.h"

static uint8_t LED_PIN;
static uint32_t prev_time;
static uint8_t led_state;

static uint8_t LED_PIN2;

void led_init(uint8_t pin, uint8_t pin2){
    LED_PIN = pin;
    LED_PIN2 = pin2;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(LED_PIN2);
    gpio_set_dir(LED_PIN2, GPIO_OUT);

    prev_time = 0;
    led_set_and_update_state(0);
}

void set_state(uint8_t state){
    led_state = state;
}

void led_set_and_update_state(uint8_t state){
    set_state(state);
    gpio_put(LED_PIN, led_state);
    gpio_put(LED_PIN2, led_state);
}

void led_time_blink(uint32_t millis){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= millis){
        gpio_put(LED_PIN, led_state);
        gpio_put(LED_PIN2, led_state);
        led_state = 1 - led_state;

        prev_time = curr_time;
    }
}

void led_repeat_blink(uint8_t repeats){
    static uint8_t led_state_changes = 0;

    if (led_state_changes == 0) {
        set_state(0);
    }

    uint16_t time_to_wait_before_next_state_change = REPEAT_BLINK;
    if (led_state_changes + 1 >= repeats * 2){
        time_to_wait_before_next_state_change = 2*REPEAT_BLINK*repeats;
        set_state(1);
        led_state_changes = -1;
    }

    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= time_to_wait_before_next_state_change){
        gpio_put(LED_PIN, led_state);
        gpio_put(LED_PIN2, led_state);
        led_state = 1 - led_state;
        led_state_changes += 1;

        prev_time = curr_time;
    }
}