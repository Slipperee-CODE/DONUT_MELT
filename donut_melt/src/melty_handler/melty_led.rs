use std::fmt;

#[derive(PartialEq, Clone, Debug)]
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
    const SLOW_BLINK: f32 = 1.0; // in secs
    const NORMAL_BLINK: f32 = 0.5; // in secs
    const FAST_BLINK: f32 = 0.1; // in secs
    const SINGLE_SLOW: Controller = Controller::Single { time_btwn_blinks: Self::SLOW_BLINK };
    const SINGLE_NORMAL: Controller = Controller::Single { time_btwn_blinks: Self::NORMAL_BLINK };
    const SINGLE_FAST: Controller = Controller::Single { time_btwn_blinks: Self::FAST_BLINK };
    const BURST_2: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 2, time_btwn_bursts: Self::SLOW_BLINK};
    const BURST_3: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 3, time_btwn_bursts: Self::SLOW_BLINK};
    const BURST_4: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 4, time_btwn_bursts: Self::SLOW_BLINK};
}

pub trait LedHandler: fmt::Debug {
    fn setup(&self);
    fn set_state(&mut self, state: bool);
    fn blink(&mut self, controller: Controller);
}

#[derive(Debug)]
pub struct SingleLedHandler {
    led_pin: u8,
    last_controller: Controller,
    last_toggle: f32,
    toggles: u32,
}

impl SingleLedHandler {
    pub const fn new(led_pin: u8) -> Self {
        Self { led_pin, last_controller: Controller::SINGLE_NORMAL, last_toggle: 0.0, toggles: 0 }    
    }
}

impl LedHandler for SingleLedHandler {
    fn setup(&self) {

    }

    fn set_state(&mut self, state: bool) {
        // set led state
    }

    fn blink(&mut self, controller: Controller) {
        // let now = current time; 
        // let time_since_last_toggle = now - self.last_toggle;
        
        // TODO: Implement the appropriate equality trait between led controllers
        if controller != self.last_controller {
            // self.set_state(false)
            // self.last_toggle = now;
            // self.toggles = 0;
        }

        match &controller {
            Controller::Single {ref time_btwn_blinks} => {
                // TODO: this should fix itself when you uncomment time_since_last_toggle above
                if time_since_last_toggle > time_btwn_blinks { 
                    // self.set_state(self.toggles % 2 != 0); 
                    // self.last_toggle = now; 
                    // self.toggles = (self.toggles + 1) % 2;
                } 
            },
            Controller::Burst {ref time_btwn_blinks, ref blinks_per_burst, ref time_btwn_bursts} => {
                if self.toggles < blinks_per_burst*2 {
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
    pub const fn new(led1_pin: u8, led2_pin: u8) -> Self {
        Self { 
            led1_handler: SingleLedHandler::new(led1_pin),
            led2_handler: SingleLedHandler::new(led2_pin), 
        }    
    }
}

impl LedHandler for DoubleLedHandler {
    fn setup(&self) {
        self.led1_handler.setup();
        self.led2_handler.setup();
    }

    fn set_state(&mut self, state: bool) {
        self.led1_handler.set_state(state);
        self.led2_handler.set_state(state);
    }
    
    fn blink(&mut self, controller: Controller) {
        self.led1_handler.blink(controller.clone());
        self.led2_handler.blink(controller);
    }
}

#[derive(Debug)]
pub struct DebugLedHandler;

impl DebugLedHandler {
    pub const fn new() -> Self {
        todo!()
    }
}

impl LedHandler for DebugLedHandler {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn set_state(&mut self, state: bool) { 
        println!("set_state called on {state}");
    }

    fn blink(&mut self, controller: Controller) {
        println!("blink called on {:#?}", controller); 
    }
}
