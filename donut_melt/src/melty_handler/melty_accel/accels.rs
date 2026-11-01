use super::Accel;

#[derive(Debug)]
pub struct MeltyAccel;

impl MeltyAccel {
    pub const fn new() -> Self { 
        todo!();
    }
}

impl Accel for MeltyAccel {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn get_all_gs(&self) -> (f32, f32, f32) {
        todo!();
    }
}

#[derive(Debug)]
pub struct DebugAccel(f32, f32, f32);

impl DebugAccel {
    fn new(x: f32, y: f32, z: f32) -> Self { 
        Self(x, y, z)
    }
}

impl Accel for DebugAccel {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn get_all_gs(&self) -> (f32, f32, f32) {
        (self.0, self.1, self.2) 
    }
}


