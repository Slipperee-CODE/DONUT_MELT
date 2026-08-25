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

impl Animation {
    pub fn new(curr: Frame, remaining: Option<Box<Animation>>) -> Self {
        Self { 
            curr,
            remaining,
        }
    }

    // might able to use some kind of a reduce func to shorten this func in the future
    pub fn from_vec(v: Vec<Frame>) -> Animation {
        let v = v.into_iter();

        let mut a: Box<Animation> = Box::new(Self { 
            v.next(),
            None,
        });
        
        for i in 1..v.len() {
            a.remainining = Some(Box::new(Self { v.next(), None }))
            Some(a) = a.remaining; 
        }
        *a
    }
}
