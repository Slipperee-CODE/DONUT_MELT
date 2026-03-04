#ifndef DONUT_CONFIG_H
#define DONUT_CONFIG_H

// Used in this header file and donut_melt.c
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"

// Used in donut_melt.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include <math.h>

typedef struct {
    int crsf_link_quality;
    int crsf_rssi;
    int crsf_snr;
    int crsf_tx_power;
    uint8_t is_failsafed;
} bot_state_t;

//---------------LED SETTINGS---------------

#define SLOW_BLINK 500
#define FAST_BLINK 50

//---------------END LED SETTINGS---------------


//----------DONUT DRIVE SETTINGS---------

#define ACCEL_MOUNT_RADIUS_CM 4.45                                     
#define ACCEL_ZERO_G_OFFSET 1.5                                           
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.5
#define MIN_TRANSLATION_RPM 400

//----------END DONUT DRIVE SETTINGS---------


//----------ACCELEROMETER SETTINGS----------

#define ACCEL_I2C_PORT i2c0
#define ACCEL_I2C_SDA 4
#define ACCEL_I2C_SCL 5

//----------END ACCELEROMETER SETTINGS----------


//----------REICEVER SETTINGS----------

#define RECEIVER_UART_ID uart1
#define RECEIVER_UART_TX_PIN 8
#define RECEIVER_UART_RX_PIN 9

//----------END REICEVER SETTINGS----------


//----------MOTOR SETTINGS----------

#define MOTOR1_PIN 2
#define MOTOR2_PIN 3

#define MOTOR1_PIO pio0
#define MOTOR2_PIO pio1

#define DSHOT_SPEED 150; // could be 150, 300, 450, 600 + some more

//----------END MOTOR SETTINGS----------
                  

//----------TELEMETRY SETTINGS----------

#define OUTPUT_DIAGNOSTICS

#define OUTPUT_VERBOSE_DIAGNOSTICS

//----------END TELEMETRY SETTINGS----------


//----------MISC----------

#define HEADING_LIGHT_STRIP_PIN	25

#define WATCH_DOG_TIMEOUT_MS 100

#define RAW_TICK_NORMALIZER 2047

//----------END MISC----------

#endif