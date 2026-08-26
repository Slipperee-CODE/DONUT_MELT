use std::fmt;

#[derive(Debug)]
pub enum Controller {
    Single {
        time_btwn_blinks: f32, // in secs
    },
    Burst {
        time_btwn_blinks: f32, // in secs
        blinks_per_burst: u32,
        time_btwn_bursts: f32, // in secs
    },
}

impl Controller {
    const SLOW_BLINK: f32 = 1; // in secs
    const NORMAL_BLINK: f32 = 0.5; // in secs
    const FAST_BLINK: f32 = 0.1; // in secs
    const SINGLE_SLOW: Controller = Single { time_btwn_blinks: SLOW_BLINK };
    const SINGLE_NORMAL: Controller = Single { time_btwn_blinks: NORMAL_BLINK };
    const SINGLE_FAST: Controller = Single { time_btwn_blinks: FAST_BLINK };
    const BURST_2: Controller = Burst { time_btwn_blinks: NORMAL_BLINK, blinks_per_burst: 2, time_btwn_bursts: SLOW_BLINK};
    const BURST_3: Controller = Burst { time_btwn_blinks: NORMAL_BLINK, blinks_per_burst: 3, time_btwn_bursts: SLOW_BLINK};
    const BURST_4: Controller = Burst { time_btwn_blinks: NORMAL_BLINK, blinks_per_burst: 4, time_btwn_bursts: SLOW_BLINK};
}

pub trait LedHandler: fmt::Debug {
    fn set_state(&mut self, state: bool);
    fn blink(&mut self, controller: Controller);
}

#[derive(Debug)]
pub struct SingleLedHandler {
    led_pin: u8,
    last_controller: f32,
    last_toggle: f32,
    toggles: u16,
}

impl SingleLedHandler {
    fn new(led_pin: u8) -> Self {
        Self { led_pin, last_controller: Controller::SINGLE_NORMAL, last_toggle: 0, toggles: 0 }    
    }
}

impl LedHandler for SingleLedHandler {
    fn set_state(&mut self, state: bool) {
        // set led state
    }

    fn blink(&mut self, controller: Controller) {
        // let now = current time; 
        // let time_since_last_toggle = now - self.last_toggle;
        
        if controller != self.last_controller {
            // self.set_state(false)
            // self.last_toggle = now;
            // self.toggles = 0;
        }

        match &controller {
            Single {ref time_btwn_blinks} => {
                if time_since_last_toggle > time_btwn_blinks { 
                    // self.set_state(self.toggles % 2 != 0); 
                    // self.last_toggle = now; 
                    // self.toggles = (self.toggles + 1) % 2;
                } 
            },
            Burst {ref time_btwn_blinks, ref blinks_per_burst, ref time_btwn_bursts} => {
                if toggles < blinks_per_burst*2 {
                    if time_since_last_toggle > time_btwn_blinks {
                        // self.set_state(self.toggles % 2 != 0); 
                        // self.last_toggle = now;
                        // self.toggles += 1;
                    }
                }
                else {
                    // led should be off by the time the code reaches this point so all we do is
                    // wait time_btwn_bursts secs then reset the appropriate variables so that on
                    // the next call the led will toggle on after time_btwn_blinks secs have passed 
                    if time_since_last_toggle > time_btwn_bursts {
                        // self.toggles = 1;
                        // self.last_toggle = now;
                    }
                }
            },
        }
        self.last_controller = controller;
    }
}

#[derive(Debug)]
pub struct DoubleLedHandler {
    led1_handler: SingleLedHandler,
    led2_handler: SingleLedHandler,
}

impl DoubleLedHandler {
    fn new(led1_pin: u8, led2_pin: u8) -> Self {
        Self { 
            led1_handler: SingleLedHandler::new(led_pin: led1_pin),
            led2_handler: SingleLedHandler::new(led_pin: led2_pin), 
        }    
    }
}

impl LedHandler for DoubleLedHandler {
    fn set_state(&mut self, state: bool) {
        self.led1_handler.set_state(state);
        self.led2_handler.set_state(state);
    }
    
    fn blink(&mut self, controller: Controller) {
        self.led1_handler.blink(controller);
        self.led2_handler.blink(controller);
    }
}

#[derive(Debug)]
pub struct DebugLedHandler;

impl DebugLedHandler {
    fn new() {
        todo!()
    }
}

impl LedHandler for DebugLedHandler {
    fn set_state(&mut self, state: bool) { 
        println!("set_state called on {state}");
    }

    fn blink(&mut self, controller: Controller) {
        println!("blink called on {controller}"); 
    }
}
