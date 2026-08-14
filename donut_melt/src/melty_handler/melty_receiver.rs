use std::fmt;

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
    HIGH,
    MED,
    LOW,
}

impl SwitchState {
    const HIGH_VALUE: u32 = 3;
    const MED_VALUE: u32 = 2;
    const LOW_VALUE: u32 = 1;
}

pub trait ReceiverHandler: fmt::Debug {
    const EPSILON: u32;

    fn is_close(a: u32, b:u32) -> bool {
        (((a as i64) - (b as i64)).abs() as u32) < Self::EPSILON
    }

    fn get_channel_as_switch(&self, channel: Channel) -> SwitchState;

    fn is_throttle_0(&self) -> bool;

    fn is_kill_switch_on(&self) -> bool;

    fn mode_switch(&self) -> SwitchState;

    fn spin_switch(&self) -> SwitchState;
}

#[derive(Debug)]
pub struct MeltyReceiverHandler<R: Receiver> {
    receiver: R,
}

impl<R: Receiver> MeltyReceiverHandler<R> {
    fn new() -> Self { 
        todo!();
    }
}

impl<R: Receiver> ReceiverHandler for MeltyReceiverHandler<R> {
    const EPSILON: u32 = 100;

    fn get_channel_as_switch(&self, channel: Channel) -> SwitchState {
        let switch_value: u32 = self.receiver.get_channel(channel);
        if Self::is_close(switch_value, SwitchState::HIGH_VALUE) {
            SwitchState::HIGH
        } else if Self::is_close(switch_value, SwitchState::MED_VALUE) {
            SwitchState::MED
        } else {
            SwitchState::LOW
        }
    }

    fn is_throttle_0(&self) -> bool {
        Self::is_close(self.receiver.get_channel(Channel::THROTTLE), 0)
    }

    fn is_kill_switch_on(&self) -> bool {
        Self::is_close(self.receiver.get_channel(Channel::KILL_SWITCH), SwitchState::HIGH_VALUE)
    }

    fn mode_switch(&self) -> SwitchState {
        self.get_channel_as_switch(Channel::MODE_SWITCH)
    }

    fn spin_switch(&self) -> SwitchState {
        self.get_channel_as_switch(Channel::SPIN_SWITCH)
    }
}
