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
unsigned int iterate = 0;

void core2_main(void)
{


	Bool8 abort = 0;

	//initializations
	odometry = (T_odo) {0.0, 0.0, 0.0f, 0.0f, 0, 0, VALID_DATA};
	double Pd[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };
	T_mat Q = {3, 3, _MAT_ Pd}; ///< Error matrix computed with Kalman filter
	Speed s_arp	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the anticollision process ARP
	Speed s_rec	= {0.0f, 0.0f, 0.0f}; ///< Speed set by the joystick if present
	Speed s_safety = {0.0f, 0.0f, 0.0f}; ///< Speed selected among the previous ones by the Speed selector
	s_gui = (Speed) {0.0f, 0.0f, VALID_DATA}; ///< Speed set by the Guidance manager

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

	/* mission initialization */
	com_mission_ordering(&input_mission, &mission, &abort);	///< initialize a default a simple mission

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


	/* path computation */
	com_path_computing(abort, &mission, &input_cartography, &path); //DIJKSTRA

	iterate = 1;

	/* COMPUTATION LOOP */
	while (iterate <= input_max_sample_nb)
	{

		/* BEGIN_mission_management_task */
		com_localize(compass, gps, odometry, &kalman_position, &Q);		///< Kalman call for better position estimation from noised data
		com_generation(kalman_position, &path, &position_sp); ///< defines SetPoint
		com_tracking(kalman_position, position_sp, &Q, &s_gui); ///< Kanayama call to track the defined SetPoint
		com_speed_selection(s_gui, s_rec, s_safety, s_arp, &s_out);	///< Shall select the biggest speed asserting s_out <= min(s_safety, s_arp)
		/* END_mission_management_task */

		sendPositions();

		/** Function simulating a step,
					the simulation aims to test the guidance_manager results, the speed is also the result of it */
		update_simulation(
				(iterate==input_max_sample_nb),
				s_gui, position_sp, kalman_position,
				&actual_position, &compass, &gps, &odometry); ///< simulation function: simule device's data & trace results

		++iterate;

		/* loop delay: eq. sleep(10ms) */
		for (DelayLoopCounter = 0x0u; DelayLoopCounter < Approx10MSecCount; ++DelayLoopCounter);
	}

}
