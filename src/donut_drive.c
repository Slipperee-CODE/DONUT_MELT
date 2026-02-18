#include "donut_drive.h"

void drive_handle_idle(){
    motor_stop_all();
}

void drive_handle_spin(double throttle){
    printf("HANDLING SPIN WITH %lf THROTTLE (LESS THAN 0.5 IS BACKWARDS) \n", 1-throttle);
    motor_send_throttle(MOTOR1_PIN, 1-throttle);
    motor_send_throttle(MOTOR2_PIN, 1-throttle);
}

void drive_handle_tank(double left_y, double right_y){
    printf("HANDLING TANK WITH %lf left_y and %lf right_y (LESS THAN 0.5 IS BACKWARDS) \n", 1-left_y, right_y);
    motor_send_throttle(MOTOR1_PIN, 1-left_y);
    motor_send_throttle(MOTOR2_PIN, right_y);
}