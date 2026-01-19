#include <stdio.h>
#include "pico/stdlib.h"
#include "donut_config.h"
#include "H3LIS331DL.h"
#include "c_pico_dshot.h"

int main()
{
    stdio_init_all();

    while (true){
        printf("DONUT MELT IS WORKING! \n");
        c_pico_dshot_is_library_accesible();
        pico_H3LIS331DL_is_library_accesible();
    }
}