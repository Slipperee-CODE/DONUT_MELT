#include "donut_config.h"
#include "receiver.h"
#include "led_driver.h"
#include "motor_driver.h"
// #include "H3LIS331DL.h"


static bot_state_t state;

void init_bot_state(){
    state.crsf_link_quality = 0;
    state.crsf_rssi = 0;
    state.crsf_snr = 0;
    state.crsf_tx_power = 0;
    state.is_failsafed = 1;
    state.require_zero_throttle = 1;
}

uint8_t is_killswitch_active(){
    return receiver_get_channel(SWITCH_E) >= RECEIVER_HIGHEST_CHANNEL_VALUE;
}

uint8_t is_throttle_zero(){
    return receiver_get_channel(LEFT_JOYSTICK_Y) <= RECEIVER_LOWEST_CHANNEL_VALUE;
}

void output_diagnostics(){
    printf("\n---------DIAGNOSTICS START-------------\n\n");

    #ifdef TIME_SINCE_BOOT_DIAGNOSTICS
        printf("DIAGNOSTICS AT %f SECS SINCE BOOT \n\n", to_ms_since_boot(get_absolute_time()) / 1000.0);
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
        printf("crsf_link_quality: %d \n", state.crsf_link_quality);
        printf("crsf_rssi: %d \n", state.crsf_rssi);
        printf("crsf_snr: %d \n", state.crsf_snr);
        printf("crsf_tx_power: %d \n\n", state.crsf_tx_power);
    #endif

    #ifdef MOTOR1_DIAGNOSTICS
        printf("--------MOTOR1 TELEMETRY INFO---------\n");
        printf("bidir_m1_erpm: %d | ", state.bidir_m1_erpm);
        printf("bidir_m1_voltage: %d | ", state.bidir_m1_voltage);
        printf("bidir_m1_current: %d \n", state.bidir_m1_current);
        printf("bidir_m1_temperature: %d | ", state.bidir_m1_temperature);
        printf("bidir_m1_status: %d | ", state.bidir_m1_status);
        printf("bidir_m1_stress: %d \n\n", state.bidir_m1_stress);
    #endif

    #ifdef MOTOR2_DIAGNOSTICS
        printf("--------MOTOR2 TELEMETRY INFO---------\n");
        printf("bidir_m2_erpm: %d | ", state.bidir_m2_erpm);
        printf("bidir_m2_voltage: %d | ", state.bidir_m2_voltage);
        printf("bidir_m2_current: %d \n", state.bidir_m2_current);
        printf("bidir_m2_temperature: %d | ", state.bidir_m2_temperature);
        printf("bidir_m2_status: %d | ", state.bidir_m2_status);
        printf("bidir_m2_stress: %d \n\n", state.bidir_m2_stress);
    #endif

    #ifdef OTHER_DIAGNOSTICS
        printf("--------OTHER INFO---------\n");
        printf("is_failsafed: %d \n", state.is_failsafed);
        printf("is_killswitch_active: %d \n", is_killswitch_active());
        printf("is_throttle_zero: %d \n\n", is_throttle_zero());
    #endif

    printf("---------DIAGNOSTICS END-------------\n\n");
}

void update_bot_state(){
    printf("BOT STATE UPDATING \n");

    motor_motor1_send_throttle(1500);
    motor_motor2_send_throttle(1500);

    // TO DO:
    // + test setting motor power values WITH DSHOT (MAKE SURE IT DOES EXACTLY WHAT INTENDED)
    // + add extended telemetry data to verbose diagnostics and then actually try to read it  
    //   - probably need to make c++ enum wrapper file (making it into c enum so we can use it) 
    //     but can just put that in c-pico-bidir-dshot folder / existing wrapper code probably

    // write auto esc configuring code w/ dshot once working 
    //  

    // then just actually write the melty logic lol
}

int main(){
    stdio_init_all();
    init_bot_state();

    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN, 70, 105, &state);
    motor_init_all(DSHOT_SPEED, MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO, &state);
    led_init(HEADING_LIGHT_STRIP_PIN);
    // accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        if (state.is_failsafed == 0 && state.require_zero_throttle == 0 && !is_killswitch_active()){
            update_bot_state();
            led_time_blink(FAST_BLINK);
        } else { // a just connected or just powered on bot starts here
            motor_stop_all();
            state.require_zero_throttle = 1;

            led_time_blink(SLOW_BLINK);

            if (state.is_failsafed == 0 && is_throttle_zero()){
                state.require_zero_throttle = 0;
            }
        }

        // motor_update_bot_state();

        receiver_update();

        watchdog_update(); // keep watchdog happy
    }

    return 0;
}