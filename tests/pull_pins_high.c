#include "pico/stdlib.h"
#include "stdio.h"

int main() {
    stdio_init_all();
    // Specify the onboard LED pin (GPIO 25)
    const uint LED_PIN1 = 10;
    const uint LED_PIN2 = 9;
    const uint LED_PIN3 = 22;

    // Initialize GPIO
    gpio_init(LED_PIN1);
    gpio_set_dir(LED_PIN1, GPIO_OUT);

    gpio_init(LED_PIN2);
    gpio_set_dir(LED_PIN2, GPIO_OUT);

    gpio_init(LED_PIN3);
    gpio_set_dir(LED_PIN3, GPIO_OUT);
    
    while (true) {
        gpio_put(LED_PIN1, 1);
        gpio_put(LED_PIN2, 1); 
        gpio_put(LED_PIN3, 1); 
        printf("LOOPING TO POWER ALL LEDS \n");
    }
}