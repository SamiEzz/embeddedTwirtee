#include "simulator.h"

#include "measurement_randomization.h"

#include <stdio.h>
#include <pthread.h>
#include <time.h>


#define EXIT_FAILURE 1
void my_delay(int i)    /*Pause l'application pour i seconds*/
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC);
}
int start_thread(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    if(pthread_create(thread, attr, start_routine, arg) == -1) {
            perror("pthread_create");
            return EXIT_FAILURE;
    }
}

int end_thread(pthread_t th, void **thread_return){
    if (pthread_join(th, thread_return)) {
            perror("pthread_join");
            return EXIT_FAILURE;
    }
} 


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
    pthread_t t_simu;
    simu_param * simu;
    simu->end_request = 0;


    printf("init main\n");
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

        printf("%d \n",k);
    }

    return 0;
}