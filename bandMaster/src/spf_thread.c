//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////


#include "../include/jsonApi.h"
#include "../include/dijkstra.h"
#include "../include/spf_thread.h"
#include "../include/threads_mgr.h"
#include "../include/misc.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>

#define outputFile "output.nodes"

void* get_mission_thread (void * mission_se)
{
    //spf_mission *mission = mission_se;
    int src,dest;
    //mission = (spf_mission *)mission;
    spf_mission * mission =  mission_se;

    pthread_mutex_lock(&mission->mut);
    printf("\nSaisissez le point de depart suivie du point d arrivee : ");
    scanf("%d %d",&src,&dest);

    mission->start = src;
    mission->end = dest;
       

    pthread_mutex_unlock(&mission->mut);

    pthread_exit(NULL);

}
 
void *spf_thread(void *mission_se){
    spf_mission * mission =  mission_se;
    int src,dest;
    // printf("%s , %s , %d",argv[0],argv[1],argc);
    
    src=mission->start;
    dest=mission->end;
    
    //=====================[ jSon IMPORT ]=====================
    struct jdata * data = safe_alloc(sizeof(jdata *));
    /* importData()
    *  this function import information from the json file in "jsonApi.h"->jsonFileName.
    *  then store it in the variable "data"(1), wich contain tables in the subVariable data->base
    *  and store the size of each element in data->occur.
    *  (1)-Check "struct jdata" in jsonApi.h
    */
	importData(data);
    
    //=====================[TEST DIJKSTRA]=====================
    Path trajectorytest;
    Cartography * graphtest = safe_alloc(sizeof(Cartography));

    
    
    /* initCarto()
    * this function use "jdata structure" to assign 
    * - the Cartography nodes, 
    * - speed and speedup limitations from "dijkstra.h" defined variables
    * - number of arcs in the cartography
    *     
    */
    initCarto(graphtest, data->base->_nd,data->occur->nodes);

    /* dijkstra()
    * this is an implementation of Dijkstra's algorithm to find the Shortest path
    * betwin "int src" and "int dest" which are the ids of nodes we want to go from 
    * and arrive to.
    *     
    */
    mission->path = safe_alloc(sizeof(Path));
    dijkstra(graphtest, src, dest, mission->path);

    //=================[ PRINT THE SHORTEST PATH ]=================
    /*
    for(int i=0;i<trajectorytest.size;i++){
        Node n = getnodebyid(data,trajectorytest.dest[i]->id);
        printnode(&n);
        traject_to_file(&n,i,outputFile);
    }
    */
    //=====================[ Tracking Robot ]=====================
    pthread_exit(NULL);
}

