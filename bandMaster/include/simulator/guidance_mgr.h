/**
******************************************************************************
* @file		guidance_mgr.h
* @author   [IRT] PHAVORIN Guillaume
* @brief	process guidance_manager
******************************************************************************
*/

#ifndef GUIDANCE_MGR_H_
#define GUIDANCE_MGR_H_

#include "../com.h"

#include "kanayama.h"

#define EPSILON				0.00001
#define _D_					0.05
#define FLOAT_DIV_2			0.50f
#define FLOAT_DIV_4			0.25f

#define DELTA_TIME 0.100 ///< 100 ms

void com_tracking (Position pos, Position pos_sp, T_mat *Q, Speed *speed_sp);

void com_generation(Position pos, Path *path_r, Position *pos_sp);

#endif /* GUIDANCE_MGR_H_ */
