#pragma once

#include <stdint.h>
#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DShotEncoderInstance DShotEncoderInstance;

DShotEncoderInstance* DShotEncoder_create(uint dshot_gpio, PIO pio);

bool DShotEncoder_init(DShotEncoderInstance* dshot_encoder_instance, bool enable_repeat);

void DShotEncoder_destroy(DShotEncoderInstance* dshot_encoder_instance);

void DShotEncoder_sendCommand(DShotEncoderInstance* dshot_encoder_instance, uint16_t c);

void DShotEncoder_sendThrottle(DShotEncoderInstance* dshot_encoder_instance, double t);

void DShotEncoder_stop(DShotEncoderInstance* dshot_encoder_instance);

void c_pico_dshot_is_library_accesible();

#ifdef __cplusplus
}
#endif
