
#ifndef T_READ_CAN_
#define T_READ_CAN_

void print_usage(char *prg);
void sigterm(int signo);
int idx2dindex(int ifidx, int socket);
void* read_can(void* can);

#endif

