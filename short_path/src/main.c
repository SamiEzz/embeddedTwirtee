//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"
#include "../include/spf_thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>



#define outputFile "output.nodes"

typedef struct T_loc {
	double x;
	double y;
	double z;
	double qf;
	bool val;
} T_loc;

typedef struct {
    pthread_mutex_t mut;
    Path mission_var;
} mission_mtx;

typedef struct {
    pthread_mutex_t mut;
    T_loc position_var;
} position_mtx;

static  spf_mission mission_se={
    .start = 13,
    .end = 22,
    .mut = PTHREAD_MUTEX_INITIALIZER
};


int main(int argc, char const *argv[])
{

    Path * trajectorytest=safe_alloc(sizeof(Path));
    trajectorytest = (Path *) trajectorytest;
    mission_se.path = trajectorytest;
    
   
    // looooooooooooooooooooooooooop
    while(1){
        //---------- Creation des threads
        // get mission thread
        // TODO = connect directly with IHM  to get mission
        pthread_t t_get_mission;
        if(pthread_create(&t_get_mission, NULL, get_mission_thread, &mission_se) == -1) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }

        if (pthread_join(t_get_mission, NULL)) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
        // short path first thread
        pthread_t t_spf;
        if(pthread_create(&t_spf, NULL, spf_thread, &mission_se) == -1) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }  

        // wait for thread to execute 
        //void ** returned_path;

        if (pthread_join(t_spf, NULL)) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
        // end of thread
        
        for(int i=0;i<mission_se.path->size;i++){
            printf("-> %d",mission_se.path->dest[i]->id);    
        }
        
    }
    
    printf("fin du main\n");
    return EXIT_SUCCESS;
    return 0;
}


void system_loop(){
    // Init varibles

    // get mission

    // sensor data initiation

    
}