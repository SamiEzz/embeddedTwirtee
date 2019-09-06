#include "Cpu_Common.h"

#include "stdio.h" // sprintf
#include <Scu/Std/IfxScuWdt.h>
#include <Vadc/Adc/IfxVadc_Adc.h>
#include <SysSe/Bsp/Bsp.h> // void initTime(void);
#include "Config/Common/Ifx_cfg_Scheduler.h"
#include "Config/Common/Ifx_cfg_FocControl.h" // for FoC, PI controller parameter
#include "Config/Common/Ifx_Cfg_VelocityControl.h"
#include "DemoApp/AppPwmDriver.h"
#include "DemoApp/AppScheduler.h"
#include "DemoApp/AppTaskFu.h"
#include "DemoApp/AppTimerOs.h"
#include "DemoApp/AppTorqueControl.h"
#include "DemoApp/AppVadc.h"
#include "DemoApp/AppVelocityControl.h"
#include "Common/Globals.h"
#include <Multican/Can/IfxMultican_Can.h>
#include "CPU2/Can.h"
static uint32 volatile DelayLoopCounter_motor1; // volatile here so that compiler does not optimize it due to assumption that some other SW will access it


boolean core0_mainInit(AppPwmDriver_3ChPwm* p_AppPwm_3ChPwm) {
	boolean status = TRUE;
	__disable();
	uint32 cpuPassword = IfxScuWdt_getCpuWatchdogPassword();
	uint32 safePassword = IfxScuWdt_getSafetyWatchdogPassword();
	IfxScuWdt_disableCpuWatchdog(cpuPassword);
	IfxScuWdt_disableSafetyWatchdog(safePassword);

	// Initialize Tasks
	{
		AppScheduler_cfg_tasks config =
				appScheduler_task_config[AppScheduler_Task_10ms];
		config.callback = (schedulerCallback) &appTaskfu_10ms;
		AppScheduler_initTask(&g_tasks_core0[AppScheduler_Task_10ms], &config);

		config = appScheduler_task_config[AppScheduler_Task_100ms];
		config.callback = (schedulerCallback) &appTaskfu_100ms;
		AppScheduler_initTask(&g_tasks_core0[AppScheduler_Task_100ms], &config);

		config = appScheduler_task_config[AppScheduler_Task_200ms];
		config.callback = (schedulerCallback) &appTaskfu_200ms;
		AppScheduler_initTask(&g_tasks_core0[AppScheduler_Task_200ms], &config);

		config = appScheduler_task_config[AppScheduler_Task_1ms];
		config.callback = (schedulerCallback) &appTaskfu_1ms;
		AppScheduler_initTask(&g_tasks_core0[AppScheduler_Task_1ms], &config);

	}
	// Initialize time constants for Time functions see bsp.c
	initTime();

	// Initialize PWM Driver
	status = AppPwmDriver_init(p_AppPwm_3ChPwm, &cfg_Pwm_3Phase_motor1);
	if(!status) return status;

	// Initialize resolver Interface and Encoder Interface
	//initPosIf();
	IfxGpt12_IncrEnc_init(&g_motor1.sensors.encoder, &cfg_PosIf_IncrEnc_Gpt12_motor1);

	// Initialize VAdc Driver
	//AppVadc_init(&g_appVadc_kernel_motor1, &IfxVadc_adc_group_config, 4, &IfxVadc_cfg_adc_channel_motor1);
	//AppVadc_init(&g_appVadc_kernel_motor2, &IfxVadc_adc_group_config, 4, &IfxVadc_cfg_adc_channel_motor2); // DOES NOT work, module reseted, need to merge like below (AppVadc_init2)!
	AppVadc_init2(&g_appVadc_kernel_motor1, &IfxVadc_adc_group_config[0], 4, &IfxVadc_cfg_adc_channel_motor1[0], &g_appVadc_kernel_motor2, &IfxVadc_cfg_adc_channel_motor2[0]);

	// Initialize TorqueControl
	AppTorqueControl_initControl(&g_motor1, p_AppPwm_3ChPwm, 20000, &g_appVadc_kernel_motor1);

	// Init Velocity control
	VelocityControl_Init(&g_velocityControl_motor1);
	VelocityControl_SetKpKi(&g_velocityControl_motor1, VELOCITY_CONTROL_KP,VELOCITY_CONTROL_KI, VELOCITY_CONTROL_PERIOD, 0, 0);
	VelocityControl_SetLimit(&g_velocityControl_motor1, VELOCITY_CONTROL_MIN,VELOCITY_CONTROL_MAX);
	VelocityControl_SetMaxSpeed(&g_velocityControl_motor1, VELOCITY_CONTROL_MAX_RPM);
	VelocityControl_SetRefSpeed(&g_velocityControl_motor1, VELOCITY_CONTROL_REF);

	// Initialize Timer for OS Scheduler
	AppTimerOs_interruptsInit();

	// make sure iq has something to do with limit.imag, which is set to eMotorBoardCurrentMax
	VelocityControl_Enable(&g_velocityControl_motor1);

	// Initialize ASCLIN (serial line)
	serial_init_0();

	// the following pins are set to VCC so no need to use digitial pins
	IfxPort_setPinMode(&MODULE_P10, 2u, IfxPort_Mode_outputPushPullGeneral); // LED, D13
	IfxPort_setPinPadDriver(&MODULE_P10, 2u, IfxPort_PadDriver_cmosAutomotiveSpeed1); // LED, D13

	return status;
}

#ifdef TEST_CONTENTION
extern int *data_0 = (int*) 0xAF000000;
//static uint32 volatile DelayLoopCounter;
void core0_main(void)
{
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	volatile int tab[1000] = {0};
	int i = 0;
	while (TRUE)
	{
		tab[i % 1000] += *(data_0 + (i % 1024));
		++i;
//		for (DelayLoopCounter = 0x0u; DelayLoopCounter < LOOP_DELAY; ++DelayLoopCounter);
	}
}
#else

#ifdef SYNCHRO
//SYNCHRO
volatile boolean g_core0_synchro = FALSE;
volatile boolean g_core0_initDone = FALSE;
#endif

volatile int hand = 0;
int core0_main(void) {

	boolean status;
	char toPrint[100];
	AppPwmDriver_3ChPwm AppPwm_3ChPwm;

	status = core0_mainInit(&AppPwm_3ChPwm);

#ifdef SYNCHRO
	//SYNCHRO
	g_core0_synchro = TRUE;
	while ((g_core1_synchro  == FALSE) || (g_core2_synchro == FALSE));
#endif

	__enable();

#ifdef SYNCHRO
	g_core0_initDone = TRUE;
#endif

#ifdef TEST_CAN_BUS

	CAN_init();
	IfxMultican_Status status1, status2;
	CAN_init_nodes(&status1, &status2);
	while(g_CAN_BUS_READY != 1);
	sendMsg_0("\r\nCAN is ready to test\r\n");
	DelayLoopCounter_motor1 = 0;
	int j = 0;
	int k = 0;
	while(1) {
		// wait sometime not to go faster than limit
		DelayLoopCounter_motor1++;
		if(DelayLoopCounter_motor1 == 2000000U) {
			DelayLoopCounter_motor1 = 0;
			j = j + 1000;
			k = 10000 - j;
			CAN_init_msg(CAN_NODE_2, SEND, I_SPEED_SP);
			CAN_send_odometry(CAN_NODE_2, j, k);
			sendMsg_0(".");
			if(j == 10000)
				j = 0;
		}
	}
#endif

	sprintf(toPrint, "\r\neMotor starts...%s\r\n", status?"OK":"FAILED");
	sendMsg_0(toPrint);

#define NO_TASK
#ifdef NO_TASK
	g_motor1.diag.focControlEnabled = TRUE;
	g_motor1.diag.encoderEnabled = TRUE;
	g_motor1.sensors.encoder.base.offset = 0;
	g_motor1.openLoop.electricalAngle = 0;
	g_motor1.control.start = 1; // ADC_ISR->TorqueControl_StartMotor(motorControlRun = 1) // setting to 0 useless, need to set stop to 1 or motorControlRun = 0
	g_motor1.control.stop = 0;
	g_motor1.openLoop.amplitude = 0.5; // amplitude_openLoop; // 0.1 (original) // ONLY USED WITHOUT CURRENT SENSING
	g_motor1.openLoop.electricalAngleDelta = 180000; // speed_openLoop; // 45000 (original) // ONLY USED WITHOUT CURRENT SENSING
	g_motor1.control.hadEmergency = FALSE; // otherwise zero sensor wont start

	int showCount = 0;

	DelayLoopCounter_motor1 = 0;

	while (TRUE)
	{
		DelayLoopCounter_motor1++;
		if(DelayLoopCounter_motor1 == 2000000U)
		{
			DelayLoopCounter_motor1 = 0;

			sprintf(toPrint, "%ld:"
					"op:%ld, "
					"sen:%ld, "
					"rpm:%ld, "
					"adc1:%ld, "
					"adc2:%ld "
					"adc3:%ld "
					"pwm1:%ld "
					"pwm2:%ld "
					"pwm3:%ld "
					"@ CPU%ld\r\n",
					adcExecuted_motor1,
					opMode_motor1,
					position_motor1, // PosIf_updatePosition((PosIf*)&g_motor1.sensors.encoder), // IFXGPT12(gpt12).T3.U // timer value, which is also the position
					rpm_motor1,
					rawADCValue1_motor1,
					rawADCValue2_motor1,
					rawADCValue3_motor1,
					g_motor1.inverter.channels[0],
					g_motor1.inverter.channels[1],
					g_motor1.inverter.channels[2],
					adcISRCPUID_motor1
				);
			sendMsg_0(toPrint);

#ifdef STOP
			showCount++;
			if(showCount < 1500) {
				sprintf(toPrint, "%d enc:%d/%d, s:%d, d1:%ld, d2:%ld, d3:%ld / %ld, rpm:%d r1:%ld, r2:%ld, r3:%ld\r\n", showCount, encoderMode, calicount, sensorposition, tOn1, tOn2, tOn3, periodt, rpm, rawADCValue1, rawADCValue2, rawADCValue3);
				sendMsg_0(toPrint);

				if(showCount == 1000)
					stopCollecting = 0;
			}
			else if(showCount == 1500) {
				stopCollecting = 1;
			}
			else if(showCount == 1600) {
				for(i = 0; i < SAMPLE_COUNT; i++) {
					sprintf(toPrint, "%ld,%d,%d,%d,%d,%d,%d\r\n", i, arrayT1[i], arrayT2[i], arrayT3[i], arrayR1[i], arrayR2[i], arrayR3[i]);
					sendMsg_0(toPrint);
				}
			}
			else {
				g_motor1.openLoop.amplitude = 0;
			}
#endif
		}
	}
#else // TASK system
	uint32 i = 0;
	i = 0;
	while (1) {
		AppScheduler_process(&g_tasks_core0[i++]);
		if(i == AppScheduler_Task_count) {
			i = 0;
		}
	}
#endif

	while (TRUE)
	{
		;
	}
	return (1);
}
#endif
