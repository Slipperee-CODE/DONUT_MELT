#include <stdio.h>
#include "pico/stdlib.h"
#include "stdint.h"
#include "hardware/pio.h"
#include "c_pico_dshot.h"
#include "pico/time.h"

#define MOTOR_POLES 14

BidirDShotX1* esc;

void send_throttle_for_millis(int throttle, int millis){
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + millis){
        BidirDShotX1_sendThrottle(esc, throttle);
        //sleep_us(200); // keep packets spaced out
    }
}

int main() {
    stdio_init_all();
    int start_wait_time = 10000;
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + start_wait_time){
        printf("WAITING BEFORE START 2 \n");
    }

    esc = BidirDShotX1_constructor(2, 600, pio0, -1);
    send_throttle_for_millis(0, 3000);

    send_throttle_for_millis(500, 5000);
    
    printf("FINISHED MOVING THROUGH ALL THROTTLES");

    return 0;
}