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
    struct Beacons B[8];
    struct Node nd;
    struct Waypoints Wp;
    struct Legs lg;
    struct Constraints ct;
    struct Constants c;
    jsonMainExtract(B, **nd, **Wp, **lg, **ct, *_c);

    return 0;
}
