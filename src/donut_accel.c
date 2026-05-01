#include "donut_accel.h"

static bot_state_t* _user_bot_state;
static accelerometer_t* accel_1;
static accelerometer_t* accel_2;

void accel_init(accelerometer_t* user_accel_1, accelerometer_t* user_accel_2, bot_state_t* user_bot_state) {
    accelerometer_init(user_accel_1, user_accel_2);

    accel_1 = user_accel_1;
    accel_2 = user_accel_2;

    _user_bot_state = user_bot_state;
}

// gets rpm
// lies about rpm depending on what right_x_percent is and what LEFT_RIGHT_HEADING_CONTROL_DIVISOR is to adjust direction
double get_rpm(double right_x_percent, double accel_offset_cm) {
    double raw_gs = accelerometer_get_y(accel_1);

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

// TEDDY ACCEL MATH STARTS NOW

// Vector math stuff for the multi-acceleromter calcs
Vector2D vec_subtract(Vector2D v1, Vector2D v2) {
    Vector2D result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

float vec_magnitude(Vector2D v) {
    return sqrtf((v.x * v.x) + (v.y * v.y));
}

// this function doesn't actually do anything with accel_offset_cm, it's just so the code
// doesn't error because I use a function pointer to a double get_rpm(double, double) 
// and the other get_rpm functions follow this pattern - Cai
double get_rpm_2accel(double right_x_percent, double accel_offset_cm) {
    // X,Y,Z of all data
    double* Accel1RawData = accelerometer_get_all_axis(accel_1);
    double* Accel2RawData = accelerometer_get_all_axis(accel_2);

    // Convert to 2D vector for math stuff
    Vector2D Accel1Data, Accel2Data, Accel1Position, Accel2Position;

    Accel1Data.x = Accel1RawData[0];
    Accel1Data.y = Accel1RawData[1];

    Accel2Data.x = Accel2RawData[0];
    Accel2Data.y = Accel2RawData[1];

    // subtract the offsets 
    // Replace this with actual code when accelerometer struct/storage is set up.
    double xoffset = 0.0;
    double yoffset = 0.0;
    Vector2D Accel1Offsets = {xoffset, yoffset};
    Vector2D Accel2Offsets = {xoffset, yoffset};

    Accel1Data = vec_subtract(Accel1Data, Accel1Offsets);
    Accel2Data = vec_subtract(Accel2Data, Accel2Offsets);


    // Position vectors of the accelerometers (in cm)
    Accel1Position.x = 1.0 * 1.0/sqrtf(2.0);
    Accel1Position.y = 1.0 * 1.0/sqrtf(2.0);

    Accel2Position.x = -1.0 * 1.0/sqrtf(2.0);
    Accel2Position.y = -1.0 * 1.0/sqrtf(2.0);


    // omega = sqrt [ ||(a1 - a2)|| / ||(r2 - r1)|| ]

    Vector2D delta_A = vec_subtract(Accel1Data, Accel2Data);
    Vector2D delta_pos = vec_subtract(Accel2Position, Accel1Position);

    float mag_delta_A = vec_magnitude(delta_A);
    float mag_delta_Pos = vec_magnitude(delta_pos);
    
    // 89445f converts from Gs to RPM using gravity and angular acceleration.
    double rpm = sqrtf((mag_delta_A / mag_delta_Pos) * 89445.0f); // Gemini is telling me to make everything floats for optimization purposes

    return rpm;
}