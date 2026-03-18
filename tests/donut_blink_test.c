#include "pico/stdlib.h"
#include "../src/led_driver.h"
#include "../src/donut_config.h"

int main() {
    led_init(HEADING_LIGHT_STRIP_PIN);

    uint32_t start_time;

    int time_to_test_each_blink_type = 5000;
    start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type){
        led_time_blink(SLOW_BLINK);
    }

    start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type){
        led_time_blink(FAST_BLINK);
    }

    for (int repeats = 2; repeats < 10; repeats+=1){
        start_time = to_ms_since_boot(get_absolute_time());
        while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type){
            led_repeat_blink(repeats);
        }
    }
}