use super::melty_control::{ Mode, Direction, Controller, Frame, Animation };

use super::melty_receiver::ReceiverHandler;
use super::melty_led::{LedHandler, Controller as LedController};
use super::melty_accel::AccelHandler;

use super::melty_motor::Motor;

use super::MeltyHandler;

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> MeltyHandler<A, L, R, M> {
    pub fn drive(&mut self, controls: Controller) {
        match controls {
            Controller { mode: Mode::MELTY, direction, left_x, left_y, right_x, right_y } => {
                self.accel_handler.shift_rpm_multiplier(right_x);

                // TODO: left_y needs to be reassigned to 1 so we go full power if our
                // rpm is below min_translation_rpm
                if self.accel_handler.get_raw_rpm() < self.melty_settings.min_translation_rpm {
                    left_y: f32 = 1.0;
                } 

                // TODO: need to handle melty heading led here

                match direction {
                    Direction::NORMAL => {
                        self.melty(left_x, left_y, right_x, right_y);
                    },
                    Direction::REVERSE => {
                        self.melty(left_x, -left_y, right_x, right_y);
                    },
                }
            },
            Controller { mode: Mode::TANK, direction, left_x, left_y, right_x, right_y} => {
                match direction {
                    Direction::NORMAL => {
                        self.one_stick_tank(right_x*self.melty_settings.tank_slowdown, right_y*self.melty_settings.tank_turning_slowdown);
                    },
                    Direction::REVERSE => {
                        self.one_stick_tank(-right_x*self.melty_settings.tank_slowdown, -right_y*self.melty_settings.tank_turning_slowdown);
                    },
                }

                self.heading_led_handler.blink(LedController::BURST_3);
            },
            Controller { mode: Mode::FAST_TANK, direction, left_x, left_y, right_x, right_y } => {
                match direction {
                    Direction::NORMAL => {
                        self.tank(left_y, right_y);
                    },
                    Direction::REVERSE => {
                        self.tank(-left_y, -right_y);
                    },
                }

                self.heading_led_handler.blink(LedController::BURST_4);
            },
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