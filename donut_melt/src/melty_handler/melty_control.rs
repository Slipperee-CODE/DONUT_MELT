#[derive(Debug)]
pub enum Mode {
    MELTY,
    TANK,
    FAST_TANK,
}

#[derive(Debug)]
pub struct Controller {
    mode: Mode,
    left_x: f32,
    left_y: f32,
    right_x: f32,
    right_y: f32,
}

#[derive(Debug)]
pub struct Frame {
   controller: Controller,
   duration: u32,
}

#[derive(Debug)]
pub struct Animation {
    curr: Frame, 
    remaining: Option<Box<Animation>>,
}
