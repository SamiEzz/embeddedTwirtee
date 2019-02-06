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
 
#include "dwm_api.h"
#include "hal.h"
#include "twirtee.h"
#include "loc_thread.h"

      
int main(void)
{

   src=1;
   dest=4;
   
   T_loc * position = malloc(sizeof(T_loc));

   //---------- Creation des threads
   pthread_t t_localisation;
   if(pthread_create(&t_localisation, NULL, loc_thread, position) == -1) {
      perror("pthread_create");
      return EXIT_FAILURE;
   }  

   // wait for thread to execute 
   //void ** returned_path;

   if (pthread_join(t_localisation, NULL)) {
      perror("pthread_join");
      return EXIT_FAILURE;
   }
   // end of thread

   
   printf("- x : %d\n",position->x);
   printf("- y : %d\n",position->y);
   printf("- z : %d\n",position->z);
   printf("- qf : %d\n",position->qf);
       
   

   printf("fin du main\n");
   return EXIT_SUCCESS;
   return 0;
}
