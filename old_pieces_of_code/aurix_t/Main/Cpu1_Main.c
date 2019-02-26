/******************************************************************************
 * \file		Cpu0_main.c
 * \ingroup	 IRT - CAPHCA
 * \brief	   XXXX.
 * \details
 * \author		Guillaume PHAVORIN
 * \date		\$LastChangedDate:: 2018-01-11
 * \version	 \$Revision: 1
 * \attention   -
 *****************************************************************************/


//#define IFX_INTPRIO_CCU6 1 ///< priorities are normally defined in Ifx_IntPrioDef.h
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Cpu_Common.h"

#include "stdio.h" // sprintf
#include "SysSe/Bsp/Bsp.h"
#include "Config/Common/Ifx_cfg_FocControl.h"
#include "Config/Common/Ifx_cfg_VelocityControl.h" // PI controller configs
#include "DemoApp/AppTorquecontrol.h"
#include "DemoApp/AppVelocitycontrol.h"
#include "Common/Globals.h"

static uint32 volatile DelayLoopCounter_motor2;


boolean core1_mainInit(AppPwmDriver_3ChPwm* p_AppPwm_3ChPwm) {
	boolean status = TRUE;
	__disable();
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// Initialize PWM Driver
	status = AppPwmDriver_init(p_AppPwm_3ChPwm, &cfg_Pwm_3Phase_motor2);

	// Initialize resolver Interface and Encoder Interface
	IfxGpt12_IncrEnc_init(&g_motor2.sensors.encoder, &cfg_PosIf_IncrEnc_Gpt12_motor2); // AB
	encoderZero_init_1(); // Z

	// Initialize VAdc Driver - comment out due to access control, only CPU0 can write to VADC regs
	// AppVadc_init(&g_appVadc_kernel_motor2, &IfxVadc_adc_group_config_motor2, 1, &IfxVadc_cfg_adc_channel_motor2);

	// Initialize TorqueControl
	AppTorqueControl_initControl(&g_motor2, p_AppPwm_3ChPwm, 20000, &g_appVadc_kernel_motor2);

	// Init Velocity control
	VelocityControl_Init(&g_velocityControl_motor2);
	VelocityControl_SetKpKi(&g_velocityControl_motor2, VELOCITY_CONTROL_KP,VELOCITY_CONTROL_KI, VELOCITY_CONTROL_PERIOD, 0, 0);
	VelocityControl_SetLimit(&g_velocityControl_motor2, VELOCITY_CONTROL_MIN,VELOCITY_CONTROL_MAX);
	VelocityControl_SetMaxSpeed(&g_velocityControl_motor2, VELOCITY_CONTROL_MAX_RPM);
	VelocityControl_SetRefSpeed(&g_velocityControl_motor2, VELOCITY_CONTROL_REF);
	VelocityControl_Enable(&g_velocityControl_motor2);

	IfxPort_setPinMode(&MODULE_P10, 1u, IfxPort_Mode_outputPushPullGeneral); // D12
	IfxPort_setPinPadDriver(&MODULE_P10, 1u, IfxPort_PadDriver_cmosAutomotiveSpeed1); // D12

	// Initialize ASCLIN (serial line)
	serial_init_1();
	return status;
}

#ifdef TEST_CONTENTION
extern int *data_1 = (int*) 0xAF000000;
//static uint32 volatile DelayLoopCounter;
void core1_main(void)
{
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

	volatile int tab[1000] = {0};
	int i = 0;
	while (TRUE)
	{
		tab[i % 1000] += *(data_1 + (i % 1024));
		++i;
//		for (DelayLoopCounter = 0x0u; DelayLoopCounter < LOOP_DELAY; ++DelayLoopCounter);
	}
}
#else

#ifdef SYNCHRO
//SYNCHRO
volatile boolean g_core1_synchro = FALSE;
volatile boolean g_core1_initDone = FALSE;
#endif

/** \brief Main entry point for CPU1  */
void core1_main(void)
{

	boolean status = TRUE;
	char toPrint[100];
	AppPwmDriver_3ChPwm AppPwm_3ChPwm;

	status = core1_mainInit(&AppPwm_3ChPwm);

#ifdef SYNCHRO
	//SYNCHRO
	g_core1_synchro = TRUE;
	while ((g_core0_synchro  == FALSE) || (g_core2_synchro == FALSE));
#endif

	// Enable the global interrupts
	__enable();

#ifdef SYNCHRO
	g_core1_initDone = TRUE;
#endif

	sprintf(toPrint, "\r\nCore1 starts...%s\r\n", status?"OK":"FAILED");
	sendMsg_1(toPrint);

	g_motor2.diag.focControlEnabled = TRUE;
	g_motor2.diag.encoderEnabled = TRUE;
	g_motor2.sensors.encoder.base.offset = 0;
	g_motor2.openLoop.electricalAngle = 0;
	g_motor2.control.start = 1;
	g_motor2.control.stop = 0;
	g_motor2.openLoop.amplitude = 0.5; // amplitude_openLoop; // 0.1 (original) // ONLY USED WITHOUT CURRENT SENSING
	g_motor2.openLoop.electricalAngleDelta = 180000; // speed_openLoop; // 45000 (original) // ONLY USED WITHOUT CURRENT SENSING
	g_motor2.control.hadEmergency = FALSE; // otherwise zero sensor wont start

	while (TRUE)
	{
		for (DelayLoopCounter_motor2 = 0x0u; DelayLoopCounter_motor2 < 6000000U; ++DelayLoopCounter_motor2);
//		IFXGPT12(gpt12).T2.U // the content of the timer T2, the position of the motor
//		encoderMode,
//		calicount,
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
				adcExecuted_motor2,
				opMode_motor2,
				position_motor2, // PosIf_updatePosition((PosIf*)&g_motor2.sensors.encoder),
				rpm_motor2,
				rawADCValue1_motor2,
				rawADCValue2_motor2,
				rawADCValue3_motor2,
				g_motor2.inverter.channels[0],
				g_motor2.inverter.channels[1],
				g_motor2.inverter.channels[2],
				adcISRCPUID_motor2
			);
		sendMsg_1(toPrint);
	}
}
#endif
