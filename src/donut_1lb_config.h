#ifndef DONUT_1LB_CONFIG
#define DONUT_1LB_CONFIG

//---------------LED SETTINGS---------------

// Onboard Pico2 led is on pin 25
// For 1lb:
//  Our usual top led is on pin 6 
//  Our usual bottom led is on pin 7
#define HEADING_LIGHT_STRIP_PIN	25
#define HEADING_LIGHT_STRIP_PIN2 7

//---------------END LED SETTINGS---------------


//----------DONUT DRIVE SETTINGS---------

#define USE_1LB_ACCEL_SETUP
// was 0.5 for working translation test with 1lb on 4/29/26 - Cai
#define THROTTLE_PC_P 1 

#define CAN_ADJUST_ACCEL_MOUNT_RADIUS
#define ACCEL_OFFSET_SENSITIVITY 0.0001

#define ACCEL_MOUNT_RADIUS_CM 2.4 + 0.3559 // 0.3559 is manually measured offset - Cai

#define ACCEL_ZERO_G_OFFSET 2
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 0.1

#define MOTOR_ON_PERCENT_DURATION 0.5 // This might technically be a half of a half - Cai (I still have no idea what this comment means - also Cai)
#define MIN_TRANSLATION_RPM 400

#define LED_OFFSET_PERCENT 0.25
#define MIN_LED_PERCENT_DURATION 0.25
#define MAX_LED_PERCENT_DURATION 0.5

// MELTY_MAX_TRANSLATION_AGGRESSION is a number from 0..1 which represents how different 
// the throttle sent to each motor can be during a half-rotation
// 1 = very different
// 0 = can't differ at all
#define MELTY_MAX_TRANSLATION_AGGRESSION 0.75  
#define MELTY_MAX_THROTTLE 1

#define TANK_DRIVE_MAX_THROTTLE 0.125
#define TANK_DRIVE_TURNING_MAX_THROTTLE 0.0625

//----------END DONUT DRIVE SETTINGS---------


#endif