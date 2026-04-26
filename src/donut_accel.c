#include "donut_accel.h"

static bot_state_t* _user_bot_state;

void accel_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl, bot_state_t* user_bot_state) {
    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);

    _user_bot_state = user_bot_state;
}

// gets microseconds per rotation
// lies about upr depending on what right_x_percent is and what LEFT_RIGHT_HEADING_CONTROL_DIVISOR is to adjust direction
double get_rpm(double right_x_percent) {
    double gs = accelerometer_get_y() - ACCEL_ZERO_G_OFFSET;
    _user_bot_state->accel_g_value = (uint16_t) (gs * 100);

    // mapping [0,1] -> [-1,1]
    right_x_percent = (right_x_percent*2) - 1;

    double effective_radius_in_cm = ACCEL_MOUNT_RADIUS_CM + (ACCEL_MOUNT_RADIUS_CM * right_x_percent * LEFT_RIGHT_HEADING_CONTROL_DIVISOR);

    double rpm = fabs(gs - ACCEL_ZERO_G_OFFSET) * 89445.0f;
    rpm = rpm / effective_radius_in_cm;
    rpm = sqrt(rpm);

    return rpm;
}

// returns a fake rpm from 0..2*FAKE_RPM, defaults to FAKE_RPM when right_x_percent == 0
double get_fake_rpm(double right_x_percent) {
    return FAKE_RPM*right_x_percent + FAKE_RPM;
}