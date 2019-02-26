/*
 * common.h
 *
 *  Created on: 26 janv. 2018
 *      Author: GPH
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "Cpu_Common.h"

#define Approx1SecCount	40000000U
#define Approx10MSecCount	400000U

//#define MAX_LINE_SIZE 80
#define UINT32_STRLEN 10
#define UINT64_STRLEN 20
#define FLOAT32_STRLEN ((2 * UINT32_STRLEN - 1) + 2)
#define BASE_10 10

void uint32_to_str(uint32 ui, char str[]);
void uint64_to_str(uint64 ui, char str[]);
void float32_to_str(float32 f, char str[]);

void sendUInt32_CPU2(uint32 ui);
void sendUInt64_CPU2(uint64 ui);
void sendFloat32_CPU2(float32 f);
void sendEOL_CPU2();

#endif /* COMMON_H_ */
