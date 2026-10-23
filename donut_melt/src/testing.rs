pub fn test() {
    let mut state = State;
    let settings = Settings;
    let vars = FirstSetOfImportantVariables;
    let now = 0.0;
    let mut curr_handler = construct_step1(3.0, now, vars);
    loop {
        let (state, mut next_handler) = curr_handler.handle(&state, &settings);
        curr_handler = match next_handler {
            Handler::StopHandling => break,
            _ => next_handler,
        }
    }
}

struct Settings;

#[derive(Clone)]
struct State;

enum Handler {
    KeepHandling {
        secs_remaining: f32,
        time_of_last_call: f32,
        f: Box<dyn FnOnce (f32, f32, &State, &Settings) -> (State, Handler)>, 
    },
    StopHandling,
}

impl Handler {
    fn handle(self, state: &State, settings: &Settings) -> (State, Self) {
        let now = 0.0;
        match self {
            Handler::KeepHandling { 
                secs_remaining, 
                time_of_last_call, 
                f 
            } => {
                f(secs_remaining, time_of_last_call, state, settings)
            }
            Handler::StopHandling => {
                (state.clone(), Handler::StopHandling)
            }
        }
    }
}

#[derive(Debug)]
struct FirstSetOfImportantVariables;
#[derive(Debug)]
struct SecondSetOfImportantVariables;

fn construct_step(secs_remaining: f32, time_of_last_call: f32, vars: FirstSetOfImportantVariables, step: fn(FirstSetOfImportantVariables) -> FirstSetOfImportantVariables) -> Handler {
    Handler::KeepHandling {
        secs_remaining,
        time_of_last_call,
        f: Box::new(|secs_remaining: f32, time_of_last_call: f32, state: &State, settings: &Settings| -> (State, Handler) {
            let now: f32 = 0.0;
            match secs_remaining > 0.0 {
                true => {
                    let vars = step(vars);
                    (state.clone(), construct_step(secs_remaining - (now - time_of_last_call), now, vars, step))
                },
                false => (state.clone(), construct_step2(3.0, now, SecondSetOfImportantVariables)),
            }
        }),
    }
}



fn do_step_1(vars: FirstSetOfImportantVariables) -> FirstSetOfImportantVariables {
    // do stuff here
    vars
}

fn construct_step1(secs_remaining: f32, time_of_last_call: f32, vars: FirstSetOfImportantVariables) -> Handler {
    Handler::KeepHandling {
        secs_remaining,
        time_of_last_call,
        f: Box::new(|secs_remaining: f32, time_of_last_call: f32, state: &State, settings: &Settings| -> (State, Handler) {
            let now: f32 = 0.0;
            match secs_remaining > 0.0 {
                true => {
                    let vars = do_step_1(vars);
                    (state.clone(), construct_step1(secs_remaining - (now - time_of_last_call), now, vars))
                },
                false => (state.clone(), construct_step2(3.0, now, SecondSetOfImportantVariables)),
            }
        }),
    }
}

fn do_step_2(vars: SecondSetOfImportantVariables) -> SecondSetOfImportantVariables {
    // do stuff here
    vars
}

fn construct_step2(secs_remaining: f32, time_of_last_call: f32, vars: SecondSetOfImportantVariables) -> Handler {
    Handler::KeepHandling {
        secs_remaining,
        time_of_last_call,
        f: Box::new(|secs_remaining: f32, time_of_last_call: f32, state: &State, settings: &Settings| -> (State, Handler) {
            let now: f32 = 0.0;
            match secs_remaining > 0.0 {
                true => {
                    let vars = do_step_2(vars);
                    (state.clone(), construct_step2(secs_remaining - (now - time_of_last_call), now, vars))
                },
                false => (state.clone(), Handler::StopHandling),
            }
        }),
    }
}