#include "donut_config.h"
#include "donut_drive.h"
#include "led_driver.h"
#include "motor_driver.h"
#include "receiver.h"
#include "H3LIS331DL.h"

// TO DO:
 // IMPLEMENT ALL OTHER MISSING LOGIC (MOSTLY DRIVE) 
 //remember to flash proper esc firmware for bidirectional am32 movement (for tank)
 
void output_diagnostics(){
    printf("Accelerometer X: %lf \n", accelerometer_get_x());
    printf("Receiver Health: %u", receiver_check_if_disconnected());
    printf("Current Drive Mode: %u", get_drive_mode());
    printf("left_joystick_x: %u \n", left_joystick_x.raw_ticks);
    printf("left_joystick_y: %lf \n", left_joystick_y.raw_ticks);
    printf("right_joystick_x: %lf \n", right_joystick_x.raw_ticks);
    printf("right_joystick_y: %lf \n", right_joystick_y.raw_ticks);

    #ifdef OUTPUT_VERBOSE_DIAGNOSTICS
        printf("switch_b: %lf \n", switch_b.raw_ticks);
        printf("switch_c: %lf \n", switch_c.raw_ticks);
        printf("switch_e: %lf \n", switch_e.raw_ticks);
        printf("switch_f: %lf \n", switch_f.raw_ticks);
        printf("knob_s1: %lf \n", knob_s1.raw_ticks);
        printf("knob_s2: %lf \n", knob_s2.raw_ticks);
    #endif
}

void wait_for_zero_throttle_and_receiver_connection(){
    while (receiver_check_if_disconnected()){ 
        led_repeated_blink(2); 
        watchdog_update(); 
    }
}

uint8_t get_drive_mode(){
    if (switch_c.percent_of_max > 0.1){
        return TANK_DRIVE;
    }
    return MELTY_DRIVE;
}

int main(){
    stdio_init_all();
    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN);
    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    motor_init_all(MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO);
    led_init(HEADING_LIGHT_STRIP_PIN);

    wait_for_zero_throttle_and_receiver_connection();

    while (1){
        if (switch_e.percent_of_max == 0){ continue; } // killswitch

        #ifdef OUTPUT_DIAGNOSTICS
            output_diagnostics();
        #endif

        watchdog_update(); // keep watchdog happy

        if (receiver_check_if_disconnected()){ wait_for_zero_throttle_and_receiver_connection(); }

        if (left_joystick_x.percent_of_max == 0){ drive_handle_idle(); continue; }

        if (get_drive_mode() == MELTY_DRIVE){ drive_handle_spin(); }

        if (get_drive_mode() == TANK_DRIVE){ drive_handle_tank(); } 
    }

    return 0;
}