/*
* Copyright(C) NXP Semiconductors, 2011
* All rights reserved.
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

#if defined(__LPC17XX__)||defined(__LPC177X_8X__)

#include "../HAL_LPC.h"
#include "../../../USBTask.h"

/********************************************************************//**
 * @brief
 * @param
 * @return
 *********************************************************************/
 void HAL_USBInit(void)
 {        
  	LPC_SC->PCONP |= (1UL<<31);                	/* USB PCLK -> enable USB Per.*/

	LPC_USB->USBClkCtrl = 0x1E;                 /* Dev, PortSel, AHB clock enable */
	while ((LPC_USB->USBClkSt & 0x1E) != 0x1E);
        
        //Set PORT0 for USB
        LPC_USB->StCtrl &= (uint32_t)~0x03;
        
        LPC_USB->USBClkCtrl = 0x12;
	while ((LPC_USB->USBClkSt & 0x12) != 0x12);

	HAL_Reset();
 }
/********************************************************************//**
 * @brief
 * @param
 * @return
 *********************************************************************/
 void HAL_USBDeInit(void)
 {
 	NVIC_DisableIRQ(USB_IRQn);               	/* disable USB interrupt */
 	LPC_SC->PCONP &= (~(1UL<<31));              /* disable USB Per.      */
 }
/********************************************************************//**
 * @brief
 * @param
 * @return
 *********************************************************************/
void HAL_EnableUSBInterrupt(void)
{
	NVIC_EnableIRQ(USB_IRQn);               	/* enable USB interrupt */
}
/********************************************************************//**
 * @brief
 * @param
 * @return
 *********************************************************************/
void HAL_DisableUSBInterrupt(void)
{
	NVIC_DisableIRQ(USB_IRQn);               	/* enable USB interrupt */
}
/********************************************************************//**
 * @brief
 * @param
 * @return
 *********************************************************************/
void HAL_USBConnect (uint32_t con)
{  
#ifdef USB_CAN_BE_DEVICE
	HAL17XX_USBConnect(con);
#endif
}

// TODO moving stuff to approriate places
extern void DcdIrqHandler (uint8_t DeviceID);
void USB_IRQHandler (void)
{
#ifdef USB_CAN_BE_HOST
	HcdIrqHandler(0);
#endif

#ifdef USB_CAN_BE_DEVICE
	DcdIrqHandler(0);
#endif
}

#endif /*__LPC17XX__*/
