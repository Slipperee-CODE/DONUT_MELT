#[derive(Debug)]
pub enum Mode {
    MELTY,
    TANK,
    FAST_TANK,
}

#[derive(Debug)]
pub struct Controller {
    pub mode: Mode,
    pub left_x: f32,
    pub left_y: f32,
    pub right_x: f32,
    pub right_y: f32,
}

#[derive(Debug)]
pub struct Frame {
   pub controller: Controller,
   pub duration: u32,
}

#[derive(Debug)]
pub struct Animation {
    pub curr: Frame, 
    pub remaining: Option<Box<Animation>>,
}
