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
