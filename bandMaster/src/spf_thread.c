//////////////////////////////////////////////////////////
//                                                      //
//      (c)         : EZZEROUALI Sami 2018              //
//      Tutor       : JENN Eric                         //
//      Project     : CAPHCA                            //
//      Program     : SPF - Shortest path first         //
//                                                      //
//////////////////////////////////////////////////////////

#include "../include/spf_thread.h"
#include "../include/dijkstra.h"
#include "../include/jsonApi.h"
#include "../include/misc.h"
#include "../include/threads_mgr.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strerror

#define outputFile "output.nodes"

void* get_mission_thread(void* mission_se) {
    // spf_mission *mission = mission_se;
    int src, dest;
    // mission = (spf_mission *)mission;
    spf_mission* mission = mission_se;

    pthread_mutex_lock(&mission->mut);
    printf("\nSaisissez le point de depart suivie du point d arrivee : ");
    scanf("%d %d", &src, &dest);

    mission->start = src;
    mission->end = dest;

    pthread_mutex_unlock(&mission->mut);

    pthread_exit(NULL);
}

void* spf_thread(void* mission_se) {
    debug_msg("spf_thread.c : starting thread");
    spf_mission* mission = mission_se;
    int src, dest;
    // printf("%s , %s , %d",argv[0],argv[1],argc);

    src = mission->start;
    dest = mission->end;

    //=====================[ jSon IMPORT ]=====================
    struct jdata* data = safe_alloc(sizeof(jdata*));
    /* importData()
     *  this function import information from the json file in "jsonApi.h"->jsonFileName.
     *  then store it in the variable "data"(1), wich contain tables in the subVariable data->base
     *  and store the size of each element in data->occur.
     *  (1)-Check "struct jdata" in jsonApi.h
     */
    debug_msg("spf_thread.c : import data");
    importData(data);
    
    //=====================[TEST DIJKSTRA]=====================
    Path trajectorytest;
    Cartography* graphtest = safe_alloc(sizeof(Cartography));

    /* initCarto()
     * this function use "jdata structure" to assign
     * - the Cartography nodes,
     * - speed and speedup limitations from "dijkstra.h" defined variables
     * - number of arcs in the cartography
     *
     */
    initCarto(graphtest, data->base->_nd, data->occur->nodes);

    /* dijkstra()
     * this is an implementation of Dijkstra's algorithm to find the Shortest path
     * betwin "int src" and "int dest" which are the ids of nodes we want to go from
     * and arrive to.
     *
     */
    mission->path = safe_alloc(sizeof(Path));
    dijkstra(graphtest, src, dest, mission->path);
    //def_speed_()
    
    mission->path->def_max_speed=data->base->_c.vdef;
    mission->path->def_max_speed_up =data->base->_c.adef;
    for(int p=0;p<mission->path->size;p++){
        for(int s=0;s<mission->path->dest[p]->nb_a;s++){
            mission->path->dest[p]->arcs[s].max_speed=mission->path->def_max_speed;
            mission->path->dest[p]->arcs[s].max_speed_up=mission->path->def_max_speed_up;
        }
    }
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
