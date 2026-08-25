mod melty_control;
mod melty_led;
mod melty_receiver;
mod melty_accel;
mod melty_drive;
mod melty_motor;

use melty_control::{ Mode, Controller, Frame, Animation};

use melty_led::{ LedHandler, DoubleLedHandler };

use melty_receiver::{ Channel, TelemetryPacket, SwitchState, Receiver, MeltyReceiver, ReceiverHandler, MeltyReceiverHandler };

use melty_accel::{ Accel, MeltyAccel, AccelHandler, FakeAccelHandler, AntAccelHandler, BeetleAccelHandler };

use melty_motor::{ PIO, DShot, Motor, MeltyMotor };

#[derive(Debug)]
struct MeltySettings {
    heading_sensitivity: f32,
    heading_led_offset: f32,
    min_led_duration: f32,
    max_led_duration: f32,
    min_translation_rpm: f32,
    aggression: f32,
    melty_max_throttle: f32,
    tank_slowdown: f32,
    tank_turning_slowdown: f32,
}

#[derive(Debug)]
struct MeltyState {
    is_failsafed: bool,
    require_0_throttle: bool,
    rotation_start: f32,
    peak_rpm: u32,
    last_controller: Controller,
    last_animation: Animation,
}

#[derive(Debug)]
struct MeltyHandler<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> {
    accel_handler: A, 
    heading_led_handler: L,
    receiver_handler: R,
    motor1: M,
    motor2: M,
    melty_settings: MeltySettings,
    melty_state: MeltyState,
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> MeltyHandler<A, L, R, M> {
    fn always(&self) {

    }

    fn when_failsafe_off(&self) {

    }

    fn when_failsafe_on(&self) {

    }

    fn when_flashing_motors(&self) {

    }
    
    fn debug(&self) {
        println!("{:#?}", self);
    }
     
    fn handle(&self) {
        loop {
            if self.melty_state.is_failsafed || self.melty_state.require_0_throttle || self.receiver_handler.is_kill_switch_on() {
                self.when_failsafe_on();
            } else {
                self.when_failsafe_off();
            }

            self.always();
        }
    }
}

