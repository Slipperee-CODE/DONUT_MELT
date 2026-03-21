#include "../src/donut_config.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "H3LIS331DL.h"

int main() {
    stdio_init_all();

    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    sleep_ms(100); // give sensor time to boot I guess? - Cai

    while (1) {
        double* accel_data = accelerometer_get_all_axis();
        printf("X axis: %lf \n", accel_data[0]-ACCEL_ZERO_G_OFFSET);
        printf("Y axis: %lf \n", accel_data[1]-ACCEL_ZERO_G_OFFSET);
        printf("Z axis: %lf \n", accel_data[2]-ACCEL_ZERO_G_OFFSET);
    }
    
    return 0;
}