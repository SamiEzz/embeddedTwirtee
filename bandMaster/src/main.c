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



#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>



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

    // shared variables
    position_mtx * Pos = (position_mtx *)safe_alloc(sizeof(position_mtx));
    Pos->mut = (pthread_mutex_t *)safe_alloc(sizeof(pthread_mutex_t));
    spf_mission * mission_se = (spf_mission *) safe_alloc(sizeof(spf_mission));
    
    
    pthread_mutex_init(Pos->mut, NULL);

    Pos->position_var.x=-1;
    Pos->position_var.y=-1;
    Pos->position_var.z=-1;
    Pos->position_var.qf=-1;
    Pos->position_var.val = 0;
    
    
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_kalman;
    pthread_t t_spf;

    

    //printf("\nval : %d",Pos->position_var.val);
    // looooooooooooooooooooooooooop
        //---------- Creation des threads
        // get mission thread
        // TODO = connect directly with IHM  to get mission
        start_thread(&t_get_mission, NULL, get_mission_thread, mission_se);
        // wait for thread to execute 
        end_thread(t_get_mission, NULL);
        
        // short path first thread
        start_thread(&t_spf, NULL, spf_thread, mission_se);
        // wait for thread to execute 
        end_thread(t_spf, NULL);
        
        for(int i=0;i<mission_se->path->size;i++){
            printf("-> %d\n",mission_se->path->dest[i]->id);    
        }
        start_thread(&t_localisation,NULL,loc_thread,Pos);
     while(1){

        pthread_mutex_lock(Pos->mut);
        printf("x -> %d\n",Pos->position_var.x);    
        printf("y -> %d\n",Pos->position_var.y);    
        printf("z -> %d\n",Pos->position_var.z);    
        printf("qf -> %d\n",Pos->position_var.qf);    
        pthread_mutex_unlock(Pos->mut);

    }
    
        end_thread(t_localisation,NULL);
        
    
    printf("\nfin du main\n");
    return EXIT_SUCCESS;
    return 0;
}


void system_loop(){
    // Init varibles

    // get mission

    // sensor data initiation

    
}