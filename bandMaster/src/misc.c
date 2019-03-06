#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


void * safe_alloc(int size){
    //errno = 0;
    void * memblock;
    memblock = malloc(size);
    if(memblock==NULL){
        return NULL;
    }
    else{
        return memblock;
    }
}
