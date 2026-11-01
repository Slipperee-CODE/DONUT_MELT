use std::mem;

pub fn test2() {

}

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

pub struct Controller {
    pub mode: Mode,
    pub direction: Direction,
    pub left_x: f32,
    pub left_y: f32,
    pub right_x: f32,
    pub right_y: f32,
}

struct Settings;

struct State {
    curr_controls: Controller,
    animations_to_call: Vec<Box<dyn FnMut(Handler) -> (bool, Handler)>>,
}

struct Handler {
    melty_settings: Settings,
    melty_state: State,
}

impl Handler {
    fn when_failsafe_off(mut self) {
        // self.melty_state.animations_to_call.extend(accel_handler.gather_animations())
        // self.melty_state.animations_to_call.extend(heading_led_handler.gather_animations())
        // self.melty_state.animations_to_call.extend(receiver_handler.queue_animations()) // receiver should queue drive animationtion, drive animationtion should always disappear after 1 use

        // (in always()) self.curr_controls = self.receiver.get_controls();

        let curr_animations = mem::take(&mut self.melty_state.animations_to_call);

        for mut animation in curr_animations {
            let (keep_animation, new_handler) = animation(self);
            
            self = new_handler;

            if keep_animation {
                self.melty_state.animations_to_call.push(animation)
            }
        }
    }
}