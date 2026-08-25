use super::melty_control::{Mode, Controller, Frame, Animation};

use super::melty_receiver::ReceiverHandler;
use super::melty_led::LedHandler;
use super::melty_accel::AccelHandler;

use super::melty_motor::Motor;

use super::MeltyHandler;

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> MeltyHandler<A, L, R, M> {
    async fn drive(&mut self, mut controls: Animation) {
        loop {
            let Animation { curr: curr_frame, remaining: next_animation } = controls;
            
            match next_animation {
                Some(animation_box) => {
                    controls = *animation_box;
                }
                None => {
                    break;
                }
            }

            match curr_frame {
                Frame { controller: Controller { mode: Mode::MELTY, left_x, left_y, right_x, right_y }, duration } => {
                    self.melty(left_x, left_y, right_x, right_y);
                    // wait duration seconds
                },
                Frame { controller: Controller { mode: Mode::TANK, left_x, left_y, right_x, right_y }, duration } => {
                    self.one_stick_tank(right_x*self.melty_settings.tank_slowdown, right_y*self.melty_settings.tank_turning_slowdown);
                    // wait duration seconds
                },
                Frame { controller: Controller { mode: Mode::FAST_TANK, left_x, left_y, right_x, right_y }, duration } => {
                    self.tank(left_y, right_y);
                    // wait duration seconds
                },
            }
        }
    }

    fn melty(&self, left_x: f32, left_y: f32, right_x: f32, right_y: f32) {
         
    }

    fn one_stick_tank(&mut self, right_x: f32, right_y: f32) {
        if right_x == 0.0 {
            self.tank(right_x, right_x);
            return;
        }
        // this negative sign might need to be moved to the second "right_y"
        self.tank(-right_y, right_y);
    }

    fn tank(&mut self, left_y: f32, right_y: f32) {
        self.motor1.set_throttle(left_y);
        self.motor2.set_throttle(right_y);
    }
}
