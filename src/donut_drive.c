#include "donut_drive.h"

void drive_handle_idle(){
    led_time_blink(TIME_BETWEEN_FAST_BLINKS);
}

void drive_handle_spin(double throttle){
    motor_send_throttle(MOTOR1, throttle);
    motor_send_throttle(MOTOR2, 1-throttle);
}

void drive_handle_tank(double left_y, double right_y){
    motor_send_throttle(MOTOR1, left_y);
    motor_send_throttle(MOTOR2, right_y);
}