mod receivers;
pub use receivers::{UartReceiver, PwmReceiver, DebugReceiver};

mod receiver_handlers;
pub use receiver_handlers::{MeltyReceiverHandler};

use std::fmt;

use super::melty_control::{Mode, Direction, Controller};

#[derive(Debug)]
pub enum Channel {
    RIGHT_X,
    RIGHT_Y,
    LEFT_Y,
    LEFT_X,
    SWITCH_E,
    SWITCH_B,
    SWITCH_C,
    SWITCH_F,
    KNOB_S1,
    KNOB_S2,
}

impl Channel {
    const THROTTLE: Channel = Channel::LEFT_Y;
    const KILL_SWITCH: Channel = Channel::SWITCH_E;
    const MODE_SWITCH: Channel = Channel::SWITCH_C;
    const DIRECTION_SWITCH: Channel = Channel::SWITCH_B;
}

#[derive(Debug)]
pub struct TelemetryPacket(u16, u16, u32, u8);

pub trait Receiver: fmt::Debug {
    fn setup(&self);
    fn get_all_channels(&self) -> &[u32; 16];
    fn get_channel(&self, channel: Channel) -> u32 {
        self.get_all_channels()[channel as usize] 
    }
    fn send_telemetry(&self, telemetry_packet: TelemetryPacket);
}

// these are eventually supposed to be the PWM values associated with these states
#[derive(Debug)]
pub enum SwitchState {
    HIGH = 3,
    MED = 2,
    LOW = 1,
}

pub trait ReceiverHandler: fmt::Debug {
    const EPSILON: u32;

    fn setup(&self);

    fn is_close(a: u32, b:u32) -> bool {
        (((a as i64) - (b as i64)).abs() as u32) < Self::EPSILON
    }

    fn set_smoothing_func(&mut self, channel: Channel, smoothing_func: fn(f32) -> f32);

    fn get_channel_as_switch(&self, channel: Channel) -> SwitchState;

    fn is_throttle_0(&self) -> bool;

    fn is_kill_switch_on(&self) -> bool;

    fn get_mode(&self) -> Mode;

    fn get_direction(&self) -> Direction;

    fn get_controls(&self) -> Controller;

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket);
}
