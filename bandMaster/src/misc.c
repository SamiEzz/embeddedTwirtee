
#include "../include/misc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void* safe_alloc(int size) {
    // errno = 0;
    void* memblock;
    memblock = malloc(size);
    if (memblock == NULL) {
        return NULL;
    } else {
        return memblock;
    }
}

void debug_msg(char* msg) {
    if (debug == 1) {
        printf("\n");
        printf("%s", msg);
        printf("\n");
    } else {
    }
}

void preMission(Path* p,float x,float y){
    debug_msg("main.c : preMission start");
    x=p->dest[0]->x;
    y=p->dest[0]->y; 
    for(int t=0;t<p->size;t++){
        p->dest[t]->x-=x;
        p->dest[t]->y-=y;
    }    
}
void postMission(Path* p,float* offset){
    for(int t=0;t<p->size;t++){
        p->dest[t]->x+=offset[0];
        p->dest[t]->y+=offset[1];
    }    
}