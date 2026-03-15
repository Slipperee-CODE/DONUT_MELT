#include "motor_driver.h"

static BidirDShotX1* MOTOR1;
static BidirDShotX1* MOTOR2;

static bot_state_t* _user_bot_state;

void motor_init_all(int dshot_speed, int motor1_pin, PIO motor1_pio, int motor2_pin, PIO motor2_pio, bot_state_t* user_bot_state){
    MOTOR2 = BidirDShotX1_constructor(motor2_pin, dshot_speed, motor2_pio, -1);
    MOTOR1 = BidirDShotX1_constructor(motor1_pin, dshot_speed, motor1_pio, -1);

    _user_bot_state = user_bot_state;

    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + 6000){
        motor_send_all_starting_zero_command();
    }

    start_time = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) <= start_time + 1000){
        // pausing for a little bit before resuming the rest of the code
    }
}

// throttle range is 0-2000, 1000 in 3D mode (forward-backward) is stopped
void motor_motor1_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR1, throttle);
}

void motor_motor2_send_throttle(uint16_t throttle){
    BidirDShotX1_sendThrottle(MOTOR2, throttle);
}

void motor_stop_all(){
    motor_motor1_send_throttle(1000);
    motor_motor2_send_throttle(1000);
}

void motor_motor1_send_command(uint16_t command){
    BidirDShotX1_sendRaw11Bit(MOTOR1, command);
}

void motor_motor2_send_command(uint16_t command){
    BidirDShotX1_sendRaw11Bit(MOTOR2, command);
}

void motor_send_all_starting_zero_command(){
    motor_motor1_send_command(0);
    motor_motor2_send_command(0);
}

void motor_update_bot_state(){
    if (BidirDShotX1_checkTelemetryAvailable(MOTOR1)){
        uint32_t data1;
        BidirDshotX1TelemetryType data_type = BidirDShotX1_getTelemetryPacket(MOTOR1, &data1);

        switch (data_type) {
            case BIDIR_ERPM:
                _user_bot_state->bidir_m1_erpm = data1;
                break;
            case BIDIR_VOLTAGE:
                _user_bot_state->bidir_m1_voltage=data1;
                break;
            case BIDIR_CURRENT:
                _user_bot_state->bidir_m1_current=data1;
                break;
            case BIDIR_TEMPERATURE:
                _user_bot_state->bidir_m1_temperature=data1;
                break;
            case BIDIR_STATUS:
                _user_bot_state->bidir_m1_status=data1;
                break;
            case BIDIR_STRESS:
                _user_bot_state->bidir_m1_stress=data1;
                break;
        }
    }

    if (BidirDShotX1_checkTelemetryAvailable(MOTOR2)){
        uint32_t data2;
        BidirDshotX1TelemetryType data_type = BidirDShotX1_getTelemetryPacket(MOTOR1, &data2);

        switch (data_type) {
            case BIDIR_ERPM:
                _user_bot_state->bidir_m2_erpm = data2;
                break;
            case BIDIR_VOLTAGE:
                _user_bot_state->bidir_m2_voltage=data2;
                break;
            case BIDIR_CURRENT:
                _user_bot_state->bidir_m2_current=data2;
                break;
            case BIDIR_TEMPERATURE:
                _user_bot_state->bidir_m2_temperature=data2;
                break;
            case BIDIR_STATUS:
                _user_bot_state->bidir_m2_status=data2;
                break;
            case BIDIR_STRESS:
                _user_bot_state->bidir_m2_stress=data2;
                break;
        }
    }
}
