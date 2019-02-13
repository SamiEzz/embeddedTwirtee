#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include "../include/threads_mgr.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

void my_delay(int i)    /*Pause l'application pour i seconds*/
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC);
}
int start_thread(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    if(pthread_create(thread, attr, start_routine, arg) == -1) {
            perror("pthread_create");
            return EXIT_FAILURE;
    }
}

int end_thread(pthread_t th, void **thread_return){
    if (pthread_join(th, thread_return)) {
            perror("pthread_join");
            return EXIT_FAILURE;
    }
} 

