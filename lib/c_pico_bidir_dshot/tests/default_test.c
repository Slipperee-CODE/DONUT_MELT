#include <stdio.h>
#include "pico/stdlib.h"
#include "stdint.h"
#include "hardware/pio.h"
#include "c_pico_dshot.h"
#include "pico/time.h"

#define MOTOR_POLES 14

BidirDShotX1* esc;

int main() {
    stdio_init_all();

    esc = BidirDShotX1_constructor(2, 600, pio0, -1);

    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + 3000){
        BidirDShotX1_sendThrottle(esc, 0);
    }

    while (1){
        sleep_us(200); // keep packets spaced out
        uint32_t rpm = 0;
        BidirDShotX1_getTelemetryErpm(esc, &rpm);
        rpm /= MOTOR_POLES / 2; // eRPM = RPM * poles/2 <=> RPM = eRPM / (poles/2)
        printf("rpm: %d \n", rpm);
        BidirDShotX1_sendThrottle(esc, 0); // make this instead sweep through full/some portion of throttle range semi-slowly
    }

    return 0;
}