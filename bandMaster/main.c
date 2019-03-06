#include "./include/simulator.h"
#include "./include/threads_mgr.h"

#include "./include/measurement_randomization.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define EXIT_FAILURE 1




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

    //###############################################
    //          Create main auxilary variables      #
    //###############################################
    double Pd[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	T_mat Q = {3, 3, _MAT_ Pd}; ///< Error matrix computed with Kalman filter
	Speed s_arp	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
	Speed s_rec	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
	Speed s_safety = {0.0f, 0.0f, 0.0f}; ///< Speed selected among the previous ones by the Speed selector

    //###############################################
    //          init variables                      #
    //###############################################
    // init mutex


    // init values


    //###############################################
    //                 Create threads               #
    //###############################################
    pthread_t t_simu;
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_kalman;
    pthread_t t_spf;

    //###############################################
    //              Main routine                    #
    //###############################################

    // Simulation thread
    start_thread(&t_simu, NULL, update_simulation, simu);
    
    end_thread(t_simu, NULL);

    //###############################################
    //              show results                    #
    //###############################################  

    printf("\nExecuted !\n");
    //###############################################
    //                  close                       #
    //###############################################


}