#ifndef T_MISC_
#define T_MISC_

#include <pthread.h>
#define debug 0


void * safe_alloc(int size);
void debug_msg(char * msg);

#endif