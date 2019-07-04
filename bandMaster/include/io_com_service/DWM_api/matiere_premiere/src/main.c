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
}
