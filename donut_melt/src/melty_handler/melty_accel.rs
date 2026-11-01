mod accels;
pub use accels::{MeltyAccel, DebugAccel};

mod accel_handlers;
pub use accel_handlers::{AntAccelHandler, BeetleAccelHandler, DebugAccelHandler};

use std::fmt;

pub trait Accel: fmt::Debug {
    fn setup(&self);
    fn get_all_gs(&self) -> (f32, f32, f32);

    fn get_x_gs(&self) -> f32 {
        self.get_all_gs().0
    }

    fn get_y_gs(&self) -> f32 {
        self.get_all_gs().1
    }

    fn get_z_gs(&self) -> f32 {
        self.get_all_gs().2
    }
}

pub trait AccelHandler: fmt::Debug {
    const EPSILON: f32;

    fn setup(&self);
    fn shift_rpm_multiplier(&mut self, shift_reference_value: f32);
    fn get_adj_rpm(&self, adjustment: f32, heading_sensitivity: f32) -> f32;
    fn get_raw_rpm(&self) -> f32;
}
