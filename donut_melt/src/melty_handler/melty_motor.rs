use std::fmt;

pub trait Motor: fmt::Debug {
    fn set_throttle(&mut self, throttle: f32); 

    async fn send_throttle(&self);

    fn stop(&mut self) {
        self.set_throttle(0.0);
    }
}

#[derive(Debug)]
pub enum DShot {
    DShot150,
    DShot300,
    DShot600,
    DShot1200,
}

impl DShot {
    fn frame_time(&self) -> f32 {
        match self {
            Self::DShot150 => 106.7,
            Self::DShot300 => 53.3,
            Self::DShot600 => 26.7,
            Self::DShot1200 => 13.3,
        }
    }
}

#[derive(Debug)]
pub struct PIO;

#[derive(Debug)]
pub struct MeltyMotor {
    motor_pin: u8,
    dshot_speed: DShot,
    motor_pio: PIO,
    throttle: f32,
}

impl MeltyMotor {
    fn new(motor_pin: u8, dshot_speed: DShot, motor_pio: PIO) -> Self {
        MeltyMotor { motor_pin, dshot_speed, motor_pio, throttle: 0.0 } 
    }
}

impl Motor for MeltyMotor {
    fn set_throttle(&mut self, throttle: f32) {
        self.throttle = throttle;
    }

    async fn send_throttle(&self) {
        // send_throttle to motor
        // wait the appropriate amount of time according to DSHOT version
    }
}
