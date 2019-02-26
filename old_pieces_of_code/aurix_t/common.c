/*
 * common.c
 *
 *  Created on: 26 janv. 2018
 *      Author: GPH
 */

//#include "Serial.h"

#include "common.h"

void uint32_to_str(uint32 ui, char str[])
{
	int i = 0;

	for (i = 0; i < UINT32_STRLEN; ++i)
	{
		str[i] = '0';
	}
	str[UINT32_STRLEN] = '\0';

	i = UINT32_STRLEN - 1;
	while (ui >= BASE_10)
	{
		str[i] = '0' + (ui % BASE_10);
		ui = ui / BASE_10;
		--i;
	}
	str[i] = '0' + ui;
}

void uint64_to_str(uint64 ui, char str[])
{
	int i = 0;

	for (i = 0; i < UINT64_STRLEN; ++i)
	{
		str[i] = '0';
	}
	str[UINT64_STRLEN] = '\0';

	i = UINT64_STRLEN - 1;
	while (ui >= BASE_10)
	{
		str[i] = '0' + (ui % BASE_10);
		ui = ui / BASE_10;
		--i;
	}
	str[i] = '0' + ui;
}

void float32_to_str(float32 f, char str[])
{
	int i = 0;
	uint32 f_int = 0;
	uint32 f_dec = 0;

	str[0] = ' ';
	for( i = 1; i < (UINT32_STRLEN + 1); ++i)
	{
		str[i] = '0';
	}
	str[UINT32_STRLEN + 1] = '.';
	for (i = UINT32_STRLEN+2; i < FLOAT32_STRLEN; ++i)
	{
		str[i] = '0';
	}
	str[FLOAT32_STRLEN] = '\0';

	if (f < 0.0)
	{
		f_int = (uint32) (-1 * f)/1;
		f_dec = (uint32) ((f + f_int) * -1000000000);
		str[0] = '-';
	}
	else
	{
		f_int = (uint32) f/1;
		f_dec = (uint32) ((f - f_int) * 1000000000);
		str[0] = ' ';
	}

	i = UINT32_STRLEN;
	while (f_int >= BASE_10)
	{
		str[i] = '0' + (f_int % BASE_10);
		f_int = f_int / BASE_10;
		--i;
	}
	str[i] = '0' + f_int;

	i = FLOAT32_STRLEN - 1;
	while (f_dec >= BASE_10)
	{
		str[i] = '0' + (f_dec % BASE_10);
		f_dec = f_dec / BASE_10;
		--i;
	}
	str[i] = '0' + f_dec;
}

void sendUInt32_CPU2(uint32 ui)
{
	char str[UINT32_STRLEN + 1];
	uint32_to_str(ui, str);
	sendMsg_CPU2(str);
}

void sendUInt64_CPU2(uint64 ui)
{
	char str[UINT64_STRLEN + 1];
	uint64_to_str(ui, str);
	sendMsg_CPU2(str);
}

void sendFloat32_CPU2(float32 f)
{
	char str[FLOAT32_STRLEN + 1];
	float32_to_str(f, str);
	sendMsg_CPU2(str);
}

void sendEOL_CPU2()
{
	sendMsg_CPU2("\r\n");
}
