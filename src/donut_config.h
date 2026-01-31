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

//---------------LED SETTINGS---------------

#define TIME_BETWEEN_SLOW_BLINK 500
#define TIME_BETWEEN_FAST_BLINK 50
#define TIME_BETWEEN_REPEATED_BLINKS 100
#define TIME_BETWEEN_REPEATED_BLINK_BURSTS 300

//---------------END LED SETTINGS---------------


//---------------BUTTONS TO CHANNELS---------------

#define RIGHT_JOYSTICK_X_CHANNEL 1
#define RIGHT_JOYSTICK_Y_CHANNEL 2
#define LEFT_JOYSTICK_Y_CHANNEL 3
#define LEFT_JOYSTICK_X_CHANNEL 4
#define SWITCH_E_CHANNEL 5
#define SWITCH_B_CHANNEL 6
#define SWITCH_C_CHANNEL 7
#define SWITCH_F_CHANNEL 8
#define KNOB_S1_CHANNEL 9
#define KNOB_S2_CHANNEL 10

//---------------END BUTTONS TO CHANNELS---------------


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
#define RECEIVER_TIMEOUT_MS 1000

//----------END REICEVER SETTINGS----------


//----------MOTOR SETTINGS----------

#define MOTOR1_PIN 2
#define MOTOR2_PIN 3

#define MOTOR1_PIO pio0
#define MOTOR2_PIO pio1

//----------END MOTOR SETTINGS----------
                  

//----------MISC----------

#define HEADING_LIGHT_STRIP_PIN	22

//#define OUTPUT_DIAGNOSTICS

#define WATCH_DOG_TIMEOUT_MS 100

#define RAW_TICK_NORMALIZER 2047

//----------END MISC----------

#endif