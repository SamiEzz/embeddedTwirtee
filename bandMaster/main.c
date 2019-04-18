#include "./include/misc.h"
#include "./include/linux-can-utils/read_can.h"
#include "./include/linux-can-utils/write_can.h"
#include "./include/threads_mgr.h"
#include "./include/guidance_mgr.h"
#include "./include/simulator.h"
#include "./include/speed_selection.h"

#include "./include/localization.h"
#include "./include/mission_mgr.h"

#include "./include/measurement_randomization.h"
#include "./include/spf_thread.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

char* out_tracking_file = "./output_txt/path.txt";
char* out_mission_file = "./output_txt/mission.txt";


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

int main() {

    //###############################################
    //                 output File                  #
    //###############################################

    FILE* f_tracking = fopen(out_tracking_file, "w");
    FILE* f_path = fopen(out_mission_file, "w");
    
    if (f_tracking) {
        debug_msg("main.c : output tracking file created !");
    } else {
        debug_msg("main.c : Unable to create tracking file.");
    }
    //###############################################
    //          Create shared variables             #
    //###############################################
    // Simulation
    simu_param simu[1];
    simu->max_iterations = 50000;
    simu->max_hach=1;
    simu->hach=simu->max_hach-1;
    float* offsetPosition[2];
    simu->p = (Position*)safe_alloc(sizeof(Position));
    simu->compass = (T_head*)safe_alloc(sizeof(T_head));
    simu->gps = (T_loc*)safe_alloc(sizeof(T_loc));
    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));
    *simu->odometry = (T_odo){0.0, 0.0, 0.0f, 0.0f, 0, 0, VALID_DATA};
    simu->end_request = 0;
    simu->speed_gui = (Speed){0.0f, 0.0f, VALID_DATA};
    simu->speed_out = (Speed){0.0f, 0.0f, VALID_DATA};

    //
    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));

    // Position
    // position_mtx * Pos = safe_alloc(sizeof(position_mtx));
    // Pos->position_var = (T_loc *)safe_alloc(sizeof(T_loc *));

    // mission
    spf_mission* mission_se = (spf_mission*)safe_alloc(sizeof(spf_mission));

    //###############################################
    //          init variables                      #
    //###############################################
    debug_msg("main.c : Variables initiation");
    // init mutex
    // Pos->mut = (pthread_mutex_t *)safe_alloc(sizeof(pthread_mutex_t));

    // init values

    //###############################################
    //          Create main auxilary variables      #
    //###############################################
    debug_msg("main.c : Auxilary variables"); 
    int abort_mission = 0;
    double Pd[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    T_mat Q = {3, 3, _MAT_ Pd};       ///< Error matrix computed with Kalman filter
    Speed s_arp = {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
    Speed s_rec = {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
    Speed s_safety = {0.0f, 0.0f,
                      0.0f}; ///< Speed selected among the previous ones by the Speed selector

    //###############################################
    //                 Create threads               #
    //###############################################
    debug_msg("main.c : Creating threads");
    pthread_t t_simu;
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_spf;
    pthread_t t_can_read;
    pthread_t t_can_write;
    
    //###############################################
    //              Main routine                    #
    //###############################################
    // CAN PROTOCOLE READ

    // char* can_name[]={"read_can", "can1"};
    // start_thread(&t_can_read, NULL, read_can, can_name);
    // // wait for thread to execute
    // end_thread(t_can_read, NULL);
    
    can_shared* can_buff = safe_alloc(sizeof(can_shared));
    
    start_thread(&t_can_write, NULL, write_can, can_buff);
    // wait for thread to execute
    end_thread(t_can_write, NULL);
    
    
    // Simulation thread
    debug_msg("main.c : Starting get_mission thread");
    start_thread(&t_get_mission, NULL, get_mission_thread, mission_se);
    // wait for thread to execute
    end_thread(t_get_mission, NULL);

    //###############################################
    //              show results                    #
    //###############################################
    //###############################################
    //                  close                       #
    //###############################################
    return EXIT_SUCCESS;
}
