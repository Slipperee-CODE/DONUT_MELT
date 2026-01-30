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
    printf("These are the diagnostics lol! \n");
}

void wait_for_zero_throttle_and_receiver_connection(){
    while (receiver_check_if_disconnected()){ 
        led_repeated_blink(2); 
        watchdog_update(); 
    }
}

int get_drive_mode(){
    if (switch_c.percent_of_max > 0.1){
        return TANK_DRIVE;
    }
    return MELTY_DRIVE;
}

int main(){
    stdio_init_all();
    watchdog_enable(WATCH_DOG_TIMEOUT_MS, 0);
    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
    motor_init_all(MOTOR1_PIN, MOTOR1_PIO, MOTOR2_PIN, MOTOR2_PIO);
    receiver_init(RECEIVER_UART_ID, RECEIVER_UART_TX_PIN, RECEIVER_UART_RX_PIN);
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