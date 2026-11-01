mod motors;
pub use motors::{PIO, DShot, DShotMotor, PwmMotor, DebugMotor};

use std::fmt;

pub trait Motor: fmt::Debug {
    fn setup(&self);
    fn set_throttle(&mut self, throttle: f32); 

    async fn send_throttle(&self);

    fn stop(&mut self) {
        self.set_throttle(0.0);
    }
}
