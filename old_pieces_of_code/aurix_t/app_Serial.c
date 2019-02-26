/*
 * Serial.c
 *
 *  Created on: 13 juil. 2017
 *      Author: Eric JENN
 *
 *      Largely inspired from the example "BaseFramework_TC27xC_iLLD_ASCLIN_ASC".
 *      Setup one serial line:
 *      - RX is pin P15_3, digital pin 0 on Arduino connector (RX0), see ShieldBuddyPinout doc.
 *      - TX is pin P15_2, digital pin 1 on Arduino connector (TX0), see ShieldBuddyPinout doc.
 *      - Speed is 38400 bds
 */

 /*
 ******************************************************************************
 * Standard include files
 ******************************************************************************
 */
#include <stdio.h> // sprintf from the compiler
#include <string.h> // strlen
#include "Config/Common/cfg_ISR.h"
#include <_Reg/IfxAsclin_reg.h>
#include <Asclin/Asc/IfxAsclin_Asc.h>

#include "Misc.h"
/*
 ******************************************************************************
 * Macro defines
 ******************************************************************************
 */
#define ASC_TX_BUFFER_SIZE 128
#define ASC_RX_BUFFER_SIZE 128


/*
 ******************************************************************************
 * Variable declarations
 ******************************************************************************
 */

static uint8 ascTxBuffer_CPU2[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 ascRxBuffer_CPU2[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/* Create the asc structure */
static IfxAsclin_Asc asc_CPU2;


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------


void sendMsg_CPU2(char * msg)
{
	Ifx_SizeT count = strlen(msg);
	IfxAsclin_Asc_write(&asc_CPU2, (uint8 *) msg, &count, TIME_INFINITE);
}

void getMsg_CPU2(char *msg)
{
	int read_size = 0;
	char read_char;
	Ifx_SizeT count = 1;
	while (read_size < (MAX_LINE_SIZE - 1))
	{
		IfxAsclin_Asc_read(&asc_CPU2, &read_char, &count, TIME_INFINITE);
		if (read_char == '\n' || read_char == '\r')
		{
			msg[read_size] = '\0';
			return;
		}
		msg[read_size] = read_char;
		++read_size;
	}
	msg[read_size] = '\0';
}

//----------------------------------------------------------------------
// ASC Interrupts handlers
//----------------------------------------------------------------------


IFX_INTERRUPT(asclin0TxISR_CPU2, 0, IFX_INTPRIO_ASCLIN2_TX);
void asclin0TxISR_CPU2(void) {
	IfxAsclin_Asc_isrTransmit(&asc_CPU2);
}

IFX_INTERRUPT(asclin0RxISR_CPU2, 0, IFX_INTPRIO_ASCLIN2_RX);
void asclin0RxISR_CPU2(void) {
	IfxAsclin_Asc_isrReceive(&asc_CPU2);
}

IFX_INTERRUPT(asclin0ErISR_CPU2, 0, IFX_INTPRIO_ASCLIN2_ER);
void asclin0ErISR_CPU2(void) {
	IfxAsclin_Asc_isrError(&asc_CPU2);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void serial_init_CPU2 (void)
{
    /* UART 0 SETUP */
	IfxAsclin_Asc_Config ascConfig_CPU2;

    // Create module config
    IfxAsclin_Asc_initModuleConfig(&ascConfig_CPU2, &MODULE_ASCLIN2);

    // Adjust sample point to suit HW (default values do not work)
    ascConfig_CPU2.baudrate.oversampling = 15;
    ascConfig_CPU2.bitTiming.medianFilter = 3;
    ascConfig_CPU2.bitTiming.samplePointPosition = 9;

    // Set the desired baudrate
    ascConfig_CPU2.baudrate.prescaler = 1;
//    ascConfig_CPU2.baudrate.baudrate = 38400; /* FDR values will be calculated in initModule */
    ascConfig_CPU2.baudrate.baudrate = 115200; /* FDR values will be calculated in initModule */

    // Set ISR priorities and interrupt targets
    ascConfig_CPU2.interrupt.txPriority = IFX_INTPRIO_ASCLIN2_TX;
    ascConfig_CPU2.interrupt.rxPriority = IFX_INTPRIO_ASCLIN2_RX;
    ascConfig_CPU2.interrupt.erPriority = IFX_INTPRIO_ASCLIN2_ER;
    ascConfig_CPU2.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreId();

    // Set FIFO configuration
    ascConfig_CPU2.txBuffer = &ascTxBuffer_CPU2;
    ascConfig_CPU2.txBufferSize = ASC_TX_BUFFER_SIZE;
    ascConfig_CPU2.rxBuffer = &ascRxBuffer_CPU2;
    ascConfig_CPU2.rxBufferSize = ASC_RX_BUFFER_SIZE;

    // Set pin configuration
    const IfxAsclin_Asc_Pins pins0 = {
            NULL_PTR,                   IfxPort_InputMode_pullUp,    /* CTS pin not used */
            &IfxAsclin2_RXE_P33_8_IN,   IfxPort_InputMode_pullUp,   		 /* Rx pin */
            NULL_PTR,                   IfxPort_OutputMode_pushPull,  /* RTS pin not used */
            &IfxAsclin2_TX_P33_9_OUT,   IfxPort_OutputMode_pushPull, 		 /* Tx pin */
            IfxPort_PadDriver_ttlSpeed1
    };
    ascConfig_CPU2.pins = &pins0;

    // Initialize ASC module
   IfxAsclin_Asc_initModule(&asc_CPU2, &ascConfig_CPU2);

    // Install SW interrupts
//    IfxCpu_Irq_installInterruptHandler(asclin0TxISR_CPU2, IFX_INTPRIO_ASCLIN2_TX);
//    IfxCpu_Irq_installInterruptHandler(asclin0RxISR_CPU2, IFX_INTPRIO_ASCLIN2_RX);
//    IfxCpu_Irq_installInterruptHandler(asclin0ErISR_CPU2, IFX_INTPRIO_ASCLIN2_ER);
}


