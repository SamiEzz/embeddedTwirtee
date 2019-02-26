/******************************************************************************
 * \file		Cpu2_main.c
 * \ingroup	 IRT - CAPHCA
 * \brief	   XXXX.
 * \details
 * \author		Guillaume PHAVORIN
 * \date		\$LastChangedDate:: 2018-06-20
 * \version	 \$Revision: 1
 * \attention   -
 *****************************************************************************/


//#define IFX_INTPRIO_CCU6 1 ///< priorities are normally defined in Ifx_IntPrioDef.h
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Cpu_Common.h"

#include "Ifx_Cfg_CStart.h"
#include "SysSe/Bsp/Bsp.h"
#include "CPU2/app_Serial.h"
#include "common.h"
#include <stdio.h>

#include "twirtee.h"
#include "parametres.h"
#include "mission_mgr.h"
#include "localization.h"
#include "guidance_mgr.h"
#include "speed_selection.h"

#include "simulator.h"
#include "measurement_randomization.h"

#ifdef PERFCOUNTERS
#include "Measurements.h"
#endif


#define Approx1SecCount		40000000U
#define Approx10MSecCount	400000U

#define MAX_LINE_SIZE 80


static uint32 volatile DelayLoopCounter;

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------
App_Cpu g_AppCpu2; // CPU 2 global data

/* Program "INPUTS" */
extern Cartography input_cartography;
Mission input_mission;
extern unsigned int input_max_sample_nb;
extern Randomization_Parameters input_random_param;
/* Program "OUTPUTS" */
Mission mission;
Path path;
Position actual_position, kalman_position, position_sp;
T_head compass;
T_loc gps;
T_odo odometry;
Speed s_gui;
Speed s_out;
#ifndef PERFCOUNTERS
uint64 cpu_ticks;
#else
t_measurements measurements;
#endif
unsigned int iterate = 0;


inline void init_CPU2(void) {
	// Initialize CPU
	//-----------------
	g_AppCpu2.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu2.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreId());
	g_AppCpu2.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu2.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);
    Ifx_CPU_PCON0 pcon0;
    pcon0.U = __mfcr(CPU_PCON0);
	g_AppCpu2.info.pCache = pcon0.B.PCBYP?FALSE:TRUE;
	Ifx_CPU_DCON0 dcon0;
	dcon0.U = __mfcr(CPU_DCON0);
	g_AppCpu2.info.dCache = dcon0.B.DCBYP?FALSE:TRUE;

	/* for serial communication */
	IfxScuCcu_setPll2Frequency(120000000U);
	IfxScuCcu_selectAdcClock(IfxScuCcu_AdcClockSelection_fpll2);
}

void sendSetupInfo()
{
#ifdef WITH_FOC
	typedef float32 StdReal;
	extern const StdReal sinCosTable[1280];
	sendMsg_CPU2("CONTENTION mode -> lookuptables in ");
	if (((sinCosTable >= 0x80000000) && (sinCosTable < 0x80200000)) || ((sinCosTable >= 0xA0000000) && (sinCosTable < 0xA0200000)))
	{
		sendMsg_CPU2("PFLS0");
	}
	else if (((sinCosTable >= 0x80200000) && (sinCosTable < 0x80400000)) || ((sinCosTable >= 0xA0200000) && (sinCosTable < 0xA0400000)))
	{
		sendMsg_CPU2("PFLS1");
	}
	else if ((sinCosTable >= 0xAF000000) && (sinCosTable < 0xAF110000))
	{
		sendMsg_CPU2("DFLS0");
	}
	else if ((sinCosTable >= 0x70000000) && (sinCosTable < 0x7001C000))
	{
		sendMsg_CPU2("DSRAM0");
	}
	else if ((sinCosTable >= 0x60000000) && (sinCosTable < 0x6001E000))
	{
		sendMsg_CPU2("DSRAM1");
	}
	else if ((sinCosTable >= 0x50000000) && (sinCosTable < 0x5001E000))
	{
		sendMsg_CPU2("DSRAM2");
	}
	else
	{
		sendMsg_CPU2("somewhere else");
	}
	sendEOL_CPU2();
#else
		sendMsg_CPU2("ISOLATION mode\r\n");
#endif

	sendMsg_CPU2("CPU_FREQ= ");
	sendFloat32_CPU2(g_AppCpu2.info.cpuFreq);
	sendEOL_CPU2();
	sendMsg_CPU2("STM_FREQ= ");
	sendFloat32_CPU2(g_AppCpu2.info.stmFreq);
	sendEOL_CPU2();
#ifndef PERFCOUNTERS
	sendMsg_CPU2("STM overhead= ");
	sendUInt32_CPU2(STM_OVERHEAD);
	sendEOL_CPU2();
#endif

	sendMsg_CPU2("PCache2=");
	sendMsg_CPU2(g_AppCpu2.info.pCache?"ENABLED":"DISABLED");
	sendMsg_CPU2(", DCache2=");
	sendMsg_CPU2(g_AppCpu2.info.dCache?"ENABLED":"DISABLED");
	sendEOL_CPU2();

	sendMsg_CPU2("simulation parameters:");
	sendEOL_CPU2();
	sendMsg_CPU2("\tGPS: INV=");
	sendFloat32_CPU2(input_random_param.invalid_gps_data);
	sendMsg_CPU2("%, NOIS=");
	sendFloat32_CPU2(input_random_param.noised_gps_data);
	sendMsg_CPU2("%, AMP=");
	sendFloat32_CPU2(input_random_param.gps_noise_amplitude);
	sendMsg_CPU2("m");
	sendEOL_CPU2();
	sendMsg_CPU2("\tCOMP: INV=");
	sendFloat32_CPU2(input_random_param.invalid_compass_data);
	sendMsg_CPU2("%, NOIS=");
	sendFloat32_CPU2(input_random_param.noised_compass_data);
	sendMsg_CPU2("%, AMP=");
	sendFloat32_CPU2(input_random_param.compass_noise_amplitude);
	sendMsg_CPU2("deg");
	sendEOL_CPU2();
	sendMsg_CPU2("\tODO: INV=");
	sendFloat32_CPU2(input_random_param.invalid_odometry_data);
	sendMsg_CPU2("%, NOIS=");
	sendFloat32_CPU2(input_random_param.noised_odometry_data);
	sendMsg_CPU2("%, AMP=");
	sendFloat32_CPU2(input_random_param.odometry_noise_amplitude);
	sendMsg_CPU2("deg");
	sendEOL_CPU2();

	sendMsg_CPU2("#max samples=");
	sendUInt32_CPU2(input_max_sample_nb);
	sendEOL_CPU2();
}
void sendCartography()
{
	sendMsg_CPU2("Carto: #nodes=");
	sendUInt32_CPU2((uint32) input_cartography.nb_nodes);
	sendMsg_CPU2(", #edges=");
	sendUInt32_CPU2((uint32) input_cartography.nb_arcs);
	sendMsg_CPU2(", vmax=");
	sendFloat32_CPU2(input_cartography.def_max_speed);
	sendMsg_CPU2("m/s");
	sendEOL_CPU2();
}
void sendMission()
{
	sendMsg_CPU2("Mission: #pts=");
	sendUInt32_CPU2((uint32) mission.size);
	sendEOL_CPU2();

	int mission_pt = 0;
	for (mission_pt = 0; mission_pt < mission.size; ++mission_pt)
	{
		sendMsg_CPU2("\t");
		sendUInt32_CPU2(mission.ind[mission_pt]);
		sendMsg_CPU2(": (");
		sendFloat32_CPU2(input_cartography.nodes[mission.ind[mission_pt]].x);
		sendMsg_CPU2(", ");
		sendFloat32_CPU2(input_cartography.nodes[mission.ind[mission_pt]].y);
		sendMsg_CPU2(")");
		sendEOL_CPU2();
	}
}
void sendExecTime()
{
	sendMsg_CPU2("ExecTime");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": ");
#ifndef PERFCOUNTERS
	sendUInt32_CPU2(cpu_ticks);
#else
	sendUInt32_CPU2(measurements.cpu_cycles);
#endif
	sendEOL_CPU2();
}
#ifdef PERFCOUNTERS
void sendAdditionalInfo()
{
	sendMsg_CPU2("Instructions");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": ");
	sendUInt32_CPU2(measurements.instructions);
	sendEOL_CPU2();
	sendMsg_CPU2("PCacheHits");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": ");
	sendUInt32_CPU2(measurements.pcache_hits);
	sendEOL_CPU2();
	sendMsg_CPU2("PCacheMisses");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": ");
	sendUInt32_CPU2(measurements.pcache_misses);
	sendEOL_CPU2();
}
#endif
void sendPositions()
{
	sendMsg_CPU2("ActPos");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": (");
	sendFloat32_CPU2(actual_position.x);
	sendMsg_CPU2(", ");
	sendFloat32_CPU2(actual_position.y);
	sendMsg_CPU2(")");
	sendEOL_CPU2();

	sendMsg_CPU2("KalPos");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": (");
	sendFloat32_CPU2(kalman_position.x);
	sendMsg_CPU2(", ");
	sendFloat32_CPU2(kalman_position.y);
	sendMsg_CPU2(")");
	sendEOL_CPU2();

	sendMsg_CPU2("GpsPos");
	sendUInt32_CPU2(iterate);
	sendMsg_CPU2(": (");
	sendFloat32_CPU2(gps.x);
	sendMsg_CPU2(", ");
	sendFloat32_CPU2(gps.y);
	sendMsg_CPU2(")");
	sendEOL_CPU2();
}

#ifdef SYNCHRO
//SYNCHRO
volatile boolean g_core2_synchro = FALSE;
#endif
/** \brief Main entry point for CPU2  */
void core2_main(void)
{

	/*
	 * !!WATCHDOG1 IS DISABLED HERE!!
	 * Enable the watchdog in the demo if it is required and also service the watchdog periodically
	 * */
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	init_CPU2();

	// Enable the global interrupts
	IfxCpu_enableInterrupts();

	// Initialize ASCLIN (serial line)
	serial_init_CPU2();

	Bool8 abort = 0;

	//initializations
	odometry = (T_odo) {0.0, 0.0, 0.0f, 0.0f, 0, 0, VALID_DATA};
	double Pd[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	T_mat Q = {3, 3, _MAT_ Pd}; ///< Error matrix computed with Kalman filter
	Speed s_arp	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
	Speed s_rec	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
	Speed s_safety = {0.0f, 0.0f, 0.0f}; ///< Speed selected among the previous ones by the Speed selector
	s_gui = (Speed) {0.0f, 0.0f, VALID_DATA}; ///< Speed set by the Guidance manager

	/* For execution time measurement */
#ifndef PERFCOUNTERS
	uint64 stm_ticks = 0;
#else
	initMeasurements();
#endif

	sendMsg_CPU2("Core2: starts...\r\n");

	/* Read INPUT from serial com */
//	sendMsg_CPU2("Reading program inputs:\r\n");
	char input_txt[MAX_LINE_SIZE];
	getMsg_CPU2(input_txt);
//	sendMsg_CPU2(input_txt);
//	sendEOL_CPU2();
	input_mission.size = atoi(input_txt);
	int index;
	for (index = 0; index < input_mission.size; ++index)
	{
		getMsg_CPU2(input_txt);
//		sendMsg_CPU2(input_txt);
//		sendEOL_CPU2();
		input_mission.ind[index] = atoi(input_txt);
	}
#ifdef SYNCHRO
////	sendMsg_CPU2("Enter shift: ");
//	getMsg_CPU2(input_txt);
//	uint32 shift = atoi(input_txt);
	uint32 shift = 0; //TEST
//	TTime stm_cycles = shift * (50 / 1000000000.0) * g_AppCpu2.info.stmFreq;
	uint32 cpu_cycles = shift * (50 / 1000000000.0) * g_AppCpu2.info.cpuFreq;
	sendMsg_CPU2("#Delay = ");
	sendUInt32_CPU2(cpu_cycles);
	sendMsg_CPU2(" CPU cycles\r\n");
#endif


	/* Send setup info */
	sendSetupInfo();

	/* mission initialization */
	com_mission_ordering(&input_mission, &mission, &abort);	///< initialize a default a simple mission

	/* Send carto and mission */
	sendCartography();
	sendMission();

	/* sensor data initialization */
	actual_position.x = input_cartography.nodes[mission.ind[0]].x;
	actual_position.y = input_cartography.nodes[mission.ind[0]].y;
	actual_position.theta = 0;
	gps.x = actual_position.x;
	gps.y = actual_position.y;
	gps.val = VALID_DATA;
	compass.theta = actual_position.theta;
	compass.val = VALID_DATA;
	odometry.l_speed = 0.0f;
	odometry.r_speed = 0.0f;

#ifdef SYNCHRO
	//SYNCHRO
	Ifx_CPU_CCNT ccnt;
//	uint32 delay;
//	IfxCpu_resetAndStartCounters(IfxCpu_CounterMode_normal);
	g_core2_synchro = TRUE;
	while ((g_core0_initDone  == FALSE) || (g_core1_initDone == FALSE));
	//	waitTime(stm_cycles);
//	ccnt0.U = __mfcr(CPU_CCNT);
//	delay = cpu_cycles + ccnt0.B.CountValue;
	IfxCpu_resetAndStartCounters(IfxCpu_CounterMode_normal);
	do
	{
		ccnt.U = __mfcr(CPU_CCNT);
	} while (ccnt.B.CountValue < cpu_cycles);
//	} while (ccnt.B.CountValue < delay);
//    ccnt.U                = __mfcr(CPU_CCNT);
//    sendMsg_CPU2("#CPU cycles=");
//    sendUInt32_CPU2(ccnt.B.CountValue);
////    sendUInt32_CPU2(ccnt.B.CountValue - ccnt0.B.CountValue);
//    sendEOL_CPU2();
    IfxCpu_stopCounters();
//	sendMsg_CPU2("time=");
//	sendUInt64_CPU2(now());
//	sendEOL_CPU2();
//	sendMsg_CPU2("time=");
//	sendUInt32_CPU2(ccnt.B.CountValue);
//	sendEOL_CPU2();
#endif

	/* BEGIN TIME MEASUREMENT */
#ifndef PERFCOUNTERS
	stm_ticks = (uint64) now();
#else
	startMeasurements();
#endif

	/* path computation */
	com_path_computing(abort, &mission, &input_cartography, &path); //DIJKSTRA

	/* END TIME MEASUREMENT */
#ifndef PERFCOUNTERS
	stm_ticks = (uint64) now() - stm_ticks;
	cpu_ticks = 2 * (stm_ticks - STM_OVERHEAD);
	sendExecTime();
#else
	stopMeasurements(&measurements);
	sendExecTime();
	sendAdditionalInfo();
#endif

	iterate = 1;

	IfxPort_setPinMode(&MODULE_P10, 2u, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinPadDriver(&MODULE_P10, 2u, IfxPort_PadDriver_cmosAutomotiveSpeed1);

	/* COMPUTATION LOOP */
	while (iterate <= input_max_sample_nb)
	{

		/* BEGIN TIME MEASUREMENT */
#ifndef PERFCOUNTERS
		stm_ticks = (uint64) now();
#else
		startMeasurements();
#endif

		/* BEGIN_mission_management_task */
		com_localize(compass, gps, odometry, &kalman_position, &Q);		///< Kalman call for better position estimation from noised data
		com_generation(kalman_position, &path, &position_sp); ///< defines SetPoint
		com_tracking(kalman_position, position_sp, &Q, &s_gui); ///< Kanayama call to track the defined SetPoint
		com_speed_selection(s_gui, s_rec, s_safety, s_arp, &s_out);	///< Shall select the biggest speed asserting s_out <= min(s_safety, s_arp)
		/* END_mission_management_task */

		/* END TIME MEASUREMENT */
#ifndef PERFCOUNTERS
		stm_ticks = (uint64) now() - stm_ticks;
		cpu_ticks = 2 * (stm_ticks - STM_OVERHEAD);
		sendExecTime();
#else
		stopMeasurements(&measurements);
		sendExecTime();
		sendAdditionalInfo();
#endif
		sendPositions();

		/** Function simulating a step,
					the simulation aims to test the guidance_manager results, the speed is also the result of it */
		update_simulation(
				(iterate==input_max_sample_nb),
				s_gui, position_sp, kalman_position,
				&actual_position, &compass, &gps, &odometry); ///< simulation function: simule device's data & trace results

		++iterate;

		IfxPort_togglePin(&MODULE_P10, 2u);

		/* loop delay: eq. sleep(10ms) */
		for (DelayLoopCounter = 0x0u; DelayLoopCounter < Approx10MSecCount; ++DelayLoopCounter);
	}

	for (DelayLoopCounter = 0x0u; DelayLoopCounter < Approx10MSecCount; ++DelayLoopCounter);

#ifdef AUTOMATIC_RESET
	/* SOFTWARE RESET -> config. to trigger SYSTEM RESET */
	uint16 endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    SCU_RSTCON.B.SW = 1;
    IfxScuWdt_setSafetyEndinit(endinitSfty_pw);
	endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    SCU_SWRSTCON.B.SWRSTREQ = 1;
    IfxScuWdt_setSafetyEndinit(endinitSfty_pw);
#endif

	IfxPort_setPinState(&MODULE_P10, 2u, IfxPort_State_high);
	/* background endless loop */
	while (TRUE)
	{
		;
	}
}
