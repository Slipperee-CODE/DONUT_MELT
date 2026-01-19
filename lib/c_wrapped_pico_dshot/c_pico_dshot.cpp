#include <stdio.h>
#include "c_pico_dshot.h"
#include "dshot_encoder.h"

struct DShotEncoderInstance {
    DShotEncoder instance;
};

extern "C" {
    DShotEncoderInstance* DShotEncoder_create(uint dshot_gpio, PIO pio){
        return new DShotEncoderInstance{
            .instance = DShotEncoder(dshot_gpio, pio)
        };
    }

    bool DShotEncoder_init(DShotEncoderInstance* dshot_encoder_instance, bool enable_repeat){
        return dshot_encoder_instance->instance.init(enable_repeat);
    }

    void DShotEncoder_destroy(DShotEncoderInstance* dshot_encoder_instance){
        if (dshot_encoder_instance != nullptr){
            delete dshot_encoder_instance;
        }
    }

    void DShotEncoder_sendCommand(DShotEncoderInstance* dshot_encoder_instance, uint16_t c){
        return dshot_encoder_instance->instance.sendCommand(c);
    }

    void DShotEncoder_sendThrottle(DShotEncoderInstance* dshot_encoder_instance, double t){
        return dshot_encoder_instance->instance.sendThrottle(t);
    }

    void DShotEncoder_stop(DShotEncoderInstance* dshot_encoder_instance){
        return dshot_encoder_instance->instance.stop();
    }

    void c_pico_dshot_is_library_accesible(){
        printf("Accessing the c_pico_dshot library is functioning properly! \n");
    }
}