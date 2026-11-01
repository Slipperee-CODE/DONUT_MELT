use super::super::{MeltyHandler, Animatable, AccelHandler, LedHandler, ReceiverHandler, Motor};

use super::Accel;

#[derive(Debug)]
pub struct AntAccelHandler<A> {
    accel: A,
    accel_0_offset: f32, // in gs
    radius: f32, // in cm
    rpm_multiplier: f32,
    upper_rpm_mult_bound: f32,
    lower_rpm_mult_bound: f32,
}

impl<A: Accel> AntAccelHandler<A> {
    pub const fn new(accel: A, accel_0_offset: f32, radius: f32, rpm_multiplier: f32, upper_rpm_mult_bound: f32, lower_rpm_mult_bound: f32) -> Self {
        Self {accel, accel_0_offset, radius, rpm_multiplier, upper_rpm_mult_bound, lower_rpm_mult_bound }
    }
}

impl<A: Accel> AccelHandler for AntAccelHandler<A> { 
    const EPSILON: f32 = 0.1;

    fn setup(&self) {
        // do any necessary setup here
        self.accel.setup();
    }

    fn shift_rpm_multiplier(&mut self, shift_reference_value: f32) {
        self.rpm_multiplier = (self.rpm_multiplier + shift_reference_value.signum()*Self::EPSILON).clamp(self.lower_rpm_mult_bound, self.upper_rpm_mult_bound);
    }

    // assumes adjustment is 0.5 by default
    fn get_adj_rpm(&self, adjustment: f32, heading_sensitivity: f32) -> f32 {
        self.get_raw_rpm() * (adjustment + 0.5) * heading_sensitivity
    }

    fn get_raw_rpm(&self) -> f32 {
        let mut rpm: f32 = (self.accel.get_x_gs() - self.accel_0_offset).abs() * 89445.0;
        rpm = rpm / self.radius;
        rpm = rpm.sqrt();
        rpm * self.rpm_multiplier
    }
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> Animatable<A, L, R, M> for AntAccelHandler<R> {
    fn gather_animations(&mut self) -> Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>> {
        vec![]
    }
}

#[derive(Debug)]
pub struct BeetleAccelHandler<A> {
    accel1: A,
    accel2: A,
    accel1_0_offset: (f32, f32),
    accel2_0_offset: (f32, f32),
    accel1_pos: (f32, f32),
    accel2_pos: (f32, f32),
    rpm_multiplier: f32,
    upper_rpm_mult_bound: f32,
    lower_rpm_mult_bound: f32,
}

impl<A: Accel> BeetleAccelHandler<A> {
    pub const fn new(accel1: A, accel2: A, accel1_0_offset: (f32, f32), accel2_0_offset: (f32, f32), accel1_pos: (f32, f32), accel2_pos: (f32, f32), rpm_multiplier: f32, upper_rpm_mult_bound: f32, lower_rpm_mult_bound: f32) -> Self {
       Self {accel1, accel2, accel1_0_offset, accel2_0_offset, accel1_pos, accel2_pos, rpm_multiplier, upper_rpm_mult_bound, lower_rpm_mult_bound }
    }
}

impl<A: Accel> AccelHandler for BeetleAccelHandler<A> { 
    const EPSILON: f32 = 0.1;

    fn setup(&self) {
        // do any necessary setup here
        self.accel1.setup();
        self.accel2.setup();
    }

    fn shift_rpm_multiplier(&mut self, shift_reference_value: f32) {
        self.rpm_multiplier = (self.rpm_multiplier + shift_reference_value.signum()*Self::EPSILON).clamp(self.lower_rpm_mult_bound, self.upper_rpm_mult_bound);
    }

    // assumes adjustment is 0.5 by default
    fn get_adj_rpm(&self, adjustment: f32, heading_sensitivity: f32) -> f32 {
        self.get_raw_rpm() * (adjustment + 0.5) * heading_sensitivity
    }

    fn get_raw_rpm(&self) -> f32 {
        let accel1_gs = (
            self.accel1.get_x_gs() - self.accel1_0_offset.0, 
            self.accel1.get_y_gs() - self.accel1_0_offset.1
        );

        let accel2_gs = (
            self.accel2.get_x_gs() - self.accel2_0_offset.0, 
            self.accel2.get_y_gs() - self.accel2_0_offset.1
        );

        // omega = sqrt [ ||(a1 - a2)|| / ||(r2 - r1)|| ]
        let delta_a: (f32, f32) = (
            accel1_gs.0 - accel2_gs.0,
            accel1_gs.1 - accel2_gs.1
        );

        let delta_pos: (f32, f32) = (
            self.accel1_pos.0 - self.accel2_pos.0,
            self.accel1_pos.1 - self.accel2_pos.1
        );

        let mag_delta_a: f32 = (delta_a.0 * delta_a.0 + delta_a.1 * delta_a.1).sqrt();
        let mag_delta_pos: f32 = (delta_pos.0 * delta_pos.0 + delta_pos.1 * delta_pos.1).sqrt();
        
        let rpm = (mag_delta_a / mag_delta_pos) * 89445.0;

        rpm * self.rpm_multiplier
    }
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> Animatable<A, L, R, M> for BeetleAccelHandler<R> {
    fn gather_animations(&mut self) -> Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>> {
        vec![]
    }
}

#[derive(Debug)]
pub struct DebugAccelHandler {
    rpm: f32,
}

impl DebugAccelHandler {
    pub const fn new(rpm: f32) -> Self {
        Self { rpm }
    }
}

impl AccelHandler for DebugAccelHandler { 
    const EPSILON: f32 = 0.1;

    fn setup(&self) {
        // do any necessary setup here
    }

    fn shift_rpm_multiplier(&mut self, shift_reference_value: f32) {
        // do nothing
    }

    // assumes adjustment is 0.5 by default
    fn get_adj_rpm(&self, adjustment: f32, heading_sensitivity: f32) -> f32 {
        self.get_raw_rpm() * (adjustment + 0.5) * heading_sensitivity
    }

    fn get_raw_rpm(&self) -> f32 {
        self.rpm
    }
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> Animatable<A, L, R, M> for DebugAccelHandler {
    fn gather_animations(&mut self) -> Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>> {
        vec![]
    }
}
