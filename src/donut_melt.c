#include "donut_config.h"
#include "receiver.h"
// #include "H3LIS331DL.h"
#include "led_driver.h"

static bot_state_t state;

void init_bot_state(){
    state.crsf_link_quality = 0;
    state.crsf_rssi= 0;
    state.crsf_snr = 0;
    state.crsf_tx_power = 0;
    state.is_failsafed = 1;
}

void output_diagnostics(){
    #ifndef OUTPUT_VERBOSE_DIAGNOSTICS
        // print main diagnostics here
    #else 
        printf("\n---------DIAGNOSTICS START-------------\n\n");

        printf("DIAGNOSTICS AT %f SECS SINCE BOOT \n\n", to_ms_since_boot(get_absolute_time()) / 1000.0);

        printf("--------FULL CONTROLLER INFO---------\n");
        printf("RIGHT_JOYSTICK_X: %d | ", receiver_get_channel(RIGHT_JOYSTICK_X));
        printf("RIGHT_JOYSTICK_Y: %d \n", receiver_get_channel(RIGHT_JOYSTICK_Y));
        printf("LEFT_JOYSTICK_Y: %d | ", receiver_get_channel(LEFT_JOYSTICK_Y));
        printf("LEFT_JOYSTICK_X: %d \n", receiver_get_channel(LEFT_JOYSTICK_X));
        printf("SWITCH_E: %d | ", receiver_get_channel(SWITCH_E));
        printf("SWITCH_B: %d | ", receiver_get_channel(SWITCH_B));
        printf("SWITCH_C: %d | ", receiver_get_channel(SWITCH_C));
        printf("SWITCH_F: %d \n", receiver_get_channel(SWITCH_F));
        printf("KNOB_S1: %d | ", receiver_get_channel(KNOB_S1));
        printf("KNOB_S2: %d \n\n", receiver_get_channel(KNOB_S2));

        printf("--------CRSF INFO---------\n");
        printf("crsf_link_quality: %d \n", state.crsf_link_quality);
        printf("crsf_rssi: %d \n", state.crsf_rssi);
        printf("crsf_snr: %d \n", state.crsf_snr);
        printf("crsf_tx_power: %d \n\n", state.crsf_tx_power);

        printf("--------OTHER INFO---------\n");
        printf("is_failsafed: %d \n", state.is_failsafed);

        printf("\n---------DIAGNOSTICS END-------------\n\n");
    #endif
}

void update_bot_state(){
    printf("BOT STATE UPDATING \n");

    // TO DO:
    // + GET CRSF LIBRARY ACTUALLY WORKING 
    // + test setting motor power values WITH DSHOT (MAKE SURE IT DOES EXACTLY WHAT INTENDED)
    // + add extended telemetry data to verbose diagnostics and then actually try to read it  
    
    // + flash both motors with same firmware
}

int main(){
    stdio_init_all();
    init_bot_state();

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &state);
    // accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    // motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO);
    led_init(HEADING_LIGHT_STRIP_PIN);

    // wait_for_zero_throttle_and_receiver_connection();

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        if (state.is_failsafed == 0){
            update_bot_state();
            led_time_blink(FAST_BLINK);
        } else {
            // stop all motors
            led_time_blink(SLOW_BLINK);
        }

        receiver_update();

        watchdog_update(); // keep watchdog happy
    }

    return 0;
}