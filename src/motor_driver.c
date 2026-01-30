#include "motor_driver.h"

void motor_init_all(int motor1_pin, pio_hw_t* motor1_pio, int motor2_pin, pio_hw_t* motor2_pio){
    MOTOR1 = DShotEncoder_create(motor1_pin, motor1_pio);
    MOTOR2 = DShotEncoder_create(motor2_pin, motor2_pio);
    DShotEncoder_init(MOTOR1, true);
    DShotEncoder_init(MOTOR2, true);
    motor_stop_all(); // stopping all motors for sanity
}

void motor_stop_all(){
    motor_send_throttle(MOTOR1, 0);
    motor_send_throttle(MOTOR2, 0);
    DShotEncoder_stop(MOTOR1);
    DShotEncoder_stop(MOTOR2);
}

void motor_send_throttle(DShotEncoderInstance* motor, double t){
    DShotEncoder_sendThrottle(motor, t);
}