#include "motor_driver.h"

/*
DShotEncoderInstance* MOTOR1;
DShotEncoderInstance* MOTOR2;

void motor_init_all(int motor1_pin, pio_hw_t* motor1_pio, int motor2_pin, pio_hw_t* motor2_pio){
    MOTOR1 = DShotEncoder_create(motor1_pin, motor1_pio);
    MOTOR2 = DShotEncoder_create(motor2_pin, motor2_pio);
    DShotEncoder_init(MOTOR1, true);
    DShotEncoder_init(MOTOR2, true);
    motor_stop_all(); // stopping all motors for sanity
}

void motor_stop_all(){
    motor_send_throttle(MOTOR1, 0.5);
    motor_send_throttle(MOTOR2, 0.5);
}

void motor_send_throttle(DShotEncoderInstance* motor, double t){
    DShotEncoder_sendThrottle(motor, t);
}
*/

static int INNER_MOTOR1_PIN;
static int INNER_MOTOR2_PIN;

const uint16_t PWM_WRAP = 20000;

void motor_init_all(int motor1_pin, int motor2_pin){
    INNER_MOTOR1_PIN = motor1_pin;
    INNER_MOTOR2_PIN = motor2_pin;

    float sys_clk = (float) clock_get_hz(clk_sys);
    float global_clkdiv = sys_clk / 1000000.0f;

    gpio_set_function(INNER_MOTOR1_PIN, GPIO_FUNC_PWM);
    gpio_set_function(INNER_MOTOR2_PIN, GPIO_FUNC_PWM);

    uint slice1 = pwm_gpio_to_slice_num(INNER_MOTOR1_PIN);
    uint slice2 = pwm_gpio_to_slice_num(INNER_MOTOR2_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, global_clkdiv);
    pwm_config_set_wrap(&config, PWM_WRAP);

    pwm_init(slice1, &config, true);
    pwm_init(slice2, &config, true);

    motor_stop_all(); // stopping all motors for sanity
}

void motor_stop_all(){
    motor_send_throttle(INNER_MOTOR1_PIN, 0.5); 
    motor_send_throttle(INNER_MOTOR2_PIN, 0.5);
}

void motor_send_throttle(int motor_pin, double t){
    uint16_t level = 1000 + (uint16_t)(t * 1000.0);
    pwm_set_gpio_level(motor_pin, level);
}