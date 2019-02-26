#include "Misc.h"

inline int abs_int(int i) {
	return i >= 0 ? i : -i;
}

inline int min_int(int i, int j) {
	return i <= j ? i : j;
}

/*****
 * @brief Convert unsigned int to char[5] and place it in msg[start]
 * for instance, convert 1 -> "00001"
 * @param IN  ui unsigned int to convert
 * @param IN  start indice where start the writing of ui
 * @param OUT msg in which insert ui
 * @assert strlen(msg) < (MAX_LINE_SIZE - MAX_UI_STRLEN),
 * where MAX_UI_STRLEN = strlen("65000") = 5;
*/
void ui_to_str(unsigned int ui, int start, char msg[MAX_LINE_SIZE]) {
	int i;
	/** 1. fill MAX_UI_STRLEN chars with '0' */
	for (i = start; i < (start + MAX_UI_STRLEN); ++i)
		msg[i] = '0';
	/** 2. use end of msg to convert in base 10 */
	i = MAX_LINE_SIZE - 1;
	while (ui >= BASE_10) {
		msg[i--] = '0' + (ui % BASE_10);
		ui /= BASE_10;
	}
	msg[i] = '0' + ui;
	/** 3. set start corresponding to the good power of 10 */
	start += (MAX_UI_STRLEN - 1) - (MAX_LINE_SIZE - 1 - i);
	/** 4. copy str of ui stored at the end of msg at the right place */
	while (i != MAX_LINE_SIZE)
		msg[start++] = msg[i++];
}

void int_to_str(int in, int start, char msg[MAX_LINE_SIZE]) {
	char str_i[MAX_INT_STRLEN] = "00000000000";
	int i, stop;
	if (in < 0)
		str_i[0] = '-';
	in = abs_int(in);
	for (i = MAX_INT_STRLEN - 1; in >= BASE_10; --i) {
		str_i[i] = '0' + (in % BASE_10);
		in /= BASE_10;
	}
	str_i[i] = '0' + in;
	i = 0;
	stop = start + MAX_INT_STRLEN;
	while (start < stop)
		msg[start++] = str_i[i++];
}
