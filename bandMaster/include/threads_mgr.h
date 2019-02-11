

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
	int val;
} T_loc;

typedef struct {
    pthread_mutex_t mut;
    Path mission_var;
} mission_mtx;

typedef struct {
    pthread_mutex_t mut;
    T_loc position_var;
} position_mtx;

int start_thread(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int end_thread(pthread_t th, void **thread_return);