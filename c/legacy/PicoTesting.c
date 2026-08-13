/*
#include <stdio.h>
#include "pico/stdlib.h"
#include "crsf.h"

void on_rc_channels(const uint16_t channels[16]) {
  printf("Channel 1: %d\n", TICKS_TO_US(channels[0]));
  printf("Channel 2: %d\n", TICKS_TO_US(channels[1]));
  printf("Channel 3: %d\n", TICKS_TO_US(channels[2]));
  printf("Channel 4: %d\n", TICKS_TO_US(channels[3]));
  printf("Channel 5: %d\n", TICKS_TO_US(channels[4]));
  printf("Channel 6: %d\n", TICKS_TO_US(channels[5]));
  printf("Channel 7: %d\n", TICKS_TO_US(channels[6]));
  printf("Channel 8: %d\n", TICKS_TO_US(channels[7]));
}

void on_link_stats(const link_statistics_t link_stats) {
  printf("RSSI: %d\n", link_stats.rssi);
  printf("Link Quality: %d\n", link_stats.link_quality);
  printf("SNR: %d\n", link_stats.snr);
  printf("TX Power: %d\n", link_stats.tx_power);
}

void on_failsafe(const bool failsafe) {
  printf("Failsafe: %d\n", failsafe);
}

int main() {
  stdio_init_all();
    sleep_ms(2000);
    crsf_begin(uart1, 4, 5);
  crsf_set_link_quality_threshold(70);
  crsf_set_rssi_threshold(105);

  crsf_set_on_rc_channels(on_rc_channels);
  crsf_set_on_link_statistics(on_link_stats);
  crsf_set_on_failsafe(on_failsafe);
  
  

  while(1){
    //printf("THIS RAN LOL \n");
    crsf_process_frames();
  } 
}

void set_battery() {
  crsf_telem_set_battery_data(0, 0, 0, 0);
}

/* #include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

int main() {
    stdio_init_all();
    
    // Explicitly init UART1 at 420000
    
    // Set the GPIO function to UART
    gpio_set_function(4, GPIO_FUNC_UART); // TX
    gpio_set_function(5, GPIO_FUNC_UART); // RX

    // Optional: Turn off flow control (CRSF doesn't use it)
    uart_set_hw_flow(uart1, false, false);

    printf("Listening for CRSF...\n");

    while (true) {
        if (uart_is_readable(uart1)) {
            uint8_t ch = uart_getc(uart1);
            if (ch == 0xC8) {
                printf("Valid Sync Byte Found: %02x\n", ch);
            } else {
                // Print invalid bytes to analyze patterns
                // printf("%02x ", ch); 
            }
        }
    }
} */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// CRSF Protocol Constants
#define CRSF_SYNC_BYTE 0xC8
#define CRSF_MAX_PACKET_LEN 64

// Buffer state
uint8_t crsf_buf[CRSF_MAX_PACKET_LEN];
int crsf_idx = 0;
int crsf_len = 0;

void process_crsf_packet(uint8_t *packet) {
    uint8_t type = packet[2];

    // Type 0x16 is RC Channels (Joysticks)
    if (type == 0x16) {
        // Payload starts at packet[3]
        // CRSF channels are 11-bit packed. This is a simplified example of unpacking:
        // You can map these raw bits to your stick values.
        uint16_t ch0 = (packet[3] | (packet[4] << 8)) & 0x07FF;
        uint16_t ch1 = ((packet[4] >> 3) | (packet[5] << 5)) & 0x07FF;
        uint16_t ch2 = ((packet[5] >> 6) | (packet[6] << 2) | (packet[7] << 10)) & 0x07FF;
        uint16_t ch3 = ((packet[7] >> 1) | (packet[8] << 7)) & 0x07FF;
        
        printf("Roll: %d | Pitch: %d | Throttle: %d | Yaw: %d\n", ch0, ch1, ch2, ch3);
    }
}

int main() {
    stdio_init_all();
    
    // Your WORKING init code
    uart_init(uart1, 420000);
    gpio_set_function(8, GPIO_FUNC_UART);
    gpio_set_function(9, GPIO_FUNC_UART);
    //uart_set_hw_flow(uart1, false, false);

    printf("CRSF Parser Started...\n");

    while (true) {
        if (uart_is_readable(uart1)) {
            uint8_t ch = uart_getc(uart1);

            // State Machine to find packets
            if (crsf_idx == 0) {
                // Looking for Sync Byte
                if (ch == CRSF_SYNC_BYTE) {
                    crsf_buf[crsf_idx++] = ch;
                }
            } else if (crsf_idx == 1) {
                // Length Byte (Payload length + 2 for Type and CRC)
                crsf_len = ch;
                if (crsf_len > 62 || crsf_len < 2) { // Sanity check
                    crsf_idx = 0; // Invalid length, reset
                } else {
                    crsf_buf[crsf_idx++] = ch;
                }
            } else {
                // Read rest of packet
                crsf_buf[crsf_idx++] = ch;
                // Check if we have received the full packet
                // Total bytes = 1 (Sync) + 1 (Len) + Len
                if (crsf_idx >= crsf_len + 2) {
                    process_crsf_packet(crsf_buf);
                    crsf_idx = 0; // Reset for next packet
                }
            }
        }
    }
}