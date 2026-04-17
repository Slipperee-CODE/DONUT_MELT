#include <stdio.h>
#include "../src/donut_config.h"

static bot_state_t bot_state;

int main() {
    stdio_init_all();
    donut_init_bot_state();

    sleep_ms(5000);

    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);

    while (1) {
        receiver_update();

        telemetry_output_diagnostics(&bot_state);
        telemetry_send_telemetry(&bot_state);
    }
    
    return 0;
}