mod melty_control;
mod melty_led;
mod melty_receiver;
mod melty_accel;
mod melty_drive;
mod melty_motor;

use melty_control::{ Mode, Controller, Frame, Animation };

use melty_led::{ LedHandler, Controller as LedController, DoubleLedHandler, DebugLedHandler };

use melty_receiver::{ Channel, TelemetryPacket, SwitchState, Receiver, UartReceiver, DebugReceiver, ReceiverHandler, MeltyReceiverHandler };

use melty_accel::{ Accel, MeltyAccel, AccelHandler, DebugAccelHandler, AntAccelHandler, BeetleAccelHandler };

use melty_motor::{ PIO, DShot, Motor, DShotMotor, DebugMotor };

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

#[derive(Debug)]
struct MeltyState {
    is_failsafed: bool,
    require_0_throttle: bool,
    rotation_start: f32,
    peak_rpm: u32,
    frame_start: f32,
    curr_controller: Option<Controller>, // tracked for debugging purposes
    curr_animation: Option<Animation>,
}

impl MeltyState {
    const DEFAULT: Self = Self {
        is_failsafed: true,
        require_0_throttle: true,
        rotation_start: 0.0,
        peak_rpm: 0,
        frame_start: 0.0,
        curr_controller: None, 
        curr_animation: None,
    };
}

#[derive(Debug)]
struct MeltyHandler<A, L, R, M> {
    accel_handler: A, 
    heading_led_handler: L,
    receiver_handler: R,
    motor1: M,
    motor2: M,
    melty_settings: MeltySettings,
    melty_state: MeltyState,
}

impl MeltyHandler<AntAccelHandler<MeltyAccel>, DoubleLedHandler, MeltyReceiverHandler<UartReceiver>, DShotMotor> {
    const ANT: Self = Self {
        accel_handler: AntAccelHandler::new(MeltyAccel::new(), 0.0, 0.0, 0.0, 0.0, 0.0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        motor1: DShotMotor::new(0, DShot::DShot600, PIO),
        motor2: DShotMotor::new(0, DShot::DShot600, PIO),
        melty_settings: MeltySettings::ANT,
        melty_state: MeltyState::DEFAULT,
    };
}

impl MeltyHandler<BeetleAccelHandler<MeltyAccel>, DoubleLedHandler, MeltyReceiverHandler<UartReceiver>, DShotMotor> {
    const BEETLE: Self = Self {
        accel_handler: BeetleAccelHandler::new(MeltyAccel::new(), MeltyAccel::new(), (0.0, 0.0), (0.0, 0.0), (0.0, 0.0), (0.0, 0.0), 0.0, 0.0, 0.0),
        heading_led_handler: DoubleLedHandler::new(0, 0),
        receiver_handler: MeltyReceiverHandler::new(UartReceiver::new()),
        motor1: DShotMotor::new(0, DShot::DShot600, PIO),
        motor2: DShotMotor::new(0, DShot::DShot600, PIO),
        melty_settings: MeltySettings::BEETLE,
        melty_state: MeltyState::DEFAULT,
    };
}


impl MeltyHandler<DebugAccelHandler, DebugLedHandler, MeltyReceiverHandler<DebugReceiver>, DebugMotor> {
    const DEBUG: Self = Self {
        accel_handler: DebugAccelHandler::new(200.0),
        heading_led_handler: DebugLedHandler::new(),
        receiver_handler: MeltyReceiverHandler::new(DebugReceiver::new()),
        motor1: DebugMotor::new(DShot::DShot600),
        motor2: DebugMotor::new(DShot::DShot600),
        melty_settings: MeltySettings::ANT,
        melty_state: MeltyState::DEFAULT,
    };
}

impl<A: AccelHandler, L: LedHandler, R: ReceiverHandler, M: Motor> MeltyHandler<A, L, R, M> {
    fn setup(&mut self) {
        self.accel_handler.setup();
        self.heading_led_handler.setup();
        self.receiver_handler.setup();
        self.motor1.setup();
        self.motor2.setup();
    }

    fn always(&self) {
        // feed watchdog
    }

    fn when_failsafe_off(&mut self) {
        // TODO: check if curr_animation is None, if so fill it with get_controls()
        // - maybe use a custom ReceiverHandler/Receiver + AccelHandler/Accel 
        //   to do the data logging for accel gs at diff rpm values 
        // otherwise let the animation play out until it is None
        // if we fill curr_animation, set self.frame_start = curr_time otherwise don't touch it
        // get_controls (so the ReceiverHandler should handle replacing normal controls with a macro animation)
        // give this self.drive call a reference to the first frame of curr_animation so it knows what to do
        
        // TODO: set curr_controller to be the controller of the frame you pass to drive
        self.drive();

        // TODO: if self.frame_start - now > duration of frame move to next frame of animation (change curr_animation)
    }

    fn when_failsafe_on(&mut self) {
        self.motor1.stop();
        self.motor2.stop();

        if !self.melty_state.is_failsafed && self.receiver_handler.is_throttle_0() {
            self.melty_state.require_0_throttle = false;
        }

        self.heading_led_handler.blink(LedController::SINGLE_NORMAL);
    }

    fn when_flashing_motors(&mut self) {
        self.heading_led_handler.blink(LedController::BURST_2);
    }
    
    pub fn debug(&self) {
        println!("{:#?}", self);
    }

    pub fn handle(&mut self, is_flashing_motors: bool) {
        // TODO: make this function call start 2 separate threads for receiver receiving and rest of code

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

