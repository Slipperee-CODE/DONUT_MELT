#include "donut_pdc.h"

double pdc_get_pdc_output(pdc_state_t* pdc_state, bool only_smooth_up) {
    double error = pdc_state->curr_target - pdc_state->curr_value;
    
    if (only_smooth_up && error <= 0) {
        pdc_state->last_error = error;
        return pdc_state->curr_target;
    }

    double output = pdc_state->P * error + pdc_state->D * (error - pdc_state->last_error);
    pdc_state->last_error = error;
    pdc_state->curr_value = output;
    return output;
}