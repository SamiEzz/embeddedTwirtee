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


int main(int argc, char const *argv[])
{
    Path * trajectorytest=safe_alloc(sizeof(Path));
    trajectorytest = (Path *) trajectorytest;
    static  spf_mission mission_se={
        .start = 13,
        .end = 22,
        .mut = PTHREAD_MUTEX_INITIALIZER
    };
    mission_se.path = trajectorytest;
    
   

    //---------- Creation des threads
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
        printf("- %d\n",mission_se.path->dest[i]->id);    
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