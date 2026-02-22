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

typedef struct BidirDShotX1Instance BidirDShotX1Instance;

BidirDShotX1Instance* BidirDShotX1_constructor(uint8_t pin, uint32_t speed, PIO pio, int8_t sm);

void BidirDShotX1_destructor(BidirDShotX1Instance* bidirDShotX1Instance);

void BidirDShotX1_sendThrottle(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t throttle);

void BidirDShotX1_sendRaw11Bit(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t data);

void BidirDShotX1_sendRaw12Bit(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t data);

bool BidirDShotX1_checkTelemetryAvailable(BidirDShotX1Instance* bidirDShotX1Instance);

BidirDshotTelemetryType BidirDShotX1_getTelemetryErpm(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *erpm);

BidirDshotTelemetryType BidirDShotX1_getTelemetryPacket(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *value);

BidirDshotTelemetryType BidirDShotX1_getTelemetryRaw(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *value);

static uint32_t BidirDShotX1_convertFromRaw(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t raw, BidirDshotTelemetryType type);

void c_pico_dshot_is_library_accesible();

#ifdef __cplusplus
}
#endif

#endif