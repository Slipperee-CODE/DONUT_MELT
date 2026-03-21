#include "pico/stdlib.h"

int main() {
    // Specify the onboard LED pin (GPIO 25)
    const uint LED_PIN1 = 6;
    const uint LED_PIN2 = 25;
    
    // Initialize GPIO
    gpio_init(LED_PIN1);
    gpio_set_dir(LED_PIN1, GPIO_OUT);

    gpio_init(LED_PIN2);
    gpio_set_dir(LED_PIN2, GPIO_OUT);
    
    while (true) {
        gpio_put(LED_PIN1, 1); // Turn LED on
        gpio_put(LED_PIN2, 1); // Turn LED on
        sleep_ms(500);        // Wait 500ms
        gpio_put(LED_PIN1, 0); // Turn LED off
        gpio_put(LED_PIN2, 0); // Turn LED off
        sleep_ms(500);        // Wait 500ms
    }
}