/*
 * twirtee.h
 *
 *  Created on: 11 janv. 2018
 *      Author: twirteeop
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "twirtee.h"

/* functions */
void update_simulation(Bool8 end_request, Speed speed_gui, Position pos_sp, Position kalm_res, Position *p, T_head *compass, T_loc *gps, T_odo *odometry);

#endif /* SIMULATOR_H_ */
