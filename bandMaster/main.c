#include "./include/simulator.h"
#include "./include/threads_mgr.h"
#include "./include/mission_mgr.h"

#include "./include/measurement_randomization.h"
#include "./include/spf_thread.h"
#include "./include/misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0





/*
typedef struct simu_param(){
    Bool8 end_request; 
    Speed speed_gui; 
    Position pos_sp; 
    Position kalm_res; 
    Position *p; 
    T_head *compass; 
    T_loc *gps; 
    T_odo *odometry;
} simu_param;
*/
   


int main(){
    
    //###############################################
    //          Create shared variables             #
    //###############################################
    // Simulation
    simu_param simu[1];
    simu->p=safe_alloc(sizeof(Position));
    simu->compass=safe_alloc(sizeof(T_head));
    simu->gps=safe_alloc(sizeof(T_loc));
    simu->odometry=safe_alloc(sizeof(T_odo));
    simu->end_request = 0;
    simu->speed_gui = (Speed) {0.0f, 0.0f, VALID_DATA};
    //
    simu->pos_sp.x=0;
    simu->pos_sp.y=0;
    simu->kalm_res.x=0;
    simu->kalm_res.y=0;
    //
    simu->p->x=0;
    simu->p->y=0;
    simu->p->theta=0;
    //
    simu->odometry = (T_odo *)safe_alloc(sizeof(T_odo));    
    
    // Position
    //position_mtx * Pos = safe_alloc(sizeof(position_mtx));
    //Pos->position_var = (T_loc *)safe_alloc(sizeof(T_loc *));
    
    // mission
    spf_mission * mission_se = (spf_mission *) safe_alloc(sizeof(spf_mission));


    //###############################################
    //          init variables                      #
    //###############################################
    // init mutex
    //Pos->mut = (pthread_mutex_t *)safe_alloc(sizeof(pthread_mutex_t));


    // init values

    //###############################################
    //          Create main auxilary variables      #
    //###############################################
    double Pd[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	T_mat Q = {3, 3, _MAT_ Pd}; ///< Error matrix computed with Kalman filter
	Speed s_arp	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
	Speed s_rec	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
	Speed s_safety = {0.0f, 0.0f, 0.0f}; ///< Speed selected among the previous ones by the Speed selector

    //###############################################
    //                 Create threads               #
    //###############################################
    pthread_t t_simu;
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_spf;

    //###############################################
    //              Main routine                    #
    //###############################################

    // Simulation thread
    start_thread(&t_simu, NULL, update_simulation, simu);
    end_thread(t_simu, NULL);

/*
    start_thread(&t_get_mission, NULL, get_mission_thread, mission_se);
    // wait for thread to execute 
    end_thread(t_get_mission, NULL);
 */
    // short path first thread
    spf_thread(mission_se);
    start_thread(&t_spf, NULL, spf_thread, mission_se);
    // wait for thread to execute 
    end_thread(t_spf, NULL);


    //###############################################
    //              show results                    #
    //###############################################  
    
    for(int i=0;i<mission_se->path->size;i++){
        printf("-> %d\n",mission_se->path->dest[i]->id);    
    }
    debug_msg("Executed !");
    
    //###############################################
    //                  close                       #
    //###############################################
    return EXIT_SUCCESS;
}