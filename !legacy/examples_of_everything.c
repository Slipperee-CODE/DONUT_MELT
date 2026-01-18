#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}



// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5



int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    // Watchdog example code
    if (watchdog_caused_reboot()) {
        printf("Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }
    
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(100, 1);
    
    // You need to call this function at least more often than the 100ms in the enable call to prevent a reboot
    watchdog_update();

    printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));
    // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    // i2c_getc(I2C_PORT);
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    //uint64_t start_time = time_us_64(); gets time (in microseconds) since boot as an unsigned 64-bit int 

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }


    //uint8_t inc_byte = uart_getc(uartid); reads the current incoming byte from uart channel with uartid 


    // When implementing donut melt you will need to interface with the elrs receiver 
    // using the uart-based "crossfire serial protocol" which has a particular frame structure (arrangement of the data it sends)

    // You will also need to communicate with escs using DShot 
    // (may need to configure escs for bidirectionality so they actually work both directions)

    //TO DO:
    // Find libraries for both DShot and CRSF communication for pico 2 in c 
    // DShot: https://github.com/cadouthat/pico-dshot/tree/main or https://github.com/josephduchesne/pico-dshot-bidir
    // CRSF: https://github.com/britannio/pico_crsf/blob/main/example/main.c
    // - Values seem very close to dshot (0-2047) + there are on_failsafe() (should call the same func as watchdog) and on_rc_channels() change methods for doing stuff
    // Figure out how to integrate cpp accelerometer library from OPEN_MELT into c
    //  Might need to directly communicate with accelerometer using i2c, might not 
    // Write out "melty logic" but maybe do it differently from OPEN_MELT
    //  Use motor timing offset, to move in a diff direction ("heading") (Reverse direction by 180 every rotation when idle)
    //  Use fixed dshot-controlled motor movement + implement two wheel drive
    //  Make sure you are using watchdog + checking for transmitter disconnection 
}   
