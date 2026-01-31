#include "donut_config.h"
#include "donut_drive.h"
#include "led_driver.h"
#include "receiver.h"
#include "H3LIS331DL.h"

// TO DO:
 // IMPLEMENT ALL OTHER MISSING LOGIC (MOSTLY DRIVE) 
 //remember to flash proper esc firmware for bidirectional am32 movement (for tank)
 
uint8_t get_drive_mode(){
    if (switch_c.percent_of_max > 0.5){
        return TANK_DRIVE;
    }
    return MELTY_DRIVE;
}

void output_diagnostics(){
    printf("----------DIAGNOSTICS---------- \n \n");
    //printf("Accelerometer X: %lf \n", accelerometer_get_x());
    printf("Receiver Health: %u \n", receiver_check_if_disconnected()==0);
    printf("Current Drive Mode: %u \n", get_drive_mode());
    printf("Killswitch State: %lf \n", switch_e.percent_of_max);
    printf("left_joystick_x: %u \n", left_joystick_x.raw_ticks);
    printf("left_joystick_y: %u \n", left_joystick_y.raw_ticks);
    printf("right_joystick_x: %u \n", right_joystick_x.raw_ticks);
    printf("right_joystick_y: %u \n", right_joystick_y.raw_ticks);

    #ifdef OUTPUT_VERBOSE_DIAGNOSTICS
        printf("switch_b: %u \n", switch_b.raw_ticks);
        printf("switch_c: %u \n", switch_c.raw_ticks);
        printf("switch_e: %u \n", switch_e.raw_ticks);
        printf("switch_f: %u \n", switch_f.raw_ticks);
        printf("knob_s1: %u \n", knob_s1.raw_ticks);
        printf("knob_s2: %u \n", knob_s2.raw_ticks);
    #endif

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

        if (switch_e.percent_of_max > 0.5){ continue; } // killswitch

        watchdog_update(); // keep watchdog happy

        if (receiver_check_if_disconnected()){ wait_for_zero_throttle_and_receiver_connection(); }

        if (left_joystick_x.percent_of_max == 0){ drive_handle_idle(); continue; }

        if (get_drive_mode() == MELTY_DRIVE){ drive_handle_spin(); }

        if (get_drive_mode() == TANK_DRIVE){ drive_handle_tank(); } 
    }

    return 0;
}