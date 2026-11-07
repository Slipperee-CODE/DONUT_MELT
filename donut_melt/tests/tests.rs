use donut_melt::MeltyHandler;

// write various tests that use the debug objects in this file!
// will probably need to import more than just Melty Handler lol
// research how to run diff tests like "cargo test test_name -- --show-output"
// interestingly, Rust will run all tests for which "test" is a prefix by default so if you're 
// clever with your test naming you can get groups of tests to run all at once just via name
#[test]
fn test_feature() {
    assert_eq!(your_function(), true);
}

// put non-integration tests (i.e. single module tests of the debug structs for a trait)
// in their respective melty files (e.g. melty_led, melty_accel, etc.)