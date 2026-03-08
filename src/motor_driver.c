#include "motor_driver.h"

static BidirDShotX1* MOTOR1;
static BidirDShotX1* MOTOR2;

static bot_state_t* _user_bot_state;

void motor_init_all(int dshot_speed, int motor1_pin, PIO motor1_pio, int motor2_pin, PIO motor2_pio, bot_state_t* user_bot_state){
    MOTOR1 = BidirDShotX1_constructor(motor1_pin, dshot_speed, motor1_pio, -1);
    MOTOR2 = BidirDShotX1_constructor(motor2_pin, dshot_speed, motor2_pio, -1);
    motor_stop_all(); // stopping all motors for sanity

    _user_bot_state = user_bot_state;
}

void motor_stop_all(){
    motor_motor1_send_throttle(1000);
    motor_motor2_send_throttle(1000);
}

// throttle range is 0-2000, 1000 in 3D mode (forward-backward) is stopped
void motor_motor1_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR1, throttle);
}

void motor_motor2_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR2, throttle);
}

void motor_update_bot_state(){
    if (BidirDShotX1_checkTelemetryAvailable(MOTOR1)){
        uint32_t data;
        BidirDshotX1TelemetryType data_type = BidirDShotX1_getTelemetryPacket(MOTOR1, data);

        switch (data_type) {
            case BIDIR_ERPM:
                _user_bot_state->bidir_m1_erpm = data;
                break;
            case BIDIR_VOLTAGE:
                _user_bot_state->bidir_m1_voltage=data;
                break;
            case BIDIR_CURRENT:
                _user_bot_state->bidir_m1_current=data;
                break;
            case BIDIR_TEMPERATURE:
                _user_bot_state->bidir_m1_temperature=data;
                break;
            case BIDIR_STATUS:
                _user_bot_state->bidir_m1_status=data;
                break;
            case BIDIR_STRESS:
                _user_bot_state->bidir_m1_stress=data;
                break;
        }
    }

    if (BidirDShotX1_checkTelemetryAvailable(MOTOR2)){
        uint32_t data;
        BidirDshotX1TelemetryType data_type = BidirDShotX1_getTelemetryPacket(MOTOR1, data);

        switch (data_type) {
            case BIDIR_ERPM:
                _user_bot_state->bidir_m2_erpm = data;
                break;
            case BIDIR_VOLTAGE:
                _user_bot_state->bidir_m2_voltage=data;
                break;
            case BIDIR_CURRENT:
                _user_bot_state->bidir_m2_current=data;
                break;
            case BIDIR_TEMPERATURE:
                _user_bot_state->bidir_m2_temperature=data;
                break;
            case BIDIR_STATUS:
                _user_bot_state->bidir_m2_status=data;
                break;
            case BIDIR_STRESS:
                _user_bot_state->bidir_m2_stress=data;
                break;
        }
    }
}
