#include "pico/stdlib.h"

int main() {
    // Specify the onboard LED pin (GPIO 25)
    const uint LED_PIN = 25;
    
    // Initialize GPIO
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (true) {
        gpio_put(LED_PIN, 1); // Turn LED on
        sleep_ms(500);        // Wait 500ms
        gpio_put(LED_PIN, 0); // Turn LED off
        sleep_ms(500);        // Wait 500ms
    }
}