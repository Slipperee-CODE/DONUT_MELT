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

    int throttle = 1000;  
    while (throttle >= 0){
        BidirDShotX1_sendThrottle(esc, throttle);

        sleep_us(20);

        uint32_t rpm = 0;
        BidirDShotX1_getTelemetryErpm(esc, &rpm);
        rpm /= MOTOR_POLES / 2; // eRPM = RPM * poles/2 <=> RPM = eRPM / (poles/2)
        printf("rpm: %d \n", rpm);

        sleep_us(180); // keep packets spaced out

        sleep_us(10000); // waiting 10 milliseconds before increasing throttle for next loop
        throttle-=10;
    }

    send_throttle_for_millis(0, 3000);

    throttle = 1002;  
    while (throttle <= 2000){
        BidirDShotX1_sendThrottle(esc, throttle);

        sleep_us(20);

        uint32_t rpm = 0;
        BidirDShotX1_getTelemetryErpm(esc, &rpm);
        rpm /= MOTOR_POLES / 2; // eRPM = RPM * poles/2 <=> RPM = eRPM / (poles/2)
        printf("rpm: %d \n", rpm);

        sleep_us(180); // keep packets spaced out
    


        sleep_us(10000); // waiting 10 milliseconds before increasing throttle for next loop
        throttle+=10;
    }

    send_throttle_for_millis(0, 3000);
    
    printf("FINISHED MOVING THROUGH ALL THROTTLES");

    return 0;
}