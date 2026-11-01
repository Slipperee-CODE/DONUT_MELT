use super::Receiver;
use super::TelemetryPacket;

#[derive(Debug)]
pub struct UartReceiver;

impl UartReceiver {
    pub const fn new() -> Self { 
        todo!();
    }
}

impl Receiver for UartReceiver {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn get_all_channels(&self) -> &[u32; 16] {
        todo!();
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        todo!();
    }
}

#[derive(Debug)]
pub struct PwmReceiver;

impl PwmReceiver {
    pub fn new() -> Self { 
        todo!();
    }
}

impl Receiver for PwmReceiver {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn get_all_channels(&self) -> &[u32; 16] {
        todo!();
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        todo!();
    }
}

#[derive(Debug)]
pub struct DebugReceiver;

impl DebugReceiver {
    pub const fn new() -> Self { 
        todo!();
    }
}

impl Receiver for DebugReceiver {
    fn setup(&self) {
        // do any necessary setup here
    }

    fn get_all_channels(&self) -> &[u32; 16] {
        &[0; 16] 
    }

    fn send_telemetry(&self, telemetry_packet: TelemetryPacket) {
        println!("send_telemetry called on {:#?}", telemetry_packet);
    }
}
