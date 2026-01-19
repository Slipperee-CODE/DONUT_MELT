#include <stdio.h>
#include "pico/stdlib.h"
#include "donut_config.h"
#include "H3LIS331DL.h"

int main()
{
    stdio_init_all();

    while (true){
        printf("DONUT MELT IS WORKING! \n");
        pico_H3LIS331DL_is_library_accesible();
    }
}