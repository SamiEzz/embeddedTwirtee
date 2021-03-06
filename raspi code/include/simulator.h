/*
 * twirtee.h
 *
 *  Created on: 11 janv. 2018
 *      Author: twirteeop
 */
#include "twirtee.h"

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

typedef struct simu_param {
    int hach;
    int max_hach;
    int max_iterations;
    Speed speed_out;
    Bool8 end_request;
    Speed speed_gui;
    Position pos_sp;
    Position kalm_res;
    Position* p;
    T_head* compass;
    T_loc* gps;
    T_odo* odometry;
} simu_param;
/* functions */
// void update_simulation(Bool8 end_request, Speed speed_gui, Position pos_sp, Position kalm_res,
// Position *p, T_head *compass, T_loc *gps, T_odo *odometry);

void* update_simulation(void* __simu);
#endif /* SIMULATOR_H_ */
