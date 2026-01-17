#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "donut_config.h"

int main()
{
    stdio_init_all();

    //receiver init -> constantly check to make sure we are receiving data
    //dshot init -> set motor power to 0
    //accelerometer init
    //watchdog start/setup
}