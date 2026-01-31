#include "donut_drive.h"

void drive_handle_idle(){
    motor_stop_all();
}

void drive_handle_spin(double throttle){
    motor_send_throttle(MOTOR1_PIN, throttle);
    motor_send_throttle(MOTOR2_PIN, throttle); //was 1-throttle
}

void drive_handle_tank(double left_y, double right_y){
    motor_send_throttle(MOTOR1_PIN, left_y);
    motor_send_throttle(MOTOR2_PIN, right_y);
}