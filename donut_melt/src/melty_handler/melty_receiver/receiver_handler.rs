use super::{Receiver, ReceiverHandler};
use super::{Channel, TelemetryPacket, SwitchState, Mode, Direction, Controller, Frame, Animation};

#[derive(Debug)]
pub struct MeltyReceiverHandler<R: Receiver> {
    receiver: R,
    smoothing_funcs: [fn(f32) -> f32; 4],
}

impl<R: Receiver> MeltyReceiverHandler<R> {
    pub const fn new(receiver: R) -> Self { 
        Self { 
            receiver,
            smoothing_funcs: [Self::linear, Self::linear, Self::linear, Self::linear],
        }
    }

    fn linear(input: f32) -> f32 {
        input
    }
}

impl<R: Receiver> ReceiverHandler for MeltyReceiverHandler<R> {
    const EPSILON: u32 = 100;

    fn setup(&self) {
        // do any necessary setup here
    }

    fn set_smoothing_func(&mut self, channel: Channel, smoothing_func: fn(f32) -> f32) {
        self.smoothing_funcs[channel as usize] = smoothing_func;
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

    fn get_direction(&self) -> Direction {
        match self.get_channel_as_switch(Channel::DIRECTION_SWITCH) {
            SwitchState::HIGH => Direction::NORMAL,
            SwitchState::MED => Direction::NORMAL,
            SwitchState::LOW => Direction::REVERSE,
        }
    }

    fn get_controls(&self) -> Controller {
        let denom: f32 = (SwitchState::HIGH as u32 - SwitchState::LOW as u32) as f32; 
        let normalize = |x: u32| -> f32 {(x - SwitchState::LOW as u32) as f32 / denom};
        let smooth = |i: usize, x: f32| -> f32 { self.smoothing_funcs[i](x) };
        Controller { 
            mode: self.get_mode(),
            direction: self.get_direction(),
            left_x: smooth(0,normalize(self.receiver.get_channel(Channel::LEFT_X))),
            left_y: smooth(1,normalize(self.receiver.get_channel(Channel::LEFT_Y))),
            right_x: smooth(2, normalize(self.receiver.get_channel(Channel::RIGHT_X))),
            right_y: smooth(3, normalize(self.receiver.get_channel(Channel::RIGHT_Y))),
        }
    }

    fn get_animations(&self) -> Option<Animation> {
        Some(Box::new([Frame(|| println!("1"), 0.0), Frame(|| println!("2"), 1.0)].into_iter()))
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        self.receiver.send_telemetry(telemetry_packet)
    }
}