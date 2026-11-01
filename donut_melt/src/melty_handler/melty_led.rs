mod led_handlers;
pub use led_handlers::{SingleLedHandler, DoubleLedHandler, DebugLedHandler};

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
    pub const SLOW_BLINK: f32 = 1.0; // in secs
    pub const NORMAL_BLINK: f32 = 0.5; // in secs
    pub const FAST_BLINK: f32 = 0.1; // in secs
    pub const SINGLE_SLOW: Controller = Controller::Single { time_btwn_blinks: Self::SLOW_BLINK };
    pub const SINGLE_NORMAL: Controller = Controller::Single { time_btwn_blinks: Self::NORMAL_BLINK };
    pub const SINGLE_FAST: Controller = Controller::Single { time_btwn_blinks: Self::FAST_BLINK };
    pub const BURST_2: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 2, time_btwn_bursts: Self::SLOW_BLINK};
    pub const BURST_3: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 3, time_btwn_bursts: Self::SLOW_BLINK};
    pub const BURST_4: Controller = Controller::Burst { time_btwn_blinks: Self::NORMAL_BLINK, blinks_per_burst: 4, time_btwn_bursts: Self::SLOW_BLINK};
}

pub trait LedHandler: fmt::Debug {
    fn setup(&self);
    fn set_state(&mut self, state: bool);
    fn blink(&mut self, controller: Controller);
}
