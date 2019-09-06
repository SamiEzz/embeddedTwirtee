#ifndef __threads_mgr__
#define __threads_mgr__


#include "twirtee.h"
#include "misc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>


//void * safe_alloc(int size);
void my_delay(int i);    /*Pause l'application pour i seconds*/

int start_thread(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int end_thread(pthread_t th, void **thread_return);
#endif