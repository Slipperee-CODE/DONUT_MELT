#include <stdio.h>
#include "pico/stdlib.h"
#include "donut_config.h"
#include "H3LIS331DL.h"

int main()
{
    stdio_init_all();

    while (true){
        does_library_work();
    }
}