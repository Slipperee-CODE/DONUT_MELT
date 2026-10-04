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

impl Controller {
    pub const DEFAULT: Self = Self {
        mode: Mode::TANK,
        direction: Direction::NORMAL,
        left_x: 0.5,
        left_y: 0.5,
        right_x: 0.5,
        right_y: 0.5,
    };
}

#[derive(Debug)]
pub struct Frame {
   pub controller: Controller,
   pub duration: u32,
}

impl Frame {
    const DEFAULT: Self = Self {
        controller: Controller::DEFAULT,
        duration: 0,
    };
}


#[derive(Debug)]
pub struct Animation {
    pub curr: Frame, 
    pub remaining: Option<Box<Animation>>,
}

impl Animation {
    pub const DEFAULT: Self = Self {
        curr: Frame::DEFAULT,
        remaining: None,
    };

    pub fn new(curr: Frame, remaining: Option<Box<Animation>>) -> Self {
        Self { 
            curr,
            remaining,
        }
    }

    pub fn from_vec(v: Vec<Frame>) -> Option<Box<Animation>> {
        Self::from_iter(v.into_iter())
    }

    pub fn from_iter(mut i: impl Iterator<Item = Frame>) -> Option<Box<Animation>> {
        match i.next() {
            Some(f) => {
                Some(Box::new(Animation::new(f, Self::from_iter(i))))
            }
            None => None
        }
    }
}
