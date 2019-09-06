/*
 * Measurements.h
 *
 *  Created on: 12 oct. 2018
 *      Author: twirteeop
 */

#ifndef MEASUREMENTS_H_
#define MEASUREMENTS_H_

typedef struct
{
	uint32 instructions;
	uint32 cpu_cycles;
	uint32 pcache_hits;
	uint32 pcache_misses;
} t_measurements;

void initMeasurements(void);
void startMeasurements(void);
void stopMeasurements(t_measurements *measurements);

#endif /* MEASUREMENTS_H_ */
