#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* json import to Base */
    struct Base base = initBase();
    jsonMainExtract(base,jsonFileName);

    printf("DEBUG\n");
    
    return 0;
}


