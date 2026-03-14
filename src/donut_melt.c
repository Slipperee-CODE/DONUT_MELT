#include "donut_config.h"
#include "donut_drive.h"
#include "receiver.h"
#include "led_driver.h"
#include "motor_driver.h"
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

double get_left_throttle_percent(){
    return fmax(0, fmin(1, (double) (receiver_get_channel(LEFT_JOYSTICK_Y) - 10) / RECEIVER_HIGHEST_CHANNEL_VALUE) - 0.12);
}

double get_right_throttle_percent(){
    return fmax(0, fmin(1, (double) (receiver_get_channel(RIGHT_JOYSTICK_Y) - 10) / RECEIVER_HIGHEST_CHANNEL_VALUE) - 0.12);
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

uint8_t get_telemetry_state(){
    if (receiver_get_channel(SWITCH_F) >= RECEIVER_HIGHEST_CHANNEL_VALUE){
        return TELEMETRY_MOTOR1;
    } else if (receiver_get_channel(SWITCH_F) <= RECEIVER_LOWEST_CHANNEL_VALUE){
        return TELEMETRY_MAIN;
    } else {
        return TELEMETRY_MOTOR2;
    }
}

uint32_t get_seconds_since_boot(){
    return to_ms_since_boot(get_absolute_time()) / 1000.0;
}

void output_diagnostics(){
    printf("\n---------DIAGNOSTICS START-------------\n\n");

    #ifdef TIME_SINCE_BOOT_DIAGNOSTICS
        printf("DIAGNOSTICS AT %f SECS SINCE BOOT \n\n", get_seconds_since_boot());
    #endif

    #ifdef FULL_CONTROLLER_DIAGNOSTICS
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
    #endif

    #ifdef CRSF_DIAGNOSTICS
        printf("--------CRSF INFO---------\n");
        printf("crsf_link_quality: %d \n", bot_state.crsf_link_quality);
        printf("crsf_rssi: %d \n", bot_state.crsf_rssi);
        printf("crsf_snr: %d \n", bot_state.crsf_snr);
        printf("crsf_tx_power: %d \n\n", bot_state.crsf_tx_power);
    #endif

    #ifdef MOTOR1_DIAGNOSTICS
        printf("--------MOTOR1 TELEMETRY INFO---------\n");
        printf("bidir_m1_erpm: %d | ", bot_state.bidir_m1_erpm);
        printf("bidir_m1_voltage: %d | ", bot_state.bidir_m1_voltage);
        printf("bidir_m1_current: %d \n", bot_state.bidir_m1_current);
        printf("bidir_m1_temperature: %d | ", bot_state.bidir_m1_temperature);
        printf("bidir_m1_status: %d | ", bot_state.bidir_m1_status);
        printf("bidir_m1_stress: %d \n\n", bot_state.bidir_m1_stress);
    #endif

    #ifdef MOTOR2_DIAGNOSTICS
        printf("--------MOTOR2 TELEMETRY INFO---------\n");
        printf("bidir_m2_erpm: %d | ", bot_state.bidir_m2_erpm);
        printf("bidir_m2_voltage: %d | ", bot_state.bidir_m2_voltage);
        printf("bidir_m2_current: %d \n", bot_state.bidir_m2_current);
        printf("bidir_m2_temperature: %d | ", bot_state.bidir_m2_temperature);
        printf("bidir_m2_status: %d | ", bot_state.bidir_m2_status);
        printf("bidir_m2_stress: %d \n\n", bot_state.bidir_m2_stress);
    #endif

    #ifdef OTHER_DIAGNOSTICS
        printf("--------OTHER INFO---------\n");
        printf("is_failsafed: %d \n", bot_state.is_failsafed);
        printf("require_zero_throttle: %d \n", bot_state.require_zero_throttle);
        printf("get_curr_drive_mode: %d \n", get_curr_drive_mode());
        printf("is_killswitch_active: %d \n", is_killswitch_active());
        printf("is_throttle_zero: %d \n", is_throttle_zero());
        printf("get_left_throttle_percent %lf \n", get_left_throttle_percent());
        printf("get_right_throttle_percent %lf \n\n", get_right_throttle_percent());
    #endif

    printf("---------DIAGNOSTICS END-------------\n\n");
}

void send_telemetry(){
    switch(get_telemetry_state()){
        case TELEMETRY_MOTOR1:
            printf("SENDING MOTOR1");
            receiver_send_telemetry(1,1,1,1);
            break;
        case TELEMETRY_MOTOR2:
            printf("SENDING MOTOR2");
            receiver_send_telemetry(2,2,2,2);
            break;
        case TELEMETRY_MAIN:
            printf("SENDING MAIN");
            receiver_send_telemetry(bot_state.require_zero_throttle,bot_state.is_failsafed,get_seconds_since_boot(),3);
            break;
    }
}

void update_bot_state(){
    printf("BOT STATE UPDATING \n");

    if (!is_throttle_zero()){
        drive_handle_tank(get_left_throttle_percent(), get_right_throttle_percent());
    } else {
        motor_stop_all();
    }

    // + test setting motor power values WITH DSHOT (MAKE SURE IT DOES EXACTLY WHAT INTENDED)
    // + add extended telemetry data to verbose diagnostics and then actually try to read it  
    //   - probably need to make c++ enum wrapper file (making it into c enum so we can use it) 
    //     but can just put that in c-pico-bidir-dshot folder / existing wrapper code probably

    // write auto esc configuring code w/ dshot once working
    //   - maybe if switch is active on startup then auto-configure escs otherwise don't

    // then just actually write the melty logic lol
}

int main(){
    stdio_init_all();
    init_bot_state();

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &bot_state);
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &bot_state);
    led_init(HEADING_LIGHT_STRIP_PIN);
    // accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        #ifdef SHOULD_SEND_CUSTOM_TELEMETRY_TO_TRANSMITTER
            send_telemetry();
        #endif

        if (bot_state.is_failsafed == 0 && bot_state.require_zero_throttle == 0 && !is_killswitch_active()){
            update_bot_state();
            led_time_blink(FAST_BLINK);
        } else { // a just connected or just powered on bot starts here
            motor_send_starting_zero_throttle();
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