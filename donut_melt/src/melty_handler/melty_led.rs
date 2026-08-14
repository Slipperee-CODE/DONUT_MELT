use std::fmt;

pub trait LedHandler: fmt::Debug {
    fn set_repeats(&mut self, repeats: u8);

    fn blink(&mut self);
}

#[derive(Debug)]
pub struct SingleLedHandler {
    led_pin: u8,
    time_btwn_blinks: f32,
    time_btwn_repeats: f32,
    repeats: u8,
    toggles: u16,
}

impl SingleLedHandler {
    fn new(led_pin: u8, time_btwn_blinks: f32, time_btwn_repeats: f32, repeats: u8) -> Self {
        Self { led_pin, time_btwn_blinks, time_btwn_repeats, repeats, toggles: 0 }    
    }
}

impl LedHandler for SingleLedHandler {
    fn set_repeats(&mut self, repeats: u8) {
        self.repeats = repeats;
        self.toggles = 1;
    }

    // make this function async and wait between blinks
    // probably run receiver packet-receiving/sending, motor command-sending, and LED blinking all
    // on a concurrent thread
    fn blink(&mut self) {
        loop {
            if self.toggles > self.repeats as u16 * 2 {
                self.toggles = 1;
                // long wait
            }

            if self.toggles == 1 {
                // set LED off 
            } else {
                // toggle LED state
            }
            // toggles += 1
            // short wait
        }
    }
}

#[derive(Debug)]
pub struct DoubleLedHandler {
    led1_handler: SingleLedHandler,
    led2_handler: SingleLedHandler,
}

impl DoubleLedHandler {
    fn new(led1_pin: u8, led2_pin: u8, time_btwn_blinks: f32, time_btwn_repeats: f32, repeats: u8) -> Self {
        Self { 
            led1_handler: SingleLedHandler { led_pin: led1_pin, time_btwn_blinks, time_btwn_repeats, repeats, toggles: 0 },
            led2_handler: SingleLedHandler { led_pin: led2_pin, time_btwn_blinks, time_btwn_repeats, repeats, toggles: 0 } 
        }    
    }
}

impl LedHandler for DoubleLedHandler {
    fn set_repeats(&mut self, repeats: u8) {
        self.led1_handler.set_repeats(repeats);
        self.led2_handler.set_repeats(repeats);
    }

    // make this function async and wait between blinks
    // probably run receiver packet-receiving/sending, motor command-sending, and LED blinking all
    // on a concurrent thread
    fn blink(&mut self) {
        self.led1_handler.blink();
        self.led2_handler.blink();
    }
}
