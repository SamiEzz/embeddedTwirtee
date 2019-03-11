#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strerror
#include <math.h>
#include <errno.h>

#define outputFile "output.txt"


int main(int argc, char const *argv[])
{
    int src,dest;
    // printf("%s , %s , %d",argv[0],argv[1],argc);
    
    if(argc==3){
        src = atoi(argv[1]);
        dest= atoi(argv[2]);
    }
    else {
        src=0;
        dest=5;
    };
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
    dijkstra(graphtest, src, dest, &trajectorytest);

    //=================[ PRINT THE SHORTEST PATH ]=================
    for(int i=0;i<trajectorytest.size;i++){
        Node n = getnodebyid(data,trajectorytest.dest[i]->id);
        printnode(&n);
        traject_to_file(&n,i,outputFile);
    }
    
    //=====================[ Tracking Robot ]=====================

    return 0;
}


