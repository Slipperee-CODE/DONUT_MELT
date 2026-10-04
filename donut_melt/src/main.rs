mod melty_handler;

fn main() {
    // TODO List
    // - make all necessary new functions const fn as opposed to fn
    // so that they can be used in melty_handler's type-associated consts
    // - create setup functions for all handlers and the objects they handle
    // - create a fully complete main function setup for running the code on 
    // multiple (2) threads + concurrently on both of those threads
    // - when main function is complete test using melty_handler's DEBUG const
    // - Implement MeltyAccel fully
    // - Implement DShotMotor fully
    // - Implement UartReceiver fully
    // 
    // EXTRA:
    // - Implement PwmMotor fully
    // - Implement PwmReceiver fully

    // MORE TODO 
    // - setup should never take any parameters so that melty_handler logic can be generic
    // - structs which represent different systems of the robot should be initialized with their setup params 

    // Use sinusoidal translation for melty movement logic

    // - Make sure it's possible to adjust accelerometer offsets/offset the overall rpm 
    //   reading permanently by some varying factor during a match
    //  - make sure that permanent reading adjustments are lower bound and upper bound limited
    //  - might need to add a set_rpm_offset_factor to the Accel/AccelHandler Trait
    //
    // - Create routines for accelerometer data denoising
    //  - Create one where robot calibrates sitting still
    //  - Create one where robot calibrates by spinning up to various known 
    //    rpms and reading the accelerometer (the robot should be able to 
    //    use this information to get more accurate readings when around those rpms)
}
