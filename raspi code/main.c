#include "./include/misc.h"
#include "./include/threads_mgr.h"
#include "./include/guidance_mgr.h"
#include "./include/simulator.h"
#include "./include/speed_selection.h"

#include "./include/localization.h"
#include "./include/mission_mgr.h"

#include "./include/measurement_randomization.h"
#include "./include/spf_thread.h"
#include "./include/io_com_service/io_com_service.h"
#include "./include/io_com_service/linux-can-utils/read_can.h"
#include "./include/io_com_service/linux-can-utils/write_can.h"
#include "./include/io_com_service/DWM_api/matiere_premiere/loc_api/loc_thread.h"

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
    
   T_loc p;
   T_loc* position=&p;
   //---------- Creation des threads
   pthread_t t_localisation;
   if(pthread_create(&t_localisation, NULL, loc_thread, (void *)position) == -1) {
      perror("pthread_create");
      return EXIT_FAILURE;
   }  
   
   printf("- x : %f\n",position->x);
   printf("- y : %f\n",position->y);
   printf("- z : %f\n",position->z);
   printf("- qf : %f\n",position->qf);

   // wait for thread to execute 
   //void ** returned_path;

   if (pthread_join(t_localisation, (void *)position)) {
      perror("pthread_join");
      return EXIT_FAILURE;
   }
   // end of thread

   
   printf("- x : %f\n",position->x);
   printf("- y : %f\n",position->y);
   printf("- z : %f\n",position->z);
   printf("- qf : %f\n",position->qf);
       
   

   printf("fin du main\n");
   
   return 0;
//    io_service_main();
}
int main_old() {

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
    pthread_t t_localisation;
    pthread_t t_get_mission;
    pthread_t t_spf;
    pthread_t t_io_service;

    pthread_t t_simu;
    //pthread_t t_can_write;
    
    //###############################################
    //              Main routine                    #
    //###############################################
    // CAN PROTOCOLE READ

    //char* can_name[]={"read_can", "can1"};
    //start_thread(&t_can_read, NULL, read_can, can_name);
    // wait for thread to execute
    //end_thread(t_can_read, NULL);
    

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
    debug_msg("main.c : Out from spf_thread");
    for (int i = 0; i < mission_se->path->size; i++) {
        fprintf(f_path, "%d,%f,%f\n", mission_se->path->dest[i]->id,
                mission_se->path->dest[i]->x, mission_se->path->dest[i]->y);
    }
    float _mx=mission_se->path->dest[0]->x;
    float _my=mission_se->path->dest[0]->y;
    preMission(mission_se->path,_mx, _my);


    debug_msg("main.c : Mission computed !");
    for (int i = 0; i < mission_se->path->size; i++) {
        printf("-> %d\n", mission_se->path->dest[i]->id);
    }

    simu->odometry = (T_odo*)safe_alloc(sizeof(T_odo));

    //fprintf(f_path, "\nMission : id,x,y \n");
    
    // fprintf(f_tracking, "\niteration,x,y,theta");
    debug_msg("main.c :  Starting Simulation loop");
    T_head _mcompass;
    T_loc _mgps;
    T_odo _modo;
    for (int iterate = 0; iterate < simu->max_iterations; iterate++) {
        _mcompass=*simu->compass;
        _mgps=*simu->gps;
        _modo=*simu->odometry;
        debug_msg("main.c : iteration inside simulation");
        // com_localize(compass, gps, odometry, &kalman_position, &Q);		///< Kalman call for
        // better position estimation from noised data
        com_localize(_mcompass,_mgps,_modo, &simu->kalm_res, &Q);
	    debug_msg("main.c : com_localise done..");
        // com_generation(kalman_position, &path, &position_sp); ///< defines SetPoint
        com_generation(simu->kalm_res, mission_se->path, &simu->pos_sp);
	    debug_msg("main.c :  com_generation done..");
        // printf("x:%f, y: %f\n",simu->pos_sp.x,simu->pos_sp.y);
        // com_tracking(kalman_position, position_sp, &Q, &s_gui); ///< Kanayama call to track the
        // defined SetPoint
        com_tracking(simu->kalm_res, simu->pos_sp, &Q, &simu->speed_gui);
	    debug_msg("main.c : com_tracking done.."),
        // com_speed_selection(s_gui, s_rec, s_safety, s_arp, &s_out);	///< Shall select the
        // biggest speed asserting s_out <= min(s_safety, s_arp)
        com_speed_selection(simu->speed_gui, s_rec, s_safety, s_arp, &simu->speed_out);
	    debug_msg("main.c :  com_speed_select done");
        //update_simulation(simu);
        start_thread(&t_simu, NULL, update_simulation, simu);
        end_thread(t_simu, NULL);
        debug_msg("main.c : simulation thread done ..");
        if(simu->hach==simu->max_hach){
            simu->hach=0;
            fprintf(f_tracking, "%d,%f,%f,%f\n", iterate, simu->kalm_res.x+_mx, 
            simu->kalm_res.y+_my, simu->kalm_res.theta);
        }
	
        simu->hach++;
    }
    debug_msg("main.c :  End of simulaiton loop");
    //###############################################
    //              show results                    #
    //###############################################

    
    debug_msg("Executed !");
    //###############################################
    //                  close                       #
    //###############################################
    return EXIT_SUCCESS;
}
