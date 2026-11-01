mod melty_control;
mod melty_led;
mod melty_receiver;
mod melty_accel;
mod melty_drive;
mod melty_motor;

use melty_control::{ Mode, Controller };

use melty_led::{ LedHandler, Controller as LedController, DoubleLedHandler, DebugLedHandler };

use melty_receiver::{ Channel, TelemetryPacket, SwitchState, Receiver, UartReceiver, DebugReceiver, ReceiverHandler, MeltyReceiverHandler };

use melty_accel::{ Accel, MeltyAccel, AccelHandler, DebugAccelHandler, AntAccelHandler, BeetleAccelHandler };

use melty_motor::{ PIO, DShot, Motor, DShotMotor, DebugMotor };

use melty_drive::{DriveHandler, MeltyDriveHandler};

use std::mem;

trait Animatable<A, L, R, M> {
    fn gather_animations(&mut self) -> Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>>;
}

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
        min_translation_rpm: 100.0,
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
        min_translation_rpm: 100.0,
        aggression: 0.5,
        melty_max_throttle: 0.75,
        tank_slowdown: 0.05,
        tank_turning_slowdown: 0.025,
    };
}

// #[derive(Debug)]
struct MeltyState<A, L, R, M> {
    is_failsafed: bool,
    require_0_throttle: bool,
    rotation_start: f32,
    peak_rpm: u32,
    curr_controls: Option<Controller>,
    // TODO: curr_animations can't be printed because it's a list of functions, need to impl Debug manually for MeltyState 
    curr_animations: Vec<Box<dyn FnMut(&mut MeltyHandler<A, L, R, M>) -> bool>>,
}

impl<A, L, R, M> MeltyState<A, L, R, M> {
    const DEFAULT: Self = Self {
        is_failsafed: true,
        require_0_throttle: true,
        rotation_start: 0.0,
        peak_rpm: 0,
        curr_controls: None,
        curr_animations: vec![],
    };
}

// #[derive(Debug)]
struct MeltyHandler<A, L, R, M> {
    accel_handler: A, 
    heading_led_handler: L,
    receiver_handler: R,
    drive_handler: M,
    melty_settings: MeltySettings,
    melty_state: MeltyState<A, L, R, M>,
}

impl MeltyHandler<AntAccelHandler<MeltyAccel>, DoubleLedHandler, MeltyReceiverHandler<UartReceiver>, MeltyDriveHandler<DShotMotor>> {
    const ANT: Self = Self {
        accel_handler: AntAccelHandler::new(MeltyAccel::new(), 0.0, 0.0, 0.0, 0.0, 0.0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        drive_handler: MeltyDriveHandler::new(DShotMotor::new(0, DShot::DShot600, PIO), DShotMotor::new(0, DShot::DShot600, PIO)),
        melty_settings: MeltySettings::ANT,
        melty_state: MeltyState::DEFAULT,
    };
}

impl MeltyHandler<BeetleAccelHandler<MeltyAccel>, DoubleLedHandler, MeltyReceiverHandler<UartReceiver>, MeltyDriveHandler<DShotMotor>> {
    const BEETLE: Self = Self {
        accel_handler: BeetleAccelHandler::new(MeltyAccel::new(), MeltyAccel::new(), (0.0, 0.0), (0.0, 0.0), (0.0, 0.0), (0.0, 0.0), 0.0, 0.0, 0.0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        drive_handler: MeltyDriveHandler::new(DShotMotor::new(0, DShot::DShot600, PIO), DShotMotor::new(0, DShot::DShot600, PIO)),
        melty_settings: MeltySettings::BEETLE,
        melty_state: MeltyState::DEFAULT,
    };
}

impl MeltyHandler<DebugAccelHandler, DebugLedHandler, MeltyReceiverHandler<DebugReceiver>, MeltyDriveHandler<DebugMotor>> {
    const DEBUG: Self = Self {
        accel_handler: DebugAccelHandler::new(200.0),
        heading_led_handler: DebugLedHandler::new(),
        receiver_handler: MeltyReceiverHandler::new(DebugReceiver::new()),
        drive_handler: MeltyDriveHandler::new(DebugMotor::new(DShot::DShot600), DebugMotor::new(DShot::DShot600)),
        melty_settings: MeltySettings::ANT,
        melty_state: MeltyState::DEFAULT,
    };
}

// I don't think this is supposed to work, how can a type be generically over itself???
impl<A, L, R, M> MeltyHandler<A, L, R, M> 
where 
    A: AccelHandler + Animatable<A, L, R, M>, 
    L: LedHandler + Animatable<A, L, R, M>, 
    R: ReceiverHandler + Animatable<A, L, R, M>,
    M: DriveHandler + Animatable<A, L, R, M>,
{
    fn setup(&mut self) {
        self.accel_handler.setup();
        self.heading_led_handler.setup();
        self.receiver_handler.setup();
        self.drive_handler.setup();
    }

    fn always(&self) {
        // feed watchdog
    }

    fn when_failsafe_off(&mut self) {
        self.melty_state.curr_animations.extend(self.accel_handler.gather_animations());
        self.melty_state.curr_animations.extend(self.heading_led_handler.gather_animations());
        self.melty_state.curr_animations.extend(self.receiver_handler.gather_animations());
        self.melty_state.curr_animations.extend(self.drive_handler.gather_animations());

        self.melty_state.curr_controls = Some(self.receiver_handler.get_controls());

        let curr_animations = mem::take(&mut self.melty_state.curr_animations);

        for mut animation in curr_animations {
            if animation(self) {
                self.melty_state.curr_animations.push(animation)
            }
        }
    }

    fn when_failsafe_on(&mut self) {
        self.drive_handler.stop_all();

        if !self.melty_state.is_failsafed && self.receiver_handler.is_throttle_0() {
            self.melty_state.require_0_throttle = false;
        }

        self.heading_led_handler.blink(LedController::SINGLE_NORMAL);
    }

    fn when_flashing_motors(&mut self) {
        self.heading_led_handler.blink(LedController::BURST_2);
    }
    
    // pub fn debug(&self) {
    //     println!("{:#?}", self);
    // }

    pub fn handle(&mut self, is_flashing_motors: bool) {
        if is_flashing_motors {
            loop {
                self.when_flashing_motors();
            }
        }

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

