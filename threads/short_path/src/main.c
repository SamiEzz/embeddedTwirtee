//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"
#include "../include/spf_thread.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>
#include <pthread.h>




#define outputFile "output.nodes"


int main(int argc, char const *argv[])
{
    int src,dest;
    Path trajectorytest;
    // printf("%s , %s , %d",argv[0],argv[1],argc);
    
    if(argc==3){
        src = atoi(argv[1]);
        dest= atoi(argv[2]);
    }
    else if(argc==2 && argv[2]=="--help"){
        (void)fprintf(stderr,"Usage: main  \n");
    }
    else {
        errno = 0;
        (void)fprintf(stderr,"TwIRTee: no start and end point specified\nTwIRTee: Use --help for more information \n");
        return errno;
        src=0;
        dest=5;
    };
    spf_mission mission_se={
        .start = src,
        .end = dest
    };

    //---------- Creation des threads
    pthread_t t_spf;
    if(pthread_create(&t_spf, NULL, spf_thread, &mission_se) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }  

    // wait for thread to execute 
    if (pthread_join(t_spf, NULL)) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }
    // end of thread

    for(int i=0;i<trajectorytest.size;i++){
        printf("- %d\n",trajectorytest.dest[i]->id);    
    }
    printf("fin du main\n");
    return EXIT_SUCCESS;

    return 0;
}


