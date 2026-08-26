mod melty_control;
mod melty_led;
mod melty_receiver;
mod melty_accel;
mod melty_drive;
mod melty_motor;

use melty_control::{ Mode, Controller, Frame, Animation };

use melty_led::{ LedHandler, DoubleLedHandler };

use melty_receiver::{ Channel, TelemetryPacket, SwitchState, Receiver, MeltyReceiver, ReceiverHandler, MeltyReceiverHandler };

use melty_accel::{ Accel, MeltyAccel, AccelHandler, DebugAccelHandler, AntAccelHandler, BeetleAccelHandler };

use melty_motor::{ PIO, DShot, Motor, DShotMotor };

#[derive(Debug)]
struct MeltySettings {
    heading_sensitivity: f32, // when MELTYing, determines turning sensitivity
    heading_led_offset: f32, // when MELTYing, offsets the timing of the heading led blink each rotation
    min_led_duration: f32, // when MELTYing, minimum led "on time" as a percent of each rotation
    max_led_duration: f32, // when MELTYing, maximum led "on time" as a percent of each rotation
    min_translation_rpm: f32, // when MELTYing, the bot will use 100% throttle until this rpm is reached
    aggression: f32, // when MELTYing, the maximum percent the motors' throttles can differ each rotation
    melty_max_throttle: f32, // when MELTYing, this caps the maximum throttle AFTER smoothing 
    tank_slowdown: f32, // when in TANK mode (not FAST_TANK), determines how slow forward & backward is
    tank_turning_slowdown: f32, // when in TANK mode (not FAST_TANK), determines how slow turning is
}

impl MeltySettings {
    const ANT: Self = Self {
        heading_sensitivity: 0.5,
        heading_led_offset: 0.2,
        min_led_duration: 0.25,
        max_led_duration: 0.75,
        min_translation_rpm: 100,
        aggression: 0.5,
        melty_max_throttle: 0.75,
        tank_slowdown: 0.05,
        tank_turning_slowdown: 0.025,
    };

    const BEETLE: Self = Self {
        heading_sensitivity: 0.5,
        heading_led_offset: 0.2,
        min_led_duration: 0.25,
        max_led_duration: 0.75,
        min_translation_rpm: 100,
        aggression: 0.5,
        melty_max_throttle: 0.75,
        tank_slowdown: 0.05,
        tank_turning_slowdown: 0.025,
    };
}

#[derive(Debug)]
struct MeltyState {
    is_failsafed: bool,
    require_0_throttle: bool,
    rotation_start: f32,
    peak_rpm: u32,
    last_controller: Controller, // tracked for debugging purposes
    last_animation: Animation, // tracked for debugging purposes
}

impl MeltyState {
    const DEFAULT: Self = Self {
        is_failsafed: true,
        require_0_throttle: true,
        rotation_start: 0,
        peak_rpm: 0,
        last_controller: Controller::DEFAULT, 
        last_animation: Animation::DEFAULT,
    };
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
    const ANT: Self = Self {
        accel_handler: AntAccelHandler::new(MeltyAccel::new(), 0, 0, 0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        motor1: DShotMotor::new(0, DShot::DShot600, PIO),
        motor2: DShotMotor::new(0, DShot::DShot600, PIO),
        MeltySettings::ANT,
        MeltyState::DEFAULT,
    };
    
    const BEETLE: Self = Self {
        accel_handler: AntAccelHandler::new(MeltyAccel::new(), 0, 0, 0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        motor1: DShotMotor::new(0, DShot::DShot600, PIO),
        motor2: DShotMotor::new(0, DShot::DShot600, PIO),
        MeltySettings::BEETLE,
        MeltyState::DEFAULT,
    };

    const DEBUG = Self {
        accel_handler: DebugAccelHandler::new(200),
        heading_led_handler: DebugLedHandler::new(),
        receiver_handler: MeltyReceiverHandler::new(DebugReceiver::new()),
        motor1: DebugMotor::new(),
        motor2: DebugMotor::new(),
        MeltySettings::ANT,
        MeltyState::DEFAULT,
    }
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> MeltyHandler<A, L, R, M> {
    fn setup(&mut self) {

    }

    fn always(&self) {
        // feed watchdog
    }

    fn when_failsafe_off(&self) {

    }

    fn when_failsafe_on(&self) {
        //stop all motors

        if (self.melty_state.is_failsafed == 0 && self.receiver_handler.is_throttle_0() {
            self.melty_state.require_0_throttle = 0;
        }

        //slow blink
    }

    fn when_flashing_motors(&self) {

    }
    
    pub fn debug(&self) {
        println!("{:#?}", self);
    }
     
    pub fn handle(&self) {
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

