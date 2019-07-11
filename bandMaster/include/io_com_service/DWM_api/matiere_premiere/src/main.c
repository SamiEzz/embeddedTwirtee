/*! ------------------------------------------------------------------------------------------------------------------
 * @file    tag_cfg.c
 * @brief   Decawave device configuration and control functions
 *
 * @attention
 *
 * Copyright 2017 (c) Decawave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>

#include "../loc_api/loc_thread.h"

// typedef struct T_loc {
// 	double x;
// 	double y;
// 	double z;
// 	double qf;
// 	bool val;
// } T_loc;



//static T_loc * position;
int main(){
   //printf("init loc thread");
   dwm_pos_t p;
    p.x=6;
    p.y=6;
    p.z=1995;
   
   dwm_pos_t* position=&p;
   //---------- Creation des threads
   printf("- x : %d\n",position->x);
   printf("- y : %d\n",position->y);
   printf("- z : %d\n",position->z);
   printf("- qf : %d\n",position->qf);
   pthread_t t_localisation;
   if(pthread_create(&t_localisation, NULL, loc_thread, (void *)position) == -1) {
      perror("pthread_create");
      return EXIT_FAILURE;
   }  
   
   //loc_thread(position);

   // wait for thread to execute 
   //void ** returned_path;

   // if (pthread_join(t_localisation, (void *)position)) {
   //    perror("pthread_join");
   //    return EXIT_FAILURE;
   // }
   // end of thread

   while(1){   
   printf("- x : %d\n",position->x);
   printf("- y : %d\n",position->y);
   printf("- z : %d\n",position->z);
   printf("- qf : %d\n",position->qf);
   HAL_Delay(1000);    
   }
   

   printf("fin du main\n");
   
   return 0;
}
