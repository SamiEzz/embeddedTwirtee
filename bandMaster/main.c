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

void preMission(Path* p,float** offset){
    *offset[0]=p->dest[0]->x;
    *offset[1]=p->dest[0]->y;
    
    for(int t=0;t<p->size;t++){
        p->dest[t]->x-=*offset[0];
        p->dest[t]->y-=*offset[1];
    }    
}
void postMission(Path* p,float* offset){
    for(int t=0;t<p->size;t++){
        p->dest[t]->x+=offset[0];
        p->dest[t]->y+=offset[1];
    }    
}

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

    //###############################################
    //              Main routine                    #
    //###############################################

    // Simulation thread
    debug_msg("main.c : Starting get_mission thread");
    start_thread(&t_get_mission, NULL, get_mission_thread, mission_se);
    // wait for thread to execute
    end_thread(t_get_mission, NULL);

    // short path first thread
    //spf_thread(mission_se);
    debug_msg("main.c : Starting short path first thread");
    start_thread(&t_spf, NULL, spf_thread, mission_se);
    // // wait for thread to execute
    end_thread(t_spf, NULL);
    for (int i = 0; i < mission_se->path->size; i++) {
        fprintf(f_path, "%d,%f,%f\n", mission_se->path->dest[i]->id,
                mission_se->path->dest[i]->x, mission_se->path->dest[i]->y);
    }
    *offsetPosition[0]=mission_se->path->dest[0]->x;
    *offsetPosition[1]=mission_se->path->dest[0]->y;
    preMission(mission_se->path,offsetPosition);


    debug_msg("main.c : Mission computed !");
    //simu->pos_sp.x = mission_se->path->dest[1]->x;
    //simu->pos_sp.y = mission_se->path->dest[1]->y;
    //simu->kalm_res.x = mission_se->path->dest[0]->x;
    //simu->kalm_res.y = mission_se->path->dest[0]->y;
    //
    
    
    
    // mission_se->path->dest[0]->x=0;
    // mission_se->path->dest[0]->y=0;

    // mission_se->path->dest[1]->x=10;
    // mission_se->path->dest[1]->y=10;
    
    // mission_se->path->dest[2]->x=0;
    // mission_se->path->dest[2]->y=10;
    
    // mission_se->path->dest[3]->x=10;
    // mission_se->path->dest[3]->y=0;
    
    // mission_se->path->dest[4]->x=5;
    // mission_se->path->dest[4]->y=15;
    
    // mission_se->path->dest[5]->x=0;
    // mission_se->path->dest[5]->y=0;
    
    
    // simu->pos_sp.x =  mission_se->path->dest[0]->x;
    // simu->pos_sp.y = mission_se->path->dest[0]->y;
    // simu->kalm_res.x = mission_se->path->dest[0]->x;
    // simu->kalm_res.y = mission_se->path->dest[0]->y;
    //
    // simu->p->x = mission_se->path->dest[0]->x+2;
    // simu->p->y = mission_se->path->dest[0]->y+3;
    // simu->p->theta =0;

    //

    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));

    //fprintf(f_path, "\nMission : id,x,y \n");
    
    // fprintf(f_tracking, "\niteration,x,y,theta");
    for (int iterate = 0; iterate < simu->max_iterations; iterate++) {
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

        //update_simulation(simu);
        start_thread(&t_simu, NULL, update_simulation, simu);
        end_thread(t_simu, NULL);
        
        if(simu->hach==simu->max_hach){
            simu->hach=0;
            fprintf(f_tracking, "%d,%f,%f,%f\n", iterate, simu->kalm_res.x+*offsetPosition[0], 
            simu->kalm_res.y+*offsetPosition[1], simu->kalm_res.theta);
        }
        simu->hach++;
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
