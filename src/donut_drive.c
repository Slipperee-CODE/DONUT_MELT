#include "donut_drive.h"

void drive_handle_idle(){
    motor_stop_all();
}

void drive_handle_spin(double throttle){
    printf("NOT FIXED YET - HANDLING SPIN WITH %d throttle", (uint16_t) 2000*throttle);
    motor_motor1_send_throttle((uint16_t) 2000*throttle);
    motor_motor2_send_throttle((uint16_t) 2000*throttle);
}

void drive_handle_tank(double left_throttle, double right_throttle){
    uint16_t int_left_throttle = (uint16_t) (2000*left_throttle);
    uint16_t int_right_throttle = (uint16_t) (2000*right_throttle);

    printf("HANDLING TANK WITH %d left_throttle and %d right_throttle", int_left_throttle, int_right_throttle);
    motor_motor1_send_throttle(int_left_throttle);
    motor_motor2_send_throttle(int_right_throttle);
}