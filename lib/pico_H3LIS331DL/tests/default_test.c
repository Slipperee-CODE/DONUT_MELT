#include <stdio.h>
#include "H3LIS331DL.h"

#define ACCELEROMETER_I2C_PORT i2c0
#define ACCELEROMETER_I2C_SDA 1
#define ACCELEROMETER_I2C_SCL 2

int main()
{
    stdio_init_all();

    accelerometer_init(ACCELEROMETER_I2C_PORT, ACCELEROMETER_I2C_SDA, ACCELEROMETER_I2C_SCL);
    
    while (true) {
        printf("This loop prints the accelerometer's x-axis output in gs! \n");
        pico_H3LIS331DL_is_library_accesible();
        printf("Acceleration along x-axis is %d gs\n", accelerometer_get_x());
    }
}