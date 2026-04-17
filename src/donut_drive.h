#ifndef DONUT_DRIVE_H
#define DONUT_DRIVE_H

#include "donut_config.h"

void drive_update_bot_state(bot_state_t* bot_state, double left_y_percent, double right_y_percent, double right_x_percent, double (*get_rpm)());

#endif