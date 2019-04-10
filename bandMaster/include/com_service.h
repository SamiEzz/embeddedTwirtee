#ifndef COM_SERVICE_
#define COM_SERVICE_

#include "twirtee.h"



typedef struct medium{
    /*
    0 : CAN
    1 : I2C
    2 : SPI
    3 : GPIO
    4 : ETHERNET
    5 : WIFI
    6 : NONE
    */
    int type; 
    int speed;
    int adresse;
}medium;

typedef struct table_de_configuration{
    unsigned int id;
    medium m;    
    int send_freq;

}table_de_configuration;












#endif /* COM_H_ */
