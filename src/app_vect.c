/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         EXCEPTION VECTORS
*
*                                       NXP LPC1766 (Cortex-M3)
*                                              on the
*
*                                          IAR LPC1766-SK 
*                                           Kickstart Kit
*
* Filename      : app_vect.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#include <includes.h>

#include "debug\debug.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#pragma language=extended
#pragma segment="CSTACK"

static  void  Reset_ISR          (void);

static void DummyHandlerROM(void);

static  void  App_NMI_ISR        (void);

static  void  App_Fault_ISR      (void);

static  void  App_BusFault_ISR   (void);

static  void  App_UsageFault_ISR (void);

static  void  App_MemFault_ISR   (void);

static  void  App_Spurious_ISR   (void);

extern  void  SystemInit         (void);
extern  void  __iar_program_start(void);
extern volatile bool boot_mode_request;

extern void  OSView_RxTxISRHandler (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
*               vector table.  The LPC17xx has 34 external interrupt vectors.
*********************************************************************************************************
*/

__root  const  APP_INTVECT_ELEM  __vector_table[] @ ".intvec" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /*  0, SP start value.                                */
    Reset_ISR,                                                  /*  1, PC start value.                                */
    DummyHandlerROM,                                                /*  2, NMI.                                           */
    DummyHandlerROM,                                              /*  3, Hard Fault.                                    */
    DummyHandlerROM,                                           /*  4, Memory Management.                             */
    DummyHandlerROM,                                           /*  5, Bus Fault.                                     */
    DummyHandlerROM,                                         /*  6, Usage Fault.                                   */
    DummyHandlerROM,                                           /*  7, Reserved.                                      */
    DummyHandlerROM,                                           /*  8, Reserved.                                      */
    DummyHandlerROM,                                           /*  9, Reserved.                                      */
    DummyHandlerROM,                                           /* 10, Reserved.                                      */
    DummyHandlerROM,                                           /* 11, SVCall.                                        */
    DummyHandlerROM,                                           /* 12, Debug Monitor.                                 */
    DummyHandlerROM,                                           /* 13, Reserved.                                      */
    DummyHandlerROM,                                       /* 14, PendSV Handler.                                */
    DummyHandlerROM,                                      /* 15, uC/OS-II Tick ISR Handler.                     */

    DummyHandlerROM,                                          /* 16, INTISR[  0]  WatchDog Timer                    */
    DummyHandlerROM,                                         /* 17, INTISR[  1]  Timer 0 (Match 0-3 Capture 0-3).  */
    DummyHandlerROM,                                         /* 18, INTISR[  2]  Timer 1 (Match 0-3 Capture 0-3).  */
    DummyHandlerROM,                                         /* 19, INTISR[  3]  Timer 2 (Match 0-3 Capture 0-3).  */
    DummyHandlerROM,                                         /* 20, INTISR[  4]  Timer 3 (Match 0-3 Capture 0-3).  */
    DummyHandlerROM,                                        /* 21, INTISR[  5]  UART 0  (RLS, THRE, RDA, CTI)     */
    DummyHandlerROM,                                        /* 22, INTISR[  6]  UART 1  (RLS, THRE, RDA, CTI)     */
    DummyHandlerROM,                                        /* 23, INTISR[  7]  UART 2  (RLS, THRE, RDA, CTI)     */
    DummyHandlerROM,                                        /* 24, INTISR[  8]  UART 3  (RLS, THRE, RDA, CTI)     */
    DummyHandlerROM,                                         /* 25, INTISR[  9]  PWM Interrupt                     */    
    DummyHandlerROM,                                         /* 26, INTISR[ 10]  I2C0   (RLS, THRE, RDA, CTI)      */
    DummyHandlerROM,                                         /* 27, INTISR[ 11]  I2C1   (RLS, THRE, RDA, CTI)      */
    DummyHandlerROM,                                         /* 28, INTISR[ 12]  I2C2   (RLS, THRE, RDA, CTI)      */
    DummyHandlerROM,                                         /* 29, INTISR[ 13]  SPI0    (SPIF, MODF)              */
    DummyHandlerROM,                                         /* 30, INTISR[ 14]  SSP0                              */
    DummyHandlerROM,                                         /* 31, INTISR[ 15]  SSP1                              */
    DummyHandlerROM,                                         /* 32, INTISR[ 15]  PLL lock (PLOCK)                  */
    DummyHandlerROM,                                          /* 33, INTISR[ 15]  Real Time Clock Interrupt         */
    DummyHandlerROM,                                        /* 34, INTISR[  7]  External interrupt 0 (EINT0)      */
    DummyHandlerROM,                                        /* 35, INTISR[  8]  External interrupt 0 (EINT0)      */
    DummyHandlerROM,                                        /* 36, INTISR[  9]  External interrupt 0 (EINT0)      */
    DummyHandlerROM,                                        /* 37, INTISR[ 10]  External interrupt 0 (EINT0).     */
    DummyHandlerROM,                                         /* 38, INTISR[ 11]  A/D Converter 0 end of conversion */
    DummyHandlerROM,                                          /* 39, INTISR[ 12]  Brown out detect                  */
    DummyHandlerROM,                                          /* 40, INTISR[ 12]  USB Interrupts, DMA Interrupt     */
    DummyHandlerROM,                                          /* 41, INTISR[ 12]  CAN Interrupt                     */
    DummyHandlerROM,                                        /* 42, INTISR[ 12]  IntSattus of DMA channel 0/1      */
    DummyHandlerROM,                                          /* 23, INTISR[ 12]  SI (state change)                 */
    DummyHandlerROM,                                         /* 44, INTISR[ 12]  Ethernet Interrupt                */
    DummyHandlerROM,                                          /* 45, INTISR[ 12]  MCI Card                          */
    DummyHandlerROM,                                    /* 46, INTISR[ 12]  Motor Control PWM                 */
    DummyHandlerROM,                                 /* 47, INTISR[ 12]  Quadrature Encoder                */
    DummyHandlerROM,                                         /* 48, INTISR[ 12]  PLL1 lock (PLOCK)                 */
    DummyHandlerROM,                                      /* 49, INTISR[ 12]  USB Activity                      */
    DummyHandlerROM,                                      /* 50, INTISR[ 12]  CAN Activity                      */
    
    DummyHandlerROM,                                        /* 51, INTISR[ 12]  UART4                             */
    DummyHandlerROM,                                         /* 52, INTISR[ 12]  SSP2                              */
    DummyHandlerROM,                                          /* 53, INTISR[ 12]  LCD                               */
    DummyHandlerROM,                                         /* 54, INTISR[ 12]  GPIO                              */
    DummyHandlerROM,                                         /* 55, INTISR[ 12]  PWM0                              */
    DummyHandlerROM                                        /* 56, INTISR[ 12]  EEPROM                            */
};

__root const APP_INTVECT_ELEM __vector_RAM_table[]  @ ".vectors_RAM" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /*  0, SP start value.                                */
    Reset_ISR,                                                  /*  1, PC start value.                                */
    App_NMI_ISR,                                                /*  2, NMI.                                           */
    App_Fault_ISR,                                              /*  3, Hard Fault.                                    */
    App_MemFault_ISR,                                           /*  4, Memory Management.                             */
    App_BusFault_ISR,                                           /*  5, Bus Fault.                                     */
    App_UsageFault_ISR,                                         /*  6, Usage Fault.                                   */
    App_Spurious_ISR,                                           /*  7, Reserved.                                      */
    App_Spurious_ISR,                                           /*  8, Reserved.                                      */
    App_Spurious_ISR,                                           /*  9, Reserved.                                      */
    App_Spurious_ISR,                                           /* 10, Reserved.                                      */
    App_Spurious_ISR,                                           /* 11, SVCall.                                        */
    App_Spurious_ISR,                                           /* 12, Debug Monitor.                                 */
    App_Spurious_ISR,                                           /* 13, Reserved.                                      */
    OS_CPU_PendSVHandler,                                       /* 14, PendSV Handler.                                */
    OS_CPU_SysTickHandler,                                      /* 15, uC/OS-II Tick ISR Handler.                     */

    BSP_IntHandlerWDT,                                          /* 16, INTISR[  0]  WatchDog Timer                    */
    BSP_IntHandlerTMR0,                                         /* 17, INTISR[  1]  Timer 0 (Match 0-3 Capture 0-3).  */
    BSP_IntHandlerTMR1,                                         /* 18, INTISR[  2]  Timer 1 (Match 0-3 Capture 0-3).  */
    BSP_IntHandlerTMR2,                                         /* 19, INTISR[  3]  Timer 2 (Match 0-3 Capture 0-3).  */
    BSP_IntHandlerTMR3,                                         /* 20, INTISR[  4]  Timer 3 (Match 0-3 Capture 0-3).  */
    BSP_IntHandlerUART0,                                        /* 21, INTISR[  5]  UART 0  (RLS, THRE, RDA, CTI)     */
    BSP_IntHandlerUART1,                                        /* 22, INTISR[  6]  UART 1  (RLS, THRE, RDA, CTI)     */
    BSP_IntHandlerUART2,                                        /* 23, INTISR[  7]  UART 2  (RLS, THRE, RDA, CTI)     */
    BSP_IntHandlerUART3,                                        /* 24, INTISR[  8]  UART 3  (RLS, THRE, RDA, CTI)     */
    BSP_IntHandlerPWM1,                                         /* 25, INTISR[  9]  PWM Interrupt                     */    
    BSP_IntHandlerI2C0,                                         /* 26, INTISR[ 10]  I2C0   (RLS, THRE, RDA, CTI)      */
    BSP_IntHandlerI2C1,                                         /* 27, INTISR[ 11]  I2C1   (RLS, THRE, RDA, CTI)      */
    BSP_IntHandlerI2C2,                                         /* 28, INTISR[ 12]  I2C2   (RLS, THRE, RDA, CTI)      */
    BSP_IntHandlerSPI0,                                         /* 29, INTISR[ 13]  SPI0    (SPIF, MODF)              */
    BSP_IntHandlerSSP0,                                         /* 30, INTISR[ 14]  SSP0                              */
    BSP_IntHandlerSSP1,                                         /* 31, INTISR[ 15]  SSP1                              */
    BSP_IntHandlerPLL0,                                         /* 32, INTISR[ 15]  PLL lock (PLOCK)                  */
    BSP_IntHandlerRTC,                                          /* 33, INTISR[ 15]  Real Time Clock Interrupt         */
    BSP_IntHandlerEINT0,                                        /* 34, INTISR[  7]  External interrupt 0 (EINT0)      */
    BSP_IntHandlerEINT1,                                        /* 35, INTISR[  8]  External interrupt 0 (EINT0)      */
    BSP_IntHandlerEINT2,                                        /* 36, INTISR[  9]  External interrupt 0 (EINT0)      */
    BSP_IntHandlerEINT3,                                        /* 37, INTISR[ 10]  External interrupt 0 (EINT0).     */
    BSP_IntHandlerADC0,                                         /* 38, INTISR[ 11]  A/D Converter 0 end of conversion */
    BSP_IntHandlerBOD,                                          /* 39, INTISR[ 12]  Brown out detect                  */
    BSP_IntHandlerUSB,                                          /* 40, INTISR[ 12]  USB Interrupts, DMA Interrupt     */
    BSP_IntHandlerCAN,                                          /* 41, INTISR[ 12]  CAN Interrupt                     */
    BSP_IntHandlerGPDMA,                                        /* 42, INTISR[ 12]  IntSattus of DMA channel 0/1      */
    BSP_IntHandlerI2S,                                          /* 23, INTISR[ 12]  SI (state change)                 */
    BSP_IntHandlerEMAC,                                         /* 44, INTISR[ 12]  Ethernet Interrupt                */
    BSP_IntHandlerMCI,                                          /* 45, INTISR[ 12]  MCI Card                          */
    BSP_IntHandlerMOTOR_PWM,                                    /* 46, INTISR[ 12]  Motor Control PWM                 */
    BSP_IntHandlerQUAD_ENCODER,                                 /* 47, INTISR[ 12]  Quadrature Encoder                */
    BSP_IntHandlerPLL1,                                         /* 48, INTISR[ 12]  PLL1 lock (PLOCK)                 */
    BSP_IntHandlerUSB_ACT,                                      /* 49, INTISR[ 12]  USB Activity                      */
    BSP_IntHandlerCAN_ACT,                                      /* 50, INTISR[ 12]  CAN Activity                      */
    
    BSP_IntHandlerUART4,                                        /* 51, INTISR[ 12]  UART4                             */
    BSP_IntHandlerSSP2,                                         /* 52, INTISR[ 12]  SSP2                              */
    BSP_IntHandlerLCD,                                          /* 53, INTISR[ 12]  LCD                               */
    BSP_IntHandlerGPIO,                                         /* 54, INTISR[ 12]  GPIO                              */
    BSP_IntHandlerPWM0,                                         /* 55, INTISR[ 12]  PWM0                              */
    BSP_IntHandlerEEPROM                                        /* 56, INTISR[ 12]  EEPROM                            */
};

static  void  Reset_ISR (void)
{
  SystemInit();
  __iar_program_start();
}

static void DummyHandlerROM(void)
{
}

/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

static  void  App_NMI_ISR (void)
{
  debug_hard_string("App_NMI_ISR\r\n");
  
  while (DEF_TRUE) {
        ;
  }
}


/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Fault_ISR (void)
{
  debug_hard_string("App_Fault_ISR\r\n");
  
  debug_fault_handler();
  
  while (DEF_TRUE) {
        ;
  }
}


/*
*********************************************************************************************************
*                                           App_BusFault_ISR()
*
* Description : Handle bus fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_BusFault_ISR (void)
{
  debug_hard_string("App_BusFault_ISR\r\n");
  
  while (DEF_TRUE) {
        ;
  }
}


/*
*********************************************************************************************************
*                                          App_UsageFault_ISR()
*
* Description : Handle usage fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_UsageFault_ISR (void)
{
  debug_hard_string("App_UsageFault_ISR\r\n");
  
  while (DEF_TRUE) {
        ;
  }
}


/*
*********************************************************************************************************
*                                           App_MemFault_ISR()
*
* Description : Handle memory fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_MemFault_ISR (void)
{
  debug_hard_string("App_MemFault_ISR\r\n");
  
  while (DEF_TRUE) {
        ;
  }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Spurious_ISR (void)
{
  debug_hard_string("App_Spurious_ISR\r\n");
  
  while (DEF_TRUE) {
        ;
  }
}
