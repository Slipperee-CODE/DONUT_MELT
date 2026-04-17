#ifndef DONUT_CONFIG_H
#define DONUT_CONFIG_H

#include "donut_dependencies.h"

typedef struct {
    uint8_t is_failsafed;
    uint8_t require_zero_throttle;

    uint64_t this_rotations_start_time_us;

    uint32_t max_rpm;
    uint32_t rpm;
} bot_state_t;

void donut_init_bot_state();

uint8_t donut_is_throttle_zero();

uint8_t donut_is_killswitch_active();

//---------------LED SETTINGS---------------

#define HEADING_LIGHT_STRIP_PIN	6 // Onboard Pico2 led is pin 25
#define HEADING_LIGHT_STRIP_PIN2 7 


#define SLOW_BLINK 500
#define FAST_BLINK 50
#define REPEAT_BLINK 250

//---------------END LED SETTINGS---------------


//----------DONUT DRIVE SETTINGS---------

#define ACCEL_MOUNT_RADIUS_CM 0.5                                     
#define ACCEL_ZERO_G_OFFSET 2                                           
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.5

#define MOTOR_ON_PERCENT_DURATION 0.5 // This might technically be a half of a half - Cai
#define MIN_TRANSLATION_RPM 400

// #define LED_OFFSET_PERCENT 0.25 // I don't know what this number is for in OpenMelt - Cai
#define MIN_LED_PERCENT_DURATION 0.25 
#define MAX_LED_PERCENT_DURATION 0.75 

#define TANK_DRIVE_THROTTLE_MAX_REGISTERED_DEVIATION_FROM_CENTER 0.1

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
#define RECEIVER_MIDDLEST_CHANNEL_VALUE 1001 // (222 + 1780)/2 = 1001

//----------END REICEVER SETTINGS----------


//----------MOTOR SETTINGS----------

// ESC pins are connected as follows:
// MOTOR1_PIN was 2, MOTOR2_PIN was 3

// #define FLASHING_MOTORS 

#define MOTOR1_PIN 2 
#define MOTOR2_PIN 3

#define MOTOR1_PIO pio0
#define MOTOR2_PIO pio1

#define DSHOT_SPEED 600 // could be 150, 300, 450, 600 + some more

#define DRIVE_MODE_MELTY 1
#define DRIVE_MODE_TANK 2

//----------END MOTOR SETTINGS----------
                  

//----------TELEMETRY SETTINGS----------

#define SHOULD_SEND_TELEMETRY_TO_TRANSMITTER

// #define OUTPUT_DIAGNOSTICS

#define TIME_SINCE_BOOT_DIAGNOSTICS
#define FULL_CONTROLLER_DIAGNOSTICS
// #define OTHER_DIAGNOSTICS

#define TELEMETRY_MOTOR1 0
#define TELEMETRY_MOTOR2 1
#define TELEMETRY_MAIN 2

//----------END TELEMETRY SETTINGS----------


//----------MISC----------

#define WATCH_DOG_TIMEOUT_MS 500 // changed to prevent 200 microsecond delay from having any chance of triggering watchdog

//----------END MISC----------


//----------DEBUG SETTINGS----------

// #define LIE_ABOUT_RPM // lets us bypass using accelerometer readings if defined
// #define NO_MOTOR_SPINNING // prevents motors from spinning up if defined
// #define BYPASS_MIN_TRANSLATION_RPM // lets us bypass MIN_TRANSLATION_RPM if defined

//----------END DEBUG----------

#endif