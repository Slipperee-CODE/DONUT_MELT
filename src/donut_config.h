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
    uint32_t crsf_link_quality;
    uint32_t crsf_rssi;
    uint32_t crsf_snr;
    uint32_t crsf_tx_power;

    uint32_t bidir_m1_erpm;
    uint32_t bidir_m1_voltage;
    uint32_t bidir_m1_current;
    uint32_t bidir_m1_temperature;
    uint32_t bidir_m1_status;
    uint32_t bidir_m1_stress;

    uint32_t bidir_m2_erpm;
    uint32_t bidir_m2_voltage;
    uint32_t bidir_m2_current;
    uint32_t bidir_m2_temperature;
    uint32_t bidir_m2_status;
    uint32_t bidir_m2_stress;

    uint8_t is_failsafed;
    uint8_t require_zero_throttle;
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

#define RECEIVER_LOWEST_CHANNEL_VALUE 222 // actual lowest is 172 for knobs and 212 for joysticks
#define RECEIVER_HIGHEST_CHANNEL_VALUE 1780 // actual highest is 1790

//----------END REICEVER SETTINGS----------


//----------MOTOR SETTINGS----------

#define MOTOR1_PIN 2
#define MOTOR2_PIN 3

#define MOTOR1_PIO pio0
#define MOTOR2_PIO pio1

#define DSHOT_SPEED 600 // could be 150, 300, 450, 600 + some more

#define DRIVE_MODE_MELTY 1
#define DRIVE_MODE_TANK 2

//----------END MOTOR SETTINGS----------
                  

//----------TELEMETRY SETTINGS----------

#define SHOULD_SEND_CUSTOM_TELEMETRY_TO_TRANSMITTER

#define OUTPUT_DIAGNOSTICS

#define TIME_SINCE_BOOT_DIAGNOSTICS
#define FULL_CONTROLLER_DIAGNOSTICS
// #define CRSF_DIAGNOSTICS
// #define MOTOR1_DIAGNOSTICS
// #define MOTOR2_DIAGNOSTICS
#define OTHER_DIAGNOSTICS

#define TELEMETRY_MOTOR1 0
#define TELEMETRY_MOTOR2 1
#define TELEMETRY_MAIN 2

//----------END TELEMETRY SETTINGS----------


//----------MISC----------

#define HEADING_LIGHT_STRIP_PIN	22

#define WATCH_DOG_TIMEOUT_MS 100

//----------END MISC----------

#endif