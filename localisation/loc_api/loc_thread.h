//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "dwm_api.h"
#include "hal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>

typedef struct T_loc {
	double x;
	double y;
	double z;
	double qf;
	bool val;
} T_loc;

void *loc_thread(void *_position);