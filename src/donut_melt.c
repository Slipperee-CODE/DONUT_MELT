#include <stdio.h>
#include "pico/stdlib.h"
#include "donut_config.h"
#include "H3LIS331DL.h"
#include "c_pico_dshot.h"
#include "crsf.h"

/*
int main()
{
    stdio_init_all();

    while (true){
        printf("DONUT MELT IS WORKING! \n");
        c_pico_dshot_is_library_accesible();
        pico_H3LIS331DL_is_library_accesible();
        pico_crsf_is_library_accesible();
    }

    return 0;
}
*/

int main(){
    //initialize stdio_init_all();
    //initialize watchdog
    //initialize accelerometer
    //initialize motors/set their power to 0
    //initialize leds
    //initialize crsf data receiving
    
    //make diagnostics toggleable using #define in donut config 
    //wait for 0 throttle and killswitch off before starting to spin (no matter what, i.e. if power down or transmitter dced)
        //make receiver.c and receiver.h which handle storing channel data in structs which I then access from this file and donutcontrol.c
        //hardcode storing specific channel in specific struct instance in get_receiver_channels() or update_receiver_channels() (<- do this) to update persisting global instances

    //stop robot if transmitter disconnects -> store some global value in crsf.c that represents how long it has been since we succesfully read uart, make a function like crsf_has_been_more_than_x_seconds_since_last_read(seconds) <- if that then return;

    //go to same failsafe state/function if watchdog triggers or transmitter dcs (require throttle 0 again, blink in specific way)

    //make donutcontrol.c and donutcontrol.h
        //include handle_idle(), handle_spin(), handle_tank() functions (call from here) which use pretty close to openmelt logic (or tank drive with both sticks)

    //make led_driver.c and led_driver.h (make private library in lib folder)
        //make multiple non-blocking led blink patterns that you call different functions for or select with a number that is abstracted into a #define (could use openmelt logic, very solid)
            //i.e. #define FAST_BLINK 0 
            // #define SLOW_BLINK 1
            // PROBABLY USE ENUM STORED IN HEADER FILE INSTEAD OF A BUNCH OF DEFINES BECAUSE ENUM IS AUTO NUMBERED AND SEEMS TO COMPILE TO SIMILAR THING

    //remember to flash proper esc firmware for bidirectional am32 movement (for tank)

    return 0;
}