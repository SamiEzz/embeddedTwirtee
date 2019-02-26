#ifndef CPU2_MISC_H
#define CPU2_MISC_H

#define MAX_LINE_SIZE 80
#define MAX_UI_STRLEN 5
#define BASE_10 10
#define MAX_INT_STRLEN 11

#define FREQ 100 ///< calls per second
#define FREQ_LED FREQ/20 ///< every 50 ms
#define FREQ_SEND FREQ/10
#define FREQ_PID 1
#define FREQ_PRINT (FREQ/FREQ_PID)

void ui_to_str(unsigned int ui, int start, char msg[MAX_LINE_SIZE]);
void int_to_str(int in, int start, char msg[MAX_LINE_SIZE]);

typedef enum {
	LEFT = 0,
	RIGHT
} SIDE;

#endif
