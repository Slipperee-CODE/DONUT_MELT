#include "donut_config.h"
#include "donut_drive.h"
#include "led_driver.h"
#include "receiver.h"
#include "H3LIS331DL.h"

uint8_t get_drive_mode(){
    return knob_s1.raw_ticks > RAW_TICK_NORMALIZER / 2;
}

void output_diagnostics(){
    printf("----------DIAGNOSTICS---------- \n \n");
    //printf("Accelerometer X: %lf \n", accelerometer_get_x());
    printf("Receiver Health: %u \n", receiver_check_if_disconnected()==0);
    printf("Current Drive Mode: %u \n", get_drive_mode());
    printf("left_joystick_x: %u \n", left_joystick_x.raw_ticks);
    printf("left_joystick_y: %u \n", left_joystick_y.raw_ticks);
    printf("right_joystick_x: %u \n", right_joystick_x.raw_ticks);
    printf("right_joystick_y: %u \n", right_joystick_y.raw_ticks);
    printf("\n ----------END DIAGNOSTICS---------- \n \n \n");
}

void wait_for_zero_throttle_and_receiver_connection(){
    while (receiver_check_if_disconnected() && fabs(right_joystick_y.raw_ticks / RAW_TICK_NORMALIZER - 0.5) > 0.1 && fabs(left_joystick_y.raw_ticks / RAW_TICK_NORMALIZER - 0.5) > 0.1){
        led_time_blink(TIME_BETWEEN_SLOW_BLINK); 
        
        #ifdef OUTPUT_DIAGNOSTICS
            printf("\n WAITING FOR ZERO THROTTLE OR RECEIVER CONNECTION \n \n");

            output_diagnostics();
        #endif

        drive_handle_idle();

        receiver_update();

        watchdog_update(); 
    }
}

int main(){
    stdio_init_all();
    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN);
    //accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    motor_init_all(MOTOR1_PIN, MOTOR2_PIN);
    led_init(HEADING_LIGHT_STRIP_PIN);

    wait_for_zero_throttle_and_receiver_connection();

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        led_time_blink(TIME_BETWEEN_FAST_BLINK); 

        receiver_update();

        watchdog_update(); // keep watchdog happy

        if (receiver_check_if_disconnected()){ wait_for_zero_throttle_and_receiver_connection(); }

        if (get_drive_mode() == 1){ drive_handle_spin( (double) right_joystick_y.raw_ticks / RAW_TICK_NORMALIZER); }

        if (get_drive_mode() == 0){ drive_handle_tank((double) left_joystick_y.raw_ticks / RAW_TICK_NORMALIZER, (double) right_joystick_y.raw_ticks / RAW_TICK_NORMALIZER); } 
    }

    return 0;
}