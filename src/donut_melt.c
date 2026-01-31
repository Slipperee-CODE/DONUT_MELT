#include "donut_config.h"
#include "donut_drive.h"
#include "led_driver.h"
#include "receiver.h"
#include "H3LIS331DL.h"
 
uint8_t get_killswitch_state(){
    return knob_s1.percent_of_max > 0.5;
}

uint8_t get_drive_mode(){
    return knob_s1.percent_of_max > 0.5;
}

void output_diagnostics(){
    printf("----------DIAGNOSTICS---------- \n \n");
    //printf("Accelerometer X: %lf \n", accelerometer_get_x());
    printf("Receiver Health: %u \n", receiver_check_if_disconnected()==0);
    printf("Current Drive Mode: %u \n", get_drive_mode());
    //printf("Killswitch State: %u \n", get_killswitch_state());
    printf("left_joystick_x: %lf \n", left_joystick_x.percent_of_max);
    printf("left_joystick_y: %lf \n", left_joystick_y.percent_of_max);
    printf("right_joystick_x: %lf \n", right_joystick_x.percent_of_max);
    printf("right_joystick_y: %lf \n", right_joystick_y.percent_of_max);
    printf("\n ----------END DIAGNOSTICS---------- \n \n \n");
}

void wait_for_zero_throttle_and_receiver_connection(){
    while (receiver_check_if_disconnected()){
        led_time_blink(TIME_BETWEEN_SLOW_BLINK); 
        
        #ifdef OUTPUT_DIAGNOSTICS
            printf("\n WAITING FOR ZERO THROTTLE OR RECEIVER CONNECTION \n \n");

            output_diagnostics();
        #endif

        receiver_update();

        watchdog_update(); 
    }
}

int main(){
    stdio_init_all();
    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN);
    //accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    //motor_init_all(MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO);
    led_init(HEADING_LIGHT_STRIP_PIN);

    wait_for_zero_throttle_and_receiver_connection();

    while (1){
        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        led_time_blink(TIME_BETWEEN_FAST_BLINK); 

        receiver_update();

        watchdog_update(); // keep watchdog happy

        //if (get_killswitch_state()){ continue; } // killswitch

        if (receiver_check_if_disconnected()){ wait_for_zero_throttle_and_receiver_connection(); }

        if (left_joystick_x.percent_of_max <= 0.15){ drive_handle_idle(); continue; }

        if (get_drive_mode() == 1){ drive_handle_spin(); }

        if (get_drive_mode() == 0){ drive_handle_tank(); } 
    }

    return 0;
}