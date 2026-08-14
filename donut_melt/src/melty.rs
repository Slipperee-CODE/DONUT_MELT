trait Accel {
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

struct MeltyAccel;

impl Accel for MeltyAccel {
    fn get_all_gs(&self) -> (f32, f32, f32) {
        todo!();
    }
}

trait AccelHandler {
    fn get_adj_rpm(&self, adjustment: f32) -> f32;
    fn get_raw_rpm(&self) -> f32;
}

struct FakeAccelHandler {
    rpm: f32,
}

impl FakeAccelHandler {
    fn new(rpm: f32) -> Self {
        Self { rpm }
    }
}

impl AccelHandler for FakeAccelHandler { 
    fn get_adj_rpm(&self, adjustment: f32) -> f32 {
        (adjustment + 0.5) * self.rpm
    }

    fn get_raw_rpm(&self) -> f32 {
        self.rpm
    }
}

struct AntAccelHandler<A: Accel> {
    accel: A,
    accel_0_offset: f32, // in gs
    radius: f32, // in cm
    radius_offset: f32, // in cm
    heading_sensitivity: f32,
}

impl<A: Accel> AntAccelHandler<A> {
    fn new(accel: A, accel_0_offset: f32, radius: f32, radius_offset: f32, heading_sensitivity: f32) -> Self {
        Self {accel, accel_0_offset, radius, radius_offset, heading_sensitivity }
    }
}

impl<A: Accel> AccelHandler for AntAccelHandler<A> { 
    // assumes adjustment is 0.5 by default
    fn get_adj_rpm(&self, adjustment: f32) -> f32 {
        todo!(); // tune heading_sensitivity so that rpm changes as wanted
        self.get_raw_rpm() * (adjustment + 0.5) * self.heading_sensitivity
    }

    fn get_raw_rpm(&self) -> f32 {
        let mut rpm: f32 = (self.accel.get_x_gs() - self.accel_0_offset).abs() * 89445.0;
        rpm = rpm / (self.radius + self.radius_offset);
        rpm = rpm.sqrt();
        rpm
    }
}

struct BeetleAccelHandler<A: Accel> {
    accel1: A,
    accel2: A,
    accel1_0_offset: (f32, f32),
    accel2_0_offset: (f32, f32),
    accel1_pos: (f32, f32),
    accel2_pos: (f32, f32),
    heading_sensitivity: f32,
    rpm_multiplier: f32,
    upper_rpm_mult_bound: f32,
    lower_rpm_mult_bound: f32,
}

impl<A: Accel> BeetleAccelHandler<A> {
    fn new(accel1: A, accel2: A, accel1_0_offset: f32, accel2_0_offset: f32, accel1_pos: (f32, f32), accel2_pos: (f32, f32), heading_sensitivity: f32, upper_rpm_mult_bound: f32, lower_rpm_mult_bound: f32) -> Self {
       Self {accel1, accel2, accel1_0_offset, accel2_0_offset, accel1_pos, accel2_pos, heading_sensitivity, upper_rpm_mult_bound, lower_rpm_mult_bound }
    }
}

impl<A: Accel> AccelHandler for BeetleAccelHandler<A> { 
    // assumes adjustment is 0.5 by default
    fn get_adj_rpm(&self, adjustment: f32) -> f32 {
        todo!(); // tune heading_sensitivity so that rpm changes as wanted
        self.get_raw_rpm() * (adjustment + 0.5) * self.heading_sensitivity
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

        rpm * self.rpm_multiplier.clamp(self.lower_rpm_mult_bound, self.upper_rpm_mult_bound)
    }
}

trait LedHandler {
    fn set_repeats(&mut self, repeats: u8);

    fn blink(&mut self);
}

struct SingleLedHandler {
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

struct MeltySettings<A: AccelHandler, L: LedHandler> {
    heading_sensitivity: f32,
    accel_handler: A, 
    led_handler: L,
    led_offset: f32,
    min_led_duration: f32,
    max_led_duration: f32,
    min_translation_rpm: f32,
    aggression: f32,
    melty_max_throttle: f32,
    tank_max_throttle: f32,
    tank_max_turning_throttle: f32,
}

struct MeltyState {
    is_failsafed: bool,
    require_zero_throttle: bool,
    rotation_start: bool,
    peak_rpm: u32,
}

enum Mode {
    MELTY,
    TANK,
    FAST_TANK,
}

struct Controller {
    mode: Mode,
    left_x: f32,
    left_y: f32,
    right_x: f32,
    right_y: f32,
}

struct Frame {
   controller: Controller,
   duration: u32,
}

struct Animation {
    curr: Frame, 
    remaining: Option<Box<Animation>>,
}

struct TelemetryPacket(u16, u16, u32, u8);
