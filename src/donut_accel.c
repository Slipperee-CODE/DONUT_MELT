#include "donut_accel.h"

void accel_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl) {
    accelerometer_init(ACCEL_I2C_PORT, ACCEL_I2C_SDA, ACCEL_I2C_SCL);
}

// gets microseconds per rotation
// lies about upr depending on what right_x_percent is and what LEFT_RIGHT_HEADING_CONTROL_DIVISOR is to adjust direction
double get_rpm(double right_x_percent) {
    // fix this: get rpm from accel data along correct axis, WILL PROBABLY NEED TO CHANGE THE AXIS LATER
    double x_gs = accelerometer_get_x() - ACCEL_ZERO_G_OFFSET;

    // mapping [0,1] -> [-1,1]
    right_x_percent = (right_x_percent*2) - 1;

    double effective_radius_in_cm = ACCEL_MOUNT_RADIUS_CM + (ACCEL_MOUNT_RADIUS_CM * right_x_percent * LEFT_RIGHT_HEADING_CONTROL_DIVISOR);

    double rpm = fabs(x_gs - ACCEL_ZERO_G_OFFSET) * 89445.0f;
    rpm = rpm / effective_radius_in_cm;
    rpm = sqrt(rpm);

    return rpm;
}

double get_fake_rpm(double right_x_percent) {
    return FAKE_RPM;
}