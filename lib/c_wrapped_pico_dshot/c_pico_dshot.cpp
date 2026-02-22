#include "c_pico_dshot.h"

extern "C" {
    struct BidirDShotX1Instance {
        BidirDShotX1 instance; // not a pointer, might become a problem later - Cai
    };

    BidirDShotX1Instance* BidirDShotX1_constructor(uint8_t pin, uint32_t speed, PIO pio, int8_t sm){
        return new BidirDShotX1Instance{ BidirDShotX1(pin, speed, pio, sm) };
    }

    void BidirDShotX1_destructor(BidirDShotX1Instance* bidirDShotX1Instance){
        if (bidirDShotX1Instance != nullptr){
            delete bidirDShotX1Instance;
        }
    }

    void BidirDShotX1_sendThrottle(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t throttle){
        bidirDShotX1Instance->instance.sendThrottle(throttle);
    }

    void BidirDShotX1_sendRaw11Bit(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t data){
        bidirDShotX1Instance->instance.sendRaw11Bit(data);
    }

    void BidirDShotX1_sendRaw12Bit(BidirDShotX1Instance* bidirDShotX1Instance, uint16_t data){
        bidirDShotX1Instance->instance.sendRaw12Bit(data);
    }

    bool BidirDShotX1_checkTelemetryAvailable(BidirDShotX1Instance* bidirDShotX1Instance){
        return bidirDShotX1Instance->instance.checkTelemetryAvailable();
    }

    BidirDshotTelemetryType BidirDShotX1_getTelemetryErpm(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *erpm){
        return bidirDShotX1Instance->instance.getTelemetryErpm(erpm);
    }

    BidirDshotTelemetryType BidirDShotX1_getTelemetryPacket(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *value){
        return bidirDShotX1Instance->instance.getTelemetryPacket(value);
    }

    BidirDshotTelemetryType BidirDShotX1_getTelemetryRaw(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t *value){
        return bidirDShotX1Instance->instance.getTelemetryRaw(value);
    }

    static uint32_t BidirDShotX1_convertFromRaw(BidirDShotX1Instance* bidirDShotX1Instance, uint32_t raw, BidirDshotTelemetryType type){
        return bidirDShotX1Instance->instance.convertFromRaw(raw, type);
    }

    void c_pico_dshot_is_library_accesible(){
        printf("Accessing the c_pico_dshot library is functioning properly! \n");
    }
}