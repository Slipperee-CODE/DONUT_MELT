#include <stdint.h>
#include "pico/stdlib.h"
#include "donut_config.h"

int main()
{
    stdio_init_all();

    while (true){
        printf("Hello World!");
    }
}