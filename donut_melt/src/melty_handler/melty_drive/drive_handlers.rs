use super::super::melty_control::{ Mode, Direction, Controller};
use super::Animatable;
use super::super::melty_receiver::ReceiverHandler;
use super::super::melty_led::{LedHandler, Controller as LedController};
use super::super::melty_accel::AccelHandler;
use super::super::melty_motor::Motor;
use super::MeltyHandler;
use crate::melty_handler::melty_drive::DriveHandler;

#[derive(Debug)]
pub struct MeltyDriveHandler<M> {
    pub motor1: M,
    pub motor2: M,
}

impl<M: Motor> MeltyDriveHandler<M> {
    pub const fn new(motor1: M, motor2: M) -> Self { 
        Self { 
            motor1,
            motor2,
        }
    }
}

impl<M: Motor> DriveHandler for MeltyDriveHandler<M> {
    fn setup(&self) {
        self.motor1.setup();
        self.motor2.setup();
    }
    
    fn melty(&self, left_x: f32, left_y: f32, right_x: f32, right_y: f32) {

    }

    fn tank(&mut self, left_y: f32, right_y: f32) {
        self.motor1.set_throttle(left_y);
        self.motor2.set_throttle(right_y);
    }
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> Animatable<A, L, R, M> for MeltyDriveHandler<M> {
    fn gather_animations(&mut self) -> Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>> {
        let drive = |melty_handler: &mut MeltyHandler<A, L, R, M>| {
            match melty_handler.melty_state.curr_controls.take() {
                Some(Controller { mode: Mode::MELTY, direction, left_x, left_y, right_x, right_y }) => {
                    melty_handler.accel_handler.shift_rpm_multiplier(right_x);

                    let mut new_left_y = left_y;
                    if melty_handler.accel_handler.get_raw_rpm() < melty_handler.melty_settings.min_translation_rpm {
                        new_left_y = 1.0;
                    } 

                    // TODO: need to handle melty heading led here

                    match direction {
                        Direction::NORMAL => {
                            self.melty(left_x, new_left_y, right_x, right_y);
                        },
                        Direction::REVERSE => {
                            self.melty(left_x, -new_left_y, right_x, right_y);
                        },
                    }
                },
                Some(Controller { mode: Mode::TANK, direction, left_x, left_y, right_x, right_y}) => {
                    match direction {
                        Direction::NORMAL => {
                            self.one_stick_tank(right_x*melty_handler.melty_settings.tank_slowdown, right_y*melty_handler.melty_settings.tank_turning_slowdown);
                        },
                        Direction::REVERSE => {
                            self.one_stick_tank(-right_x*melty_handler.melty_settings.tank_slowdown, -right_y*melty_handler.melty_settings.tank_turning_slowdown);
                        },
                    }

                    melty_handler.heading_led_handler.blink(LedController::BURST_3);
                },
                Some(Controller { mode: Mode::FAST_TANK, direction, left_x, left_y, right_x, right_y }) => {
                    match direction {
                        Direction::NORMAL => {
                            self.tank(left_y, right_y);
                        },
                        Direction::REVERSE => {
                            self.tank(-left_y, -right_y);
                        },
                    }

                    melty_handler.heading_led_handler.blink(LedController::BURST_4);
                },
                None => { 
                    // do nothing 
                },
            }
            false
        };
        vec![Box::new(drive)]
    }
}


