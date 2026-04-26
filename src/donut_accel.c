#include "donut_accel.h"

static bot_state_t* _user_bot_state;

void accel_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl, bot_state_t* user_bot_state) {
    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);

    _user_bot_state = user_bot_state;
}

// gets rpm
// lies about rpm depending on what right_x_percent is and what LEFT_RIGHT_HEADING_CONTROL_DIVISOR is to adjust direction
double get_rpm(double right_x_percent, double accel_offset_cm) {
    double raw_gs = accelerometer_get_y();

    // attempts to store 2 decimal places worth of raw_gs into a uint16_t
    _user_bot_state->accel_g_value = (uint16_t) (raw_gs * 100);

    // Reasoning behind the negative sign on "right_x_percent":
    // Centripetal acceleration formula is ac = w^2 * r, meaning w = sqrt(ac/r). 
    // When right_x_percent is negative, you add to the effective radius. 
    // A larger radius results in a smaller calculated software RPM. 
    // A smaller software RPM increases the calculated us_per_rotation, 
    // causing the LED to trigger later in the physical spin (shifting it left).
    double offset_accel_mount_radius_cm = ACCEL_MOUNT_RADIUS_CM + accel_offset_cm;
    double effective_radius_in_cm = offset_accel_mount_radius_cm + (offset_accel_mount_radius_cm * -right_x_percent * LEFT_RIGHT_HEADING_CONTROL_DIVISOR);

    double rpm = fabs(raw_gs - ACCEL_ZERO_G_OFFSET) * 89445.0f;
    rpm = rpm / effective_radius_in_cm;
    rpm = sqrt(rpm);

    return rpm;
}

// returns a fake rpm from 0..2*FAKE_RPM, defaults to FAKE_RPM when right_x_percent == 0
// double get_fake_rpm(double right_x_percent, double accel_offset_cm) {
//     return FAKE_RPM*right_x_percent + FAKE_RPM;
// }

double get_fake_rpm(double right_x_percent, double accel_offset_cm) {
    double raw_gs = 200;

    // attempts to store 2 decimal places worth of raw_gs into a uint16_t
    _user_bot_state->accel_g_value = (uint16_t) (raw_gs * 100);

    // Reasoning behind the negative sign on "right_x_percent":
    // Centripetal acceleration formula is ac = w^2 * r, meaning w = sqrt(ac/r). 
    // When right_x_percent is negative, you add to the effective radius. 
    // A larger radius results in a smaller calculated software RPM. 
    // A smaller software RPM increases the calculated us_per_rotation, 
    // causing the LED to trigger later in the physical spin (shifting it left).
    double offset_accel_mount_radius_cm = ACCEL_MOUNT_RADIUS_CM + accel_offset_cm;
    double effective_radius_in_cm = offset_accel_mount_radius_cm + (offset_accel_mount_radius_cm * -right_x_percent * LEFT_RIGHT_HEADING_CONTROL_DIVISOR);

    double rpm = fabs(raw_gs - ACCEL_ZERO_G_OFFSET) * 89445.0f;
    rpm = rpm / effective_radius_in_cm;
    rpm = sqrt(rpm);

    return rpm;
}