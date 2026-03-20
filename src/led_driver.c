#include "led_driver.h"

static uint8_t LED_PIN;
static uint32_t prev_time;
static uint8_t led_state;

void led_init(uint8_t pin){
    LED_PIN = pin;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    led_reset();
}

void led_set_state(uint8_t state){
    led_state = state;
}

void led_reset(){
    prev_time = 0;
    led_set_state(0);
    gpio_put(LED_PIN, led_state);
}

// this isn't gonna work, need to try a different approach so LED doesn't get thrown off between changes to parameters
// it should be an invariant that led_reset() is called before this function
// make sure this function properly times itself across function calls, calling led_reset() before this function should help
void led_time_blink_on_off(uint32_t millis_on, uint32_t millis_off){
    static uint8_t curr_wait_time_type = 0;

    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    uint16_t time_to_wait_before_next_state_change = millis_on;
    if (curr_wait_time_type){
        time_to_wait_before_next_state_change = millis_off;
    }

    if (curr_time - prev_time >= time_to_wait_before_next_state_change){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;
        curr_wait_time_type = 1 - curr_wait_time_type;

        prev_time = curr_time;
    }
}

void led_time_blink(uint32_t millis){
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= millis){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;

        prev_time = curr_time;
    }
}

void led_repeat_blink(uint8_t repeats){
    static uint8_t led_state_changes = 0;

    if (led_state_changes == 0) {
        led_set_state(0);
    }

    uint16_t time_to_wait_before_next_state_change = REPEAT_BLINK;
    if (led_state_changes + 1 >= repeats * 2){
        time_to_wait_before_next_state_change = 2*REPEAT_BLINK*repeats;
        led_set_state(1);
        led_state_changes = -1;
    }

    uint32_t curr_time = to_ms_since_boot(get_absolute_time());

    if (curr_time - prev_time >= time_to_wait_before_next_state_change){
        gpio_put(LED_PIN, led_state);
        led_state = 1 - led_state;
        led_state_changes += 1;

        prev_time = curr_time;
    }
}