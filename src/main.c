#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* json import */
    /** Counting structures */
    printf("%d\n", argc);
    int countlist[jsonTypes];
    countJson(countlist);
    struct Beacons B[countlist[0]];
    struct Node nd[countlist[1]];
    struct Constraints ct[countlist[2]];
    struct Waypoints Wp[countlist[3]];
    struct Legs lg[countlist[4]];
    struct Constants c;
    jsonMainExtract(B, nd, Wp, lg, ct, c);

    return 0;
}


