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
unsigned int gen_can_message(unsigned int payload,int i){
    //strncpy(temp_data_buffer,&can_buff->data,can_buff->available*sizeof(char)*8);
    //strcat(can_buff->data,payload);
    char base[5][13]={"00112233","12345678","ABCDEF","DEADBEEF","99999999"};
    // payload = base[i];
    float f_base[]={9.81f,1.6184f,3.14f,2.74f,1.609f};
    //ftoa(f_base[i],payload,3);
    //payload = (unsigned char*)&f_base[i];
    //char id[12]="137#";
    memcpy(&payload,&f_base[i],4);
    // strcat(id,payload);


    // union {
    //     float f_data;
    //     unsigned char c_data[sizeof(float)]
    // }cast;
    // cast.f_data=f_base[i];
    // payload=cast.c_data;
    
    return payload;
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
    unsigned int test_payload;
    int i = 0;
    caster.f_data=3.14f;
    memcpy (&test_payload, &caster.f_data, 4);  
    //test_payload = (unsigned int) caster.f_data;
    printf("casted float : %08x %f\n",test_payload,caster.f_data);
    

    while(true){
        pthread_mutex_lock(&can_buff->mutex);
        test_payload=gen_can_message(test_payload,can_buff->available);
        //printf("can_gen [%d] : %s\n",can_buff->available,test_payload);
        //printf("Enter message to send : ");
        //fgets(can_buff->data[can_buff->available],12,stdin);
        //printf("\n");
        memcpy(&test_payload,&can_buff->data[can_buff->available],4);
        // strncpy(can_buff->data[can_buff->available],test_payload,_CAN_MSG_LEN*sizeof(char));
        
        printf("Payload : %x\n",can_buff->data[can_buff->available]);
        can_buff->available++;
        pthread_mutex_unlock(&can_buff->mutex);
        my_delay(2);
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
