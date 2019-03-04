#include "simulator.h"
#include "threads_mgr.h"

#include "measurement_randomization.h"

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
/* functions */
//void update_simulation(Bool8 end_request, Speed speed_gui, Position pos_sp, Position kalm_res, Position *p, T_head *compass, T_loc *gps, T_odo *odometry);



int main(){
    #ifdef DEBUG 
        printf("init main\n");
    #endif
    pthread_t t_simu;
    //simu_param * simu = safe_alloc(sizeof(simu_param));
    simu_param simu[1];
    simu->end_request = 0;


    //initializations
	double Pd[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	T_mat Q = {3, 3, _MAT_ Pd}; ///< Error matrix computed with Kalman filter
	Speed s_arp	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
	Speed s_rec	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
	Speed s_safety = {0.0f, 0.0f, 0.0f}; ///< Speed selected among the previous ones by the Speed selector
	
	
    //
    simu->speed_gui = (Speed) {0.0f, 0.0f, VALID_DATA};
    //
    simu->pos_sp.x=0;
    simu->pos_sp.y=0;
    simu->kalm_res.x=0;
    simu->kalm_res.y=0;
    
     
    //
    T_odo m_odometry = (T_odo) {0.0, 0.0, 0.0f, 0.0f, 0, 0, VALID_DATA};
    simu->odometry = &m_odometry;
    //
    for(int k=0;k<20;k++){
        start_thread(&t_simu, NULL, update_simulation, simu);
        // wait for thread to execute 
        end_thread(t_simu, NULL);

        #ifdef DEBUG 
            printf("%d \n",k);
        #endif
    }

    return 0;
}