#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>

// -----------------Variables Globaux----------------


// --------------------------------------------------


int main(int argc, char const *argv[])
{
    //---------------[ jSon import ]---------------
	struct jdata * data ;
	data = importData(data);
	/*
	 * Affichage de test
	for(int i=0;i<data->occur->beacons;i++){
		printf("%f\n",(data->base->_b+i)->x);
    }
    */
    
    //---------------[TEST DIJKSTRA]---------------
    Cartography graphtest;
    Path trajectorytest;
    int src=0;
    int dest=125;
	
    graphtest.def_max_speed=0.3;
    graphtest.def_max_speed_up=0.5;
    graphtest.nb_nodes=data->occur->nodes;
    graphtest.nodes=malloc(data->occur->nodes*sizeof(Node));
    memcpy(graphtest.nodes,data->base->_nd,data->occur->nodes*sizeof(Node));
    
    for(int k=0;k<data->occur->nodes;k++){
        graphtest.nb_arcs+=(graphtest.nodes+k)->nb_a;
    }
    

    dijkstra(&graphtest, src, dest, &trajectorytest);

    for(int i=0;i<trajectorytest.size;i++){
        printf("%d\n",trajectorytest.dest[i]->id);
    }

    printf("DEBUG\n");
   
    return 0;
}


