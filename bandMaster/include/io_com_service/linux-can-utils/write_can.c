/*
 *  $Id$
 */

/*
 * cansend.c - simple command line tool to send CAN-frames via CAN_RAW sockets
 *
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <linux-can@vger.kernel.org>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <pthread.h>


#include "lib.h"
#include "../../twirtee.h"


// typedef struct can_shared{
// char* can_name;
// int id;
// uint *data[8];
// int available;
// pthread_mutex_t mutex;
// }


enum data_name {
	e_urgent_stop,
	e_theta,
	e_angular_speed_left,
	e_angular_speed_right,
	
};

void assign_id(int e_name,char* id){
	//char* id="000#";
	switch(e_name){
		case e_urgent_stop: 
			id="001#";break;
		case e_theta: 
			id="010#";break;
		case e_angular_speed_left: 
			id="011#";break;
		case e_angular_speed_right: 
			id="012#";break;
		default : 
			printf("write_can.c : error assigning id");
	}

}

void delay(int number_of_seconds) 
{ 
    int milli_seconds = 1000 * number_of_seconds; 
    // Stroing start time 
    clock_t start_time = clock(); 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
  
void float_can(char* value,float f_in,int id){
    uint32 payload;
    memcpy(&payload,&f_in,4);
    sprintf(value,"%d#%08x",id,payload);
    printf("CAN SEND : %s \t %f\n",value,f_in);
}


void* write_can(void* can_buffer)
{
    while(true)
    {
        //printf("write_can.c : another iteration in while loop\n");
        char* can_id="137#";
        char* payload_pipe[8];
        
        can_shared* can_buff = (can_shared* )can_buffer;
        pthread_mutex_lock(&can_buff->mutex);


        //    *(can_buff->data+sizeof(int)*1)=[1111111111111111];


        int s; /* can raw socket */ 
        int nbytes;
        struct sockaddr_can addr;
        struct can_frame frame;
        struct ifreq ifr;

        char payload[12];
        /* parse CAN frame */
        for(int i=0;i<can_buff->available;i++)
        {
            char* id="137#0000000000000000";
            //printf("write_can.c : Iteration in for loop\n");
            //sprintf(payload,"%s",can_buff->data[i]);
            //strcat(id,payload);
            //memcpy(payload,id,4);

            printf("write_can.c : buffer[%d] : %s\n",i,can_buff->data[i]);
            //if (parse_canframe(can_buff->data[i], &frame)){
            
            if (parse_canframe(can_buff->data[i], &frame)){
                printf("write_can.c : Eror can write %s\n",payload);
            }

            /* open socket */
            if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
                perror("socket");
                printf("write_can.c : Eror can write N2\n");;
            }

            addr.can_family = AF_CAN;

            strcpy(ifr.ifr_name, "can1");
            if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
                perror("SIOCGIFINDEX");
                printf("write_can.c : Eror can write N3\n");;
            }
            addr.can_ifindex = ifr.ifr_ifindex;

            /* disable default receive filter on this RAW socket */
            /* This is obsolete as we do not read from the socket at all, but for */
            /* this reason we can remove the receive list in the Kernel to save a */
            /* little (really a very little!) CPU usage.                          */
            setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

            if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("bind");
                printf("write_can.c : Eror can write N4\n");;
            }

            /* send frame */

            if ((nbytes = write(s, &frame, sizeof(frame))) != sizeof(frame)) {
                perror("write");
                printf("write_can.c : Eror can write N5\n" );;
            }

            //fprint_long_canframe(stdout, &frame, "\n", 0);

            close(s);
        }
        can_buff->available=0;
        pthread_mutex_unlock(&can_buff->mutex);
        delay(1000);
        //return 0;
    }
}
