#include "donut_drive.h"

void drive_handle_idle(){
    motor_stop_all();
}

void drive_handle_spin(double throttle){
    printf("HANDLING SPIN WITH %lf throttle", throttle);
    motor_motor1_send_throttle(throttle);
    motor_motor2_send_throttle(throttle);
}

void drive_handle_tank(double left_throttle, double right_throttle){
    printf("HANDLING TANK WITH %lf left_throttle and %lf right_throttle", left_throttle, right_throttle);
    motor_motor1_send_throttle(left_throttle);
    motor_motor2_send_throttle(right_throttle);
}