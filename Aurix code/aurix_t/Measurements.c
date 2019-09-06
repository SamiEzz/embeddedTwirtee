/*
 * Measurements.c
 *
 *  Created on: 12 oct. 2018
 *      Author: twirteeop
 */

#include <IfxCbs_reg.h>
#include <IfxCpu.h>
#include <IfxCpu_reg.h>

#include "Measurements.h"

void initMeasurements(void)
{
	/* Enable OCDS */
	CBS_OEC.U = 0xa1;
	CBS_OEC.U = 0x5e;
	CBS_OEC.U = 0xa1;
	CBS_OEC.U = 0x5e;

	Ifx_CPU_CCTRL cctrl;
	cctrl.U    = __mfcr(CPU_CCTRL);
	/* Disable the counters */
	cctrl.B.CE = 0;
	/* Configure general counters (1 to 3) */
	cctrl.B.M1 = 1; // Counter1 -> PCache hits
	cctrl.B.M2 = 1; // Counter2 -> PCache misses
	cctrl.B.M3 = 0;
//	__asm__ volatile ("dsync\n":::"memory");
	__mtcr(CPU_CCTRL, cctrl.U);
//	__asm__ volatile ("isync\n":::"memory");
}

void startMeasurements(void)
{
	IfxCpu_resetAndStartCounters(IfxCpu_CounterMode_normal);
}

void stopMeasurements(t_measurements *measurements)
{
	IfxCpu_Perf perfCounts = IfxCpu_stopCounters();

	measurements->instructions 	= perfCounts.instruction.counter;
	measurements->cpu_cycles 	= perfCounts.clock.counter;
	measurements->pcache_hits 	= perfCounts.counter1.counter;
	measurements->pcache_misses = perfCounts.counter2.counter;
}
