#include <stdio.h>
#include "pico/stdlib.h"
#include "donut_config.h"
#include "receiver.h"
#include "crsf.h"

int main()
{
    stdio_init_all();

    receiver_setup();
    
    while (1){
        uint64_t num = 1000;
        while (--num > 0){
            crsf_process_frames();
            //printf("proccesing frames \n");
            //printf("right_stick_x: %d \n", right_stick_x.raw_ticks);
        }

        printf("%d", left_stick_x.raw_ticks);
    }
}