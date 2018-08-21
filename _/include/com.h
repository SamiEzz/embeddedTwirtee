/**
******************************************************************************
* @file		com.h
* @author   [IRT] RIPOLL Guillaume
* @brief	Generic header for the COMpute unit
******************************************************************************
*/
#ifndef __COM_H_
#define __COM_H_
#include <stdio.h>
#include "header.h"

#ifndef STANDALONE
#include <sys/types.h>
#endif

#define WHEEL_TEETH_NUMBER	2752.f	/**< -- called in float operations, Localization + Rec */
#define MAX_DELTA_SEGMENTS	16		/**< -- called in guidance_mgr.c && Segment/segment.c */

#endif
