
#ifndef T_READ_CAN_
#define T_READ_CAN_
#include "../io_com_service.h"

void print_usage(char *prg);
void sigterm(int signo);
int idx2dindex(int ifidx, int socket);
int read_can(void* can);

#endif
