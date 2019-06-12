#ifndef T_MISC_
#define T_MISC_

#include <pthread.h>
#include "twirtee.h"


#define debug 0


void * safe_alloc(int size);
void debug_msg(char * msg);

void preMission(Path* p,float x,float y);
void postMission(Path* p,float* offset);

#endif
