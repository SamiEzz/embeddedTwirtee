
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
