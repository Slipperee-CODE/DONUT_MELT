typedef struct {
    volatile uint16_t raw_ticks;
    uint8_t channel_id;
    volatile double percent_of_max;
    bool is_percent_signed; 
} channel_t;

extern channel_t right_stick_x;
extern channel_t right_stick_y;
extern channel_t left_stick_y;
extern channel_t left_stick_x;

void receiver_setup();