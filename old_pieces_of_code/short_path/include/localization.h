/**
 * @file localization.h
 * @author Guillaume PHAVORIN
 * @brief process localization
*/

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include "com.h"

#define KALMAN_SAMPLE_PERIOD	0.1			/* in sec */

void com_localize(T_head compass, T_loc gps, T_odo odometry, Position *pos, T_mat *Q);

#endif /* LOCALIZATION_H_ */
