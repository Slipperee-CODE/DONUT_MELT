#[derive(Debug)]
pub enum Mode {
    MELTY,
    TANK,
    FAST_TANK,
}

#[derive(Debug)]
pub enum Direction {
    NORMAL,
    REVERSE,
}

#[derive(Debug)]
pub struct Controller {
    pub mode: Mode,
    pub direction: Direction,
    pub left_x: f32,
    pub left_y: f32,
    pub right_x: f32,
    pub right_y: f32,
}

#[derive(Debug)]
pub struct Frame(pub fn() -> (), pub f32);

pub type Animation = Box<dyn Iterator<Item = Frame>>;
