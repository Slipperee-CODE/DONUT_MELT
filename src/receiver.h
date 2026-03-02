#ifndef RECEIVER_H
#define RECEIVER_H

// Used in header file and c file
#include <stdint.h>
#include "hardware/uart.h"

// Used in c file
#include "donut_config.h"
#include "crsf.h"

void receiver_init(uart_inst_t* uart_id, int tx_pin, int rx_pin);

void receiver_update();

#endif