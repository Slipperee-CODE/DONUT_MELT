#include "led_driver.h"

static uint8_t LED_PIN;
static uint32_t prev_time;
static uint8_t led_state;

void led_init(uint8_t pin){
    LED_PIN = pin;
    prev_time = 0;
    led_state = 0;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_zero_prev_time(){
    prev_time = 0;
}

void led_set_state(uint8_t state){
    led_state = state;
}

void led_time_blink(uint32_t millis){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= millis){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;

        prev_time = curr_time;
    }
}

// something's wrong with this logic, try to simplify it
void led_repeat_blink(uint8_t repeats){
    static uint8_t led_state_changes = 0;

    if (led_state_changes > repeats * 2 + 1){
        led_state_changes = 0;
    }

    if (led_state_changes == 0){
        led_set_state(1);
    }

    uint16_t time_to_wait_before_next_blink = REPEAT_BLINK;
    if (led_state_changes == repeats * 2){
        time_to_wait_before_next_blink = SLOW_BLINK;
    }

    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= time_to_wait_before_next_blink){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;
        led_state_changes += 1;

        prev_time = curr_time;
    }
}