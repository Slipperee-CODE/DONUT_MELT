use std::fmt;

pub trait Motor: fmt::Debug {
    fn set_throttle(&mut self, throttle: f32); 

    async fn send_throttle(&self);

    fn stop(&mut self) {
        self.set_throttle(0.0);
    }
}

#[derive(Debug)]
pub struct PIO;

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
pub struct DShotMotor {
    motor_pin: u8,
    dshot_speed: DShot,
    motor_pio: PIO,
    throttle: f32,
}

impl DShotMotor {
    fn new(motor_pin: u8, dshot_speed: DShot, motor_pio: PIO) -> Self {
       Self { motor_pin, dshot_speed, motor_pio, throttle: 0.0 } 
    }
}

impl Motor for DShotMotor {
    fn set_throttle(&mut self, throttle: f32) {
        self.throttle = throttle;
    }

    async fn send_throttle(&self) {
        // send throttle to motor using DSHOT
        // wait the appropriate amount of time according to DSHOT version
    }
}

#[derive(Debug)]
pub struct PwmMotor {
    motor_pin: u8,
    motor_pio: PIO,
    throttle: f32,
}

impl PwmMotor {
    fn new(motor_pin: u8, motor_pio: PIO) -> Self {
       Self { motor_pin, motor_pio, throttle: 0.0 } 
    }
}

impl Motor for PwmMotor {
    fn set_throttle(&mut self, throttle: f32) {
        self.throttle = throttle;
    }

    async fn send_throttle(&self) {
        // send throttle to motor using PWM
    }
}

#[derive(Debug)]
pub struct DebugMotor {
    dshot_speed: DShot,
    throttle: f32,
}

impl DebugMotor {
    fn new(dshot_speed: DShot) -> Self { 
        Self { dshot_speed, throttle: 0.0 }
    }
}

impl Motor for DebugMotor {
    fn set_throttle(&mut self, throttle: f32) {
        println!("set_throttle called on {throttle}");

        self.throttle = throttle;
    }

    async fn send_throttle(&self) { 
        println!("send_throttle called with {self.throttle}");
    }
}
