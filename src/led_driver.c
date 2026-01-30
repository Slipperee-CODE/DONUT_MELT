#include "led_driver.h"

void led_init(int pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
}

void led_repeat_blink(int repeats){

}

void led_time_blink(int millis){
    
}