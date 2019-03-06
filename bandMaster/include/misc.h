#ifndef T_MISC_
#define T_MISC_

#include <pthread.h>

typedef struct spf_mission{
    int start;
    int end;
    Path * path;
    pthread_mutex_t mut;
}spf_mission;

typedef struct {
    pthread_mutex_t mut;
    Path mission_var;
} mission_mtx;

void * safe_alloc(int size);

#endif