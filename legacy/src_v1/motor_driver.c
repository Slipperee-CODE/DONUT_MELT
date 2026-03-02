#include "motor_driver.h"

static BidirDShotX1* MOTOR1;
static BidirDShotX1* MOTOR2;

void motor_init_all(){
    MOTOR1 = BidirDShotX1_constructor(MOTOR1_PIN, DSHOT_SPEED, MOTOR1_PIO, -1);
    MOTOR2 = BidirDShotX1_constructor(MOTOR2_PIN, DSHOT_SPEED, MOTOR2_PIO, -1);
    motor_stop_all(); // stopping all motors for sanity
}

void motor_stop_all(){
    motor1_send_throttle(1000);
    motor2_send_throttle(1000);
}

// throttle range is 0-2000, 1000 in 3D mode (forward-backward) is stopped
void motor1_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR1, throttle);
}

void motor2_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR2, throttle);
}