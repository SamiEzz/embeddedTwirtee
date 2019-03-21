#include "./include/guidance_mgr.h"
#include "./include/simulator.h"
#include "./include/speed_selection.h"
#include "./include/threads_mgr.h"

#include "./include/localization.h"
#include "./include/mission_mgr.h"

#include "./include/measurement_randomization.h"
#include "./include/misc.h"
#include "./include/spf_thread.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

char* out_tracking_file = "./output_txt/out_tracking.txt";

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
    simu->p = (Position*)safe_alloc(sizeof(Position));
    simu->compass = (T_head*)safe_alloc(sizeof(T_head));
    simu->gps = (T_loc*)safe_alloc(sizeof(T_loc));
    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));
    *simu->odometry = (T_odo){0.0, 0.0, 0.0f, 0.0f, 0, 0, INVALID_DATA};
    simu->end_request = 0;
    simu->speed_gui = (Speed){0.0f, 0.0f, VALID_DATA};
    simu->speed_out = (Speed){0.0f, 0.0f, VALID_DATA};

    // 1995.581055,-199.699997
    simu->pos_sp.x = 0;
    simu->pos_sp.y = 0;
    simu->kalm_res.x = 0;
    simu->kalm_res.y = 0;
    //
    simu->p->x = 0;
    simu->p->y = 0;
    simu->p->theta = 0;
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
    // init mutex
    // Pos->mut = (pthread_mutex_t *)safe_alloc(sizeof(pthread_mutex_t));

    // init values

    //###############################################
    //          Create main auxilary variables      #
    //###############################################
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
    pthread_t t_simu;
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_spf;

    //###############################################
    //              Main routine                    #
    //###############################################

    // Simulation thread

    start_thread(&t_get_mission, NULL, get_mission_thread, mission_se);
    // wait for thread to execute
    end_thread(t_get_mission, NULL);

    // short path first thread
    // spf_thread(mission_se);
    start_thread(&t_spf, NULL, spf_thread, mission_se);
    // wait for thread to execute
    end_thread(t_spf, NULL);

    //simu->pos_sp.x = mission_se->path->dest[1]->x;
    //simu->pos_sp.y = mission_se->path->dest[1]->y;
    //simu->kalm_res.x = mission_se->path->dest[0]->x;
    //simu->kalm_res.y = mission_se->path->dest[0]->y;
    //
    mission_se->path->dest[0]->x=0;
    mission_se->path->dest[0]->y=0;
    mission_se->path->dest[1]->x=100;
    mission_se->path->dest[1]->y=100;
    
    simu->p->x = mission_se->path->dest[0]->x;
    simu->p->y = mission_se->path->dest[0]->y;
    //

    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));
    int max_iterations = 1000;

    // fprintf(f_tracking, "\nMission : id,x,y \n");
    // for (int i = 0; i < mission_se->path->size; i++) {
    //     fprintf(f_tracking, "-> %d,%f,%f\n", mission_se->path->dest[i]->id,
    //             mission_se->path->dest[i]->x, mission_se->path->dest[i]->y);
    // }
    // fprintf(f_tracking, "\niteration,x,y,theta");
    for (int iterate = 0; iterate < max_iterations; iterate++) {
        // com_localize(compass, gps, odometry, &kalman_position, &Q);		///< Kalman call for
        // better position estimation from noised data
        com_localize(*simu->compass, *simu->gps, *simu->odometry, &simu->kalm_res, &Q);

        // com_generation(kalman_position, &path, &position_sp); ///< defines SetPoint
        com_generation(simu->kalm_res, mission_se->path, &simu->pos_sp);
        // printf("x:%f, y: %f\n",simu->pos_sp.x,simu->pos_sp.y);
        // com_tracking(kalman_position, position_sp, &Q, &s_gui); ///< Kanayama call to track the
        // defined SetPoint
        com_tracking(simu->kalm_res, simu->pos_sp, &Q, &simu->speed_gui);

        // com_speed_selection(s_gui, s_rec, s_safety, s_arp, &s_out);	///< Shall select the
        // biggest speed asserting s_out <= min(s_safety, s_arp)
        com_speed_selection(simu->speed_gui, s_rec, s_safety, s_arp, &simu->speed_out);

        update_simulation(simu);
        // start_thread(&t_simu, NULL, update_simulation, simu);
        // end_thread(t_simu, NULL);


        // fprintf(f_tracking, "\n%d,%f,%f,%f", iterate, simu->kalm_res.x, simu->kalm_res.y,
        //         simu->kalm_res.theta);
    }
    //###############################################
    //              show results                    #
    //###############################################

    for (int i = 0; i < mission_se->path->size; i++) {
        printf("-> %d\n", mission_se->path->dest[i]->id);
    }
    debug_msg("Executed !");

    //###############################################
    //                  close                       #
    //###############################################
    return EXIT_SUCCESS;
}