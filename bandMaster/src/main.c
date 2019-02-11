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
#include "../include/threads_mgr.h"


//#include "../loc_api/loc_thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>



#define outputFile "output.nodes"


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
    
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_spf;

    // looooooooooooooooooooooooooop
    //while(1){
        //---------- Creation des threads
        // get mission thread
        // TODO = connect directly with IHM  to get mission
        start_thread(&t_get_mission, NULL, get_mission_thread, &mission_se);
        // wait for thread to execute 
        end_thread(t_get_mission, NULL);
        
        // short path first thread
        start_thread(&t_spf, NULL, spf_thread, &mission_se);
        // wait for thread to execute 
        end_thread(t_spf, NULL);
        
        for(int i=0;i<mission_se.path->size;i++){
            printf("-> %d",mission_se.path->dest[i]->id);    
        }

        
    //}
    
    printf("fin du main\n");
    return EXIT_SUCCESS;
    return 0;
}


void system_loop(){
    // Init varibles

    // get mission

    // sensor data initiation

    
}