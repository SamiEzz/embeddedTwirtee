#include "../include/header.h"
#include "../include/config.h"
#include "../include/jsonApi.h"

#include <stdlib.h>
#include <stdio.h>

// -----------------Variables Globaux----------------


// --------------------------------------------------


int main(int argc, char const *argv[])
{
	struct jdata * data ;
	data = importData(data);
	for(int i=0;i<data->occur->beacons;i++){
		printf("%f\n",(data->base->_b+i)->x);
    }
    printf("DEBUG\n");
    
    return 0;
}


