mod melty_control;
mod melty_led;
mod melty_receiver;
mod melty_accel;

use melty_control::Mode;
use melty_control::Controller;
use melty_control::Frame;
use melty_control::Animation;

use melty_led::LedHandler;
use melty_led::DoubleLedHandler;

use melty_receiver::Channel;
use melty_receiver::TelemetryPacket;
use melty_receiver::SwitchState;
use melty_receiver::Receiver;
use melty_receiver::MeltyReceiver;
use melty_receiver::ReceiverHandler;
use melty_receiver::MeltyReceiverHandler;

use melty_accel::Accel;
use melty_accel::MeltyAccel;
use melty_accel::AccelHandler;
use melty_accel::FakeAccelHandler;
use melty_accel::AntAccelHandler;
use melty_accel::BeetleAccelHandler;

#[derive(Debug)]
struct MeltySettings {
    heading_sensitivity: f32,
    heading_led_offset: f32,
    min_led_duration: f32,
    max_led_duration: f32,
    min_translation_rpm: f32,
    aggression: f32,
    melty_max_throttle: f32,
    tank_max_throttle: f32,
    tank_max_turning_throttle: f32,
}

#[derive(Debug)]
struct MeltyState {
    is_failsafed: bool,
    require_0_throttle: bool,
    rotation_start: bool,
    peak_rpm: u32,
    last_controller: Controller,
    last_animation: Animation,
}

#[derive(Debug)]
struct MeltyHandler<A: AccelHandler, L: LedHandler, R: ReceiverHandler> {
    accel_handler: A, 
    heading_led_handler: L,
    receiver_handler: R,
    melty_settings: MeltySettings,
    melty_state: MeltyState,
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler> MeltyHandler<A, L, R> {
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

