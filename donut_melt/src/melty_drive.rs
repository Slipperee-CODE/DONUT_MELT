pub enum MeltyController {
   State(i32, i32, i32),
   Animation(Box<MeltyController>, i32, Box<MeltyController>),
}
