/**
******************************************************************************
* @file		mission_mgr.h
* @author   [IRT] PHAVORIN Guillaume
* @brief	process mission_manager
******************************************************************************
*/

#include "com.h"

#ifndef MISSION_MGR_H_
#define MISSION_MGR_H_

#define MISSION_DELTA_TIME 0.01
#define MAX_FLT  3.402823e+38
#define MAX_POINTS_BEFORE_SETPOINT 12

void com_mission_ordering(Mission *mission0, Mission *o_mission, Bool8 *abort);

void com_path_computing(Bool8 abort, Mission *mission, Cartography *map, Path *path_r);

#endif /* MISSION_MGR_H_ */
