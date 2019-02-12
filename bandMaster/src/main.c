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
#include "../include/loc_thread.h"
#include "../include/threads_mgr.h"


//#include "../loc_api/loc_thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>



#define outputFile "output.nodes"



// typedef struct T_loc {
// 	double x;
// 	double y;
// 	double z;
// 	double qf;
// 	bool val;
// } T_loc;


// typedef struct {
//     pthread_mutex_t mut;
//     T_loc position_var;
// } position_mtx;
int main(int argc, char const *argv[]){
    
    static pthread_mutex_t pos_mtx=PTHREAD_MUTEX_INITIALIZER;

    position_mtx * Pos;
    Pos->position_var.x=-1;
    Pos->position_var.y=-1;
    Pos->position_var.z=-1;
    Pos->position_var.qf=-1;
    Pos->position_var.val = 0;
    

    static  spf_mission mission_se={
        .start = 13,
        .end = 22,
        .mut = PTHREAD_MUTEX_INITIALIZER
    };
        
    Path * trajectorytest=safe_alloc(sizeof(Path));
    //trajectorytest = (Path *) trajectorytest;
    mission_se.path = trajectorytest;
    T_loc pos;

    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_kalman;
    pthread_t t_spf;
    
    start_thread(&t_localisation,NULL,loc_thread,Pos);

    end_thread(t_localisation,NULL);

    printf("\nval : %d",Pos->position_var.val);
    // looooooooooooooooooooooooooop
    /* while(1){
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
    }
    */
        
    
    printf("\nfin du main\n");
    return EXIT_SUCCESS;
    return 0;
}


void system_loop(){
    // Init varibles

    // get mission

    // sensor data initiation

    
}