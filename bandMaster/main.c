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
#include "./include/twirtee.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int add_to_can_buffer(char* payload,can_shared* can_buff){
    //strncpy(temp_data_buffer,&can_buff->data,can_buff->available*sizeof(char)*8);
    //strcat(can_buff->data,payload);
    return 0;
}

int main() {

    //###############################################
    //                 output File                  #
    //###############################################

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
    while(true){

        //char payloads[100][8];
        scanf("Write can message %s\n",can_buff->data[can_buff->available]);
        printf("Payload : %s\n",can_buff->data[can_buff->available]);

        // else{
        //     printf("Eror adding payload to buffer, return : %d\n",add_to_can_buffer(payload,can_buff));
        // }
        

    }
    
    end_thread(t_can_write, NULL);
    
    
    // Simulation thread


    //###############################################
    //              show results                    #
    //###############################################
    //###############################################
    //                  close                       #
    //###############################################
    return EXIT_SUCCESS;
}
