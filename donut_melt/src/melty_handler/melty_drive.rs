mod drive_handlers;

use super::melty_control::{ Mode, Direction, Controller};
use super::Animatable;
use super::melty_receiver::ReceiverHandler;
use super::melty_led::{LedHandler, Controller as LedController};
use super::melty_accel::AccelHandler;
pub use drive_handlers::MeltyDriveHandler;

use super::melty_motor::Motor;
use std::fmt;

use super::MeltyHandler;

pub trait DriveHandler: fmt::Debug{
    fn setup(&self);

    fn melty(&self, left_x: f32, left_y: f32, right_x: f32, right_y: f32);

    fn tank(&mut self, left_y: f32, right_y: f32);

    fn stop_all(&mut self) {
        self.tank(0.0, 0.0);
    }

    fn one_stick_tank(&mut self, right_x: f32, right_y: f32) {
        if right_x == 0.0 {
            self.tank(right_x, right_x);
            return;
        }
        // this negative sign might need to be moved to the second "right_y"
        self.tank(-right_y, right_y);
    }
}
