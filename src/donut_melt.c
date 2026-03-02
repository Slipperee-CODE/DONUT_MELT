#include "donut_config.h"
#include "receiver.h"
// #include "H3LIS331DL.h"

int main(){
    stdio_init_all();

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN);
    // accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    // motor_init_all(MOTOR1_PIN, MOTOR2_PIN);
    // led_init(HEADING_LIGHT_STRIP_PIN);

    // wait_for_zero_throttle_and_receiver_connection();

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            // output_diagnostics();
        #endif

        receiver_update();

        watchdog_update(); // keep watchdog happy
    }

    return 0;
}