#include "../src/donut_config.h"

int main() {
    led_init(HEADING_LIGHT_STRIP_PIN, HEADING_LIGHT_STRIP_PIN2);

    uint32_t start_time;

    int time_to_test_each_blink_type = 10000;

    while (1) {
        start_time = to_ms_since_boot(get_absolute_time());
        while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type){
            led_set_and_update_state(0);
            sleep_ms(100);
            led_set_and_update_state(1);
            sleep_ms(100);
        }

        led_set_and_update_state(0);
        sleep_ms(5000);

        start_time = to_ms_since_boot(get_absolute_time());
        while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type) {
            led_time_blink(SLOW_BLINK);
        }

        led_set_and_update_state(0);
        sleep_ms(5000);

        start_time = to_ms_since_boot(get_absolute_time());
        while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type) {
            led_time_blink(FAST_BLINK);
        }

        led_set_and_update_state(0);
        sleep_ms(5000);

        for (int repeats = 2; repeats < 6; repeats+=1) {
            start_time = to_ms_since_boot(get_absolute_time());
            while (to_ms_since_boot(get_absolute_time()) <= start_time + time_to_test_each_blink_type){
                led_repeat_blink(repeats);
            }
            sleep_ms(5000);
        }
    }
}
