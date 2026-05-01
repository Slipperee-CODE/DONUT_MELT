#include "donut_pc.h"

void pc_update_pc_state(pc_state_t* pc_state, uint64_t dt) {
    if (dt == 0) {
        return;
    }

    double error = pc_state->curr_target - pc_state->curr_value;
    
    double dt_sec = (double) dt / 1000000.0;

    double amount_to_change = pc_state->P * error + pc_state->D * (error - pc_state->last_error) / dt_sec;
    pc_state->last_error = error;
    pc_state->curr_value += amount_to_change * dt_sec;
}