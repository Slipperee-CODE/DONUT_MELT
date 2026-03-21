#include "pico/stdlib.h"
#include "stdio.h"

int main() {
    stdio_init_all();
    
    uint number_of_pins = 3;
    uint pins[number_of_pins] = {9, 10, 22}

    for (int i = 0; i < number_of_pins; i++){
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_OUT);
    }
    
    while (true) {
        for (int i = 0; i < number_of_pins; i++){
            gpio_put(pins[i], 1);
        }
    
        printf("LOOPING TO POWER ALL LEDS \n");
    }
}