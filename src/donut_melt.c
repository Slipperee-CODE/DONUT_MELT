#include "donut_config.h"
#include "donut_drive.h"
#include "receiver.h"
#include "led_driver.h"
#include "motor_driver.h"
#include "donut_drive.h"
// #include "H3LIS331DL.h"

static bot_state_t bot_state;

void init_bot_state(){
    bot_state.crsf_link_quality = 0;
    bot_state.crsf_rssi = 0;
    bot_state.crsf_snr = 0;
    bot_state.crsf_tx_power = 0;
    bot_state.is_failsafed = 1;
    bot_state.require_zero_throttle = 1;
}

uint8_t is_throttle_zero(){
    return receiver_get_channel(LEFT_JOYSTICK_Y) <= RECEIVER_LOWEST_CHANNEL_VALUE;
}

uint8_t is_killswitch_active(){
    return receiver_get_channel(SWITCH_E) >= RECEIVER_HIGHEST_CHANNEL_VALUE;
}

uint8_t get_curr_drive_mode(){
    if (receiver_get_channel(SWITCH_C) >= RECEIVER_HIGHEST_CHANNEL_VALUE){
        return DRIVE_MODE_TANK;
    } else {
        return DRIVE_MODE_MELTY;
    }
}

double get_percent_for_channel(Channel channel){
    return fmax(0, fmin(1, (receiver_get_channel(channel) - 10.0) / RECEIVER_HIGHEST_CHANNEL_VALUE) - ((double) RECEIVER_LOWEST_CHANNEL_VALUE / RECEIVER_HIGHEST_CHANNEL_VALUE));
}

void update_bot_state(){
    printf("BOT STATE UPDATING \n");

    if (!is_throttle_zero()){
        drive_handle_tank(get_percent_for_channel(LEFT_JOYSTICK_Y), get_percent_for_channel(RIGHT_JOYSTICK_Y));
    } else {
        drive_handle_idle();
    }
}

int main(){
    #ifdef FLASHING_MOTORS
        while(1){

        }
    #endif

    stdio_init_all();
    init_bot_state();
    
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);
    led_init(HEADING_LIGHT_STRIP_PIN);
    // accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);

    // motor_init_all sends special dshot 0 command for 300 millis to get escs ready to work 
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics(bot_state);
        #endif

        #ifdef SHOULD_SEND_CUSTOM_TELEMETRY_TO_TRANSMITTER
            send_telemetry(bot_state);
        #endif

        if (bot_state.is_failsafed == 0 && bot_state.require_zero_throttle == 0 && !is_killswitch_active()){
            update_bot_state();
            led_time_blink(FAST_BLINK);
        } else { // a just connected or just powered on bot starts here, this is also where failsafed bots go
            motor_stop_all(); // calls sendThrottle(0) on both motors
            bot_state.require_zero_throttle = 1;

            led_time_blink(SLOW_BLINK);

            if (bot_state.is_failsafed == 0 && is_throttle_zero()){
                bot_state.require_zero_throttle = 0;
            }
        }

        // motor_update_bot_state();

        receiver_update();

        watchdog_update(); // keep watchdog happy
    }

    return 0;
}