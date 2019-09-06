/**
 * @file speed_selection.h
 * @author Guillaume PHAVORIN
 * @brief process speed selection
*/

#ifndef SPEED_SELECTION_H_
#define SPEED_SELECTION_H_

#include "com.h"

#define MAXSPEED		0.51f	/* Max speed of Rover= 1.043, Reduce Max speed more control movement */
#define MAXANGULARSPEED	3.500f	/* Max Angular Speed = 6.953, Value must be computed in function of MAXSPEED */

void com_speed_selection (Speed gui, Speed rec, Speed safety, Speed arp, Speed *speed_sp);

#endif /* SPEED_SELECTION_H_ */
