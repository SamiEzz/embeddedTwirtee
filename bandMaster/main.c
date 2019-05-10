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
#include <math.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define _CAN_MSG_LEN 8

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
void gen_can_message(char* output,int i){
    unsigned int payload;
    char base[5][13]={"00112233","12345678","ABCDEF","DEADBEEF","99999999"};
    float f_base[]={9.81f,1.6184f,3.14f,2.74f,1.609f};
    memcpy(&payload,&f_base[i],4);
    sprintf(output,"137#%08x",payload);
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

    union {
        float f_data;
        unsigned int i_data[4];
    }caster;
    // char* can_name[]={"read_can", "can1"};
    // start_thread(&t_can_read, NULL, read_can, can_name);
    // // wait for thread to execute
    // end_thread(t_can_read, NULL);
    
    can_shared* can_buff = safe_alloc(sizeof(can_shared));
    can_buff->available=0;
    start_thread(&t_can_write, NULL, write_can, can_buff);
    // wait for thread to execute
    char k;
    char test_payload[12];
    int i = 0;

        
    while(!true){ // send data over can loop
        pthread_mutex_lock(&can_buff->mutex);
        gen_can_message(test_payload,can_buff->available);
        memcpy(&test_payload,&can_buff->data[can_buff->available],4);
        printf("Payload : %hhn\n",can_buff->data[can_buff->available]);
        can_buff->available++;
        pthread_mutex_unlock(&can_buff->mutex);
        my_delay(2);
    }
    start_thread(&t_can_read, NULL, read_can, can_buff);
    while(true){
        printf("main.c : inside loop");
        pthread_mutex_lock(&can_buff->mutex);
        if(can_buff->available>0){
            for(int i=0;i<=can_buff->available;i++){
                printf("\nid \t message \n");
                printf("%s \t ",can_buff->data[i]);
                //printf("\n\n\n");
            }
            can_buff->available=0;
        }
        pthread_mutex_unlock(&can_buff->mutex);
        my_delay(2);    

    }
    
    end_thread(t_can_read, NULL);
    
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
