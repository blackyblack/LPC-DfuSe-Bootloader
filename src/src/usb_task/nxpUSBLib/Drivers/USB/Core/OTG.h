/*
* Copyright(C) NXP Semiconductors, 2011
* All rights reserved.
*
* Copyright (C) Dean Camera, 2011.
*
* LUFA Library is licensed from Dean Camera by NXP for NXP customers 
* for use with NXP's LPC microcontrollers.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and 
* all warranties, express or implied, including all implied warranties of 
* merchantability, fitness for a particular purpose and non-infringement of 
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in 
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no 
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
* 
* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' and its 
* licensor's relevant copyrights in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
*/



/** \file
 *  \brief Common USB OTG definitions for all architectures.
 *  \copydetails Group_OTG
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in lpcroot/libraries/nxpUSBLib/Drivers/USB/USB.h.
 */

/** \ingroup Group_USB
 *  \defgroup Group_OTG USB On The Go (OTG) Management
 *  \brief USB OTG management definitions.
 *
 *  This module contains macros for embedded USB hosts with dual role On The Go capabilities, for managing role
 *  exchange. OTG is a way for two USB dual role devices to talk to one another directly without fixed device/host
 *  roles.
 *
 *  @{
 */

#ifndef __USBOTG_H__
#define __USBOTG_H__

	/* Includes: */
		#include "../../../Common/Common.h"
		#include "USBMode.h"		
		
	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include lpcroot/libraries/nxpUSBLib/Drivers/USB/USB.h instead.
		#endif

	/* Architecture Includes: */
		#if (ARCH == ARCH_AVR8)
			#include "AVR8/OTG_AVR8.h"
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

