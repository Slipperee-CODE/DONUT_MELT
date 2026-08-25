use std::fmt;
use std::default::Default;
use super::melty_control::{Mode, Controller, Frame, Animation};

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
    const SPIN_SWITCH: Channel = Channel::SWITCH_B;
}

#[derive(Debug)]
pub struct TelemetryPacket(u16, u16, u32, u8);

pub trait Receiver: fmt::Debug {
    fn get_all_channels(&self) -> &[u32; 16];
    fn get_channel(&self, channel: Channel) -> u32;
    fn send_telemetry(&self, telemetry_packet: TelemetryPacket);
}

#[derive(Debug)]
pub struct MeltyReceiver;

impl MeltyReceiver {
    fn new() -> Self { 
        todo!();
    }
}

impl Receiver for MeltyReceiver {
    fn get_all_channels(&self) -> &[u32; 16] {
        todo!();
    }

    fn get_channel(&self, channel: Channel) -> u32 {
        todo!();
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        todo!();
    }
}

#[derive(Debug)]
pub enum SwitchState {
    HIGH = 3,
    MED = 2,
    LOW = 1,
}

pub trait ReceiverHandler: fmt::Debug {
    const EPSILON: u32;

    fn is_close(a: u32, b:u32) -> bool {
        (((a as i64) - (b as i64)).abs() as u32) < Self::EPSILON
    }

    fn set_smoothing_func(&mut self, channel: Channel, smoothing_func: fn(f32) -> f32) {

    fn get_channel_as_switch(&self, channel: Channel) -> SwitchState;

    fn is_throttle_0(&self) -> bool;

    fn is_kill_switch_on(&self) -> bool;

    fn get_mode(&self) -> Mode;

    fn spin_switch(&self) -> SwitchState;

    fn get_controls(&self) -> Animation;

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket);
}

#[derive(Debug)]
pub struct MeltyReceiverHandler<R: Receiver> {
    receiver: R,
    smoothing_funcs: [fn(f32) -> f32: 4];
}

impl<R: Receiver> MeltyReceiverHandler<R> {
    pub fn new(receiver: R) -> Self { 
        Self { 
            receiver,
            smoothings_funcs: [linear, linear, linear, linear],
        }
    }

    fn linear(input: f32) -> f32 {
        input
    }
}

impl<R: Receiver> ReceiverHandler for MeltyReceiverHandler<R> {
    const EPSILON: u32 = 100;

    fn set_smoothing_func(&mut self, channel: Channel, smoothing_func: fn(f32) -> f32) {
        self.smoothings_funcs[channel as usize] = smoothing_func;
    }

    fn get_channel_as_switch(&self, channel: Channel) -> SwitchState {
        let switch_value: u32 = self.receiver.get_channel(channel);
        if Self::is_close(switch_value, SwitchState::HIGH as u32) {
            SwitchState::HIGH
        } else if Self::is_close(switch_value, SwitchState::MED as u32) {
            SwitchState::MED
        } else {
            SwitchState::LOW
        }
    }

    fn is_throttle_0(&self) -> bool {
        Self::is_close(self.receiver.get_channel(Channel::THROTTLE), 0)
    }

    fn is_kill_switch_on(&self) -> bool {
        Self::is_close(self.receiver.get_channel(Channel::KILL_SWITCH), SwitchState::HIGH as u32)
    }

    fn get_mode(&self) -> Mode {
        match self.get_channel_as_switch(Channel::MODE_SWITCH) {
            SwitchState::HIGH => Mode::MELTY,
            SwitchState::MED => Mode::FAST_TANK,
            SwitchState::LOW => Mode::TANK,
        }
    }

    fn spin_switch(&self) -> SwitchState {
        self.get_channel_as_switch(Channel::SPIN_SWITCH)
    }

    fn get_controls(&self) -> Animation {
        let denom: f32 = (SwitchState::HIGH as u32 - SwitchState::LOW as u32) as f32; 
        let normalize = |x: u32| -> f32 {(x - SwitchState::LOW as u32) as f32 / denom};
        Animation { 
            curr: Frame { 
                controller: Controller { 
                    mode: self.get_mode(),
                    left_x: normalize(self.receiver.get_channel(Channel::LEFT_X)),
                    left_y: normalize(self.receiver.get_channel(Channel::LEFT_Y)),
                    right_x: normalize(self.receiver.get_channel(Channel::RIGHT_X)),
                    right_y: normalize(self.receiver.get_channel(Channel::RIGHT_Y)),
                },
                duration: 0,
            },
            remaining: None,
        }
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        self.receiver.send_telemetry(telemetry_packet)
    }
}
