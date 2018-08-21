
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/jsmn.h"
#include "../include/header.h"




int main(int argc, char const *argv[])
{   
    struct Legs legs;

    struct Node N = convertLegs(Legs legs);

    bool check=4==4||1==2;
    printf("resp %d\n",check);
    return 0;
}
