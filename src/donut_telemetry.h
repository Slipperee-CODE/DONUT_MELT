#ifndef DONUT_TELEMETRY_H
#define DONUT_TELEMETRY_H

#include "donut_config.h"
#include "receiver.h"
#include "stdint.h"

uint32_t get_seconds_since_boot();

void output_diagnostics(bot_state_t bot_state);

void send_telemetry(bot_state_t bot_state);

#endif