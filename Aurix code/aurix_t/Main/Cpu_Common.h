/**
 * \file Cpu0_Main.h
 * \brief System initialization and main program implementation.
 *
 * \version iLLD_Demos_1_0_0_3_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Demo_GtmTomPwmHl_SrcDoc Source code documentation
 * \ingroup IfxLld_Demo_GtmTomPwmHl
 */

#ifndef CPU0_MAIN_H
#define CPU0_MAIN_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Cpu/Std/Ifx_Types.h"
#include "IfxScuWdt.h"

#include "Application_cfg.h"

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/
typedef struct
{
    float32 sysFreq; /**< \brief Actual SPB frequency */
    float32 cpuFreq; /**< \brief Actual CPU frequency */
    float32 pllFreq; /**< \brief Actual PLL frequency */
    float32 stmFreq; /**< \brief Actual STM frequency */
    boolean pCache;  /* program cache status: enabled or disabled */
    boolean dCache;  /* data cache status: enabled or disabled */
} AppInfo;

/** \brief Application information */
typedef struct
{
    AppInfo info;                               /**< \brief Info object */
} App_Cpu;


/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_Cpu g_AppCpu2;

#ifndef PERFCOUNTERS
#define STM_OVERHEAD 20
#endif

//SYNCHRO
extern volatile boolean g_core0_synchro, g_core1_synchro, g_core2_synchro, g_core0_initDone, g_core1_initDone;
#endif
