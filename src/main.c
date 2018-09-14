#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>


int main(int argc, char const *argv[])
{
    //---------------[ jSon import ]---------------
	struct jdata * data ;
	data = importData(data);
    
    //---------------[TEST DIJKSTRA]---------------
    Path trajectorytest;
    Cartography * graphtest = malloc(sizeof(Cartography));

    int src=0;
    int dest=125;
    initCarto(graphtest, data->base->_nd,data->occur->nodes);
    dijkstra(graphtest, src, dest, &trajectorytest);

    //---------------[ PRINT RESULT ]---------------
    for(int i=0;i<trajectorytest.size;i++){
        Node n = getnodebyid(data,trajectorytest.dest[i]->id);
        printnode(&n);
    }

   
    return 0;
}


