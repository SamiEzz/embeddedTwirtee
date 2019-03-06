//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


//#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>



typedef struct spf_mission{
    int start;
    int end;
    Path * path;
    pthread_mutex_t mut;
}spf_mission;


void* get_mission_thread (void * mission_se);
void *spf_thread(void *mission_se);
