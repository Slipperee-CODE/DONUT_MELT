#include <stdio.h>
#include "c_pico_dshot.h"
#include "dshot_encoder.h"

// Wrap all pico_dshot functions for c here

void c_pico_dshot_is_library_accesible(){
    printf("Accessing the c_pico_dshot library is functioning properly! \n");
}