#ifndef C_PICO_DSHOT_H
#define C_PICO_DSHOT_H

// Used in header file and c file
#include <stdint.h>
#include <stdio.h>
#include "hardware/pio.h"
#include "bidir_dshot_x1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BidirDShotX1 BidirDShotX1;

BidirDShotX1* BidirDShotX1_constructor(uint8_t pin, uint32_t speed, PIO pio, int8_t sm);

void BidirDShotX1_destructor(BidirDShotX1* bidirDShotX1);

void BidirDShotX1_sendThrottle(BidirDShotX1* bidirDShotX1, uint16_t throttle);

void BidirDShotX1_sendRaw11Bit(BidirDShotX1* bidirDShotX1, uint16_t data);

void BidirDShotX1_sendRaw12Bit(BidirDShotX1* bidirDShotX1, uint16_t data);

bool BidirDShotX1_checkTelemetryAvailable(BidirDShotX1* bidirDShotX1);

BidirDshotTelemetryType BidirDShotX1_getTelemetryErpm(BidirDShotX1* bidirDShotX1, uint32_t *erpm);

BidirDshotTelemetryType BidirDShotX1_getTelemetryPacket(BidirDShotX1* bidirDShotX1, uint32_t *value);

BidirDshotTelemetryType BidirDShotX1_getTelemetryRaw(BidirDShotX1* bidirDShotX1, uint32_t *value);

static uint32_t BidirDShotX1_convertFromRaw(BidirDShotX1* bidirDShotX1, uint32_t raw, BidirDshotTelemetryType type);

void c_pico_dshot_is_library_accesible();

#ifdef __cplusplus
}
#endif

#endif