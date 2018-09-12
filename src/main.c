#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"
#include "../include/dijkstra.h"

#include <stdlib.h>
#include <stdio.h>

// -----------------Variables Globaux----------------


// --------------------------------------------------


int main(int argc, char const *argv[])
{
    //---------------[jSon import to data]---------------
	struct jdata * data ;
	data = importData(data);
	for(int i=0;i<data->occur->beacons;i++){
		printf("%f\n",(data->base->_b+i)->x);
    }
    
    
    //---------------[TEST DIJKSTRA]---------------
    Cartography graphtest;
    Path trajectorytest;
    int src=0;
    int dest=125;

    graphtest.def_max_speed=0.3;
    graphtest.def_max_speed_up=0.5;
    graphtest.nb_nodes=data->occur->nodes;
    
    for(int i=0;i<data->occur->nodes;i++){
        graphtest.nodes[i]=&(data->base->_nd+i);
    }
    for(int k=0;k<data->occur->nodes;k++){
        graphtest.nb_arcs+=graphtest.nodes[k].nb_a;
        //printf("\nnb_arcs : %d\n",graphtest.nodes[k].nb_a);
    }
    //printf("\nnb_arcs : %d\n",graphtest.nb_arcs);
    

    dijkstra(&graphtest, src, dest, &trajectorytest);

    for(int i=0;i<trajectorytest.size;i++){
        printf("%d\n",trajectorytest.dest[i]->id);
    }

    printf("DEBUG\n");
   
    return 0;
}


