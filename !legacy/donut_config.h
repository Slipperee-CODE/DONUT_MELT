#include "hardware/i2c.h"
#include "hardware/uart.h"

#ifndef DONUT_CONFIG_GUARD
#define DONUT_CONFIG_GUARD

//ALL OF THESE VALUES NEED TO BE ADJUSTED TO MATCH REALITY

//----------TRANSLATIONAL DRIFT SETTINGS---------
#define ACCEL_MOUNT_RADIUS_CM 4.45                                     
#define ACCEL_ZERO_G_OFFSET 1.5f      
                                                           
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.5f

#define MIN_TRANSLATION_RPM 400  


//----------PIN MAPPINGS AND MORE----------
#define ACCEL_I2C_PORT i2c0
#define ACCEL_I2C_SDA 8
#define ACCEL_I2C_SCL 9

#define RECEIVER_UART_ID uart1
// RECEIVER_BAUD_RATE is 420000 and set in the crsf.c file
#define RECEIVER_UART_TX_PIN 6
#define RECEIVER_UART_RX_PIN 7
#define CRSF_LINK_QUALITY_THRESHOLD 70
#define CRSF_RSSI_THRESHOLD 105

#define HEADING_LIGHT_STRIP_PIN	2                     

#define MOTOR1_PIN 18
#define MOTOR2_PIN 19                         

#define BATTERY_ADC_PIN A0 


//----------BATTERY MONITOR----------
#define BATTERY_ALERT_ENABLED          
#define VOLTAGE_DIVIDER 11                       
#define BATTERY_ADC_WARN_VOLTAGE_THRESHOLD 7.0f  
#define PICO_VOLTAGE 5.0f                     
#define LOW_BAT_REPEAT_READS_BEFORE_ALARM 20


//----------SAFETY----------
#define ENABLE_WATCHDOG                           
#define WATCH_DOG_TIMEOUT_MS 2000                
#define VERIFY_RC_THROTTLE_ZERO_AT_BOOT 


#endif 