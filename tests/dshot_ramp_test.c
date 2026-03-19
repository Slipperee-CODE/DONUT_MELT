#include <stdio.h>
#include "stdint.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "../src/donut_config.h"
#include "../src/motor_driver.h"

static bot_state_t bot_state;

void init_bot_state(){
    bot_state.crsf_link_quality = 0;
    bot_state.crsf_rssi = 0;
    bot_state.crsf_snr = 0;
    bot_state.crsf_tx_power = 0;

    bot_state.is_failsafed = 1;
    bot_state.require_zero_throttle = 1;

    bot_state.rpm = 0;
    bot_state.rotation_time_elapsed = 0;
}

int main() {
    stdio_init_all();

    int start_wait_time = 5000;
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + start_wait_time){
        printf("WAITING BEFORE START \n");
    }

    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    sleep_ms(3000);

    // low is 1, high is 999
    for (int i = 1; i <= 500; i+=10){
        motor_set_throttle_for_all(i);
        sleep_ms(100);
    }

    motor_set_throttle_for_all(1001);
    sleep_ms(20000);

    // low is 1001, high is 2000
    for (int i = 1001; i <= 1500; i+=10){
        motor_set_throttle_for_all(i);
        sleep_ms(100);
    }

    motor_set_throttle_for_all(0);

    printf("FINISHED MOVING THROUGH ALL THROTTLES");

    return 0;
}