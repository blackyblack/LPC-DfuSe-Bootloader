#ifndef APP_CFG_H
#define APP_CFG_H

#include  <os_cfg.h>
#include  <os_cfg_app.h>

/*
**************************************************************************************************************
*                                               STACK SIZES
**************************************************************************************************************
*/

#if (OS_CFG_STAT_TASK_EN == 0)
  #define  OS_TASK_DEFAULT_STK_SIZE   OS_CFG_IDLE_TASK_STK_SIZE
#else
  #define  OS_TASK_DEFAULT_STK_SIZE   OS_CFG_STAT_TASK_STK_SIZE
#endif

///HACK: ������� ������ ����������� �������, � ����������� �� ���������� ����� � ��� ���
///      ������� ������ ���������. ������� ����������� ����� �������, ����� � ������� 
///      ��������� ����� � 40 ���� � ������.

#define  OS_TASK_START_STK_SIZE          120
#define  OS_TASK_WATCHDOG_STK_SIZE       OS_TASK_DEFAULT_STK_SIZE
#define  OS_TASK_MAIN_STK_SIZE           120
#define  OS_TASK_INDIC_STK_SIZE          100
//��� ������ ��������� ����� ����� �������� ������ ����� � ������.
#define  OS_TASK_SOUND_STK_SIZE          OS_TASK_DEFAULT_STK_SIZE
#define  OS_TASK_GSM_UART_STK_SIZE       100
#define  OS_TASK_UART0_STK_SIZE          120
#define  OS_TASK_SHLF_STK_SIZE           OS_TASK_DEFAULT_STK_SIZE
#define  OS_TASK_SHLF_REACT_STK_SIZE     150
#define  OS_TASK_TOUCH_STK_SIZE          100
#define  OS_TASK_BUTTONS_STK_SIZE        150
#define  OS_TASK_TIMER_STK_SIZE          100
#define  OS_TASK_USB_STK_SIZE            600
#define  OS_TASK_USB_TALK_STK_SIZE       300

#ifdef AUTONOME_MODE
  #define  OS_TASK_GSM_STK_SIZE            300
#else
  #define  OS_TASK_GSM_STK_SIZE            200
#endif  //AUTONOME_MODE
#define  OS_TASK_LWIP_INIT               220
#define  OS_TASK_LWIP_TASK	         220
//������� 750 ��� SSL.
#define  OS_TASK_SSL_SIZE                500
#define  OS_TASK_SSL_SERV_SIZE           500
#define  OS_TASK_MODBUS_STK_SIZE         200
#define  OS_TASK_GPRS_STK_SIZE           120
/*
**************************************************************************************************************
*                                             TASK PRIORITIES
**************************************************************************************************************
*/

//#define  OS_CFG_TICK_TASK_PRIO             2
#define  OS_TASK_WATCHDOG_PRIO             3  //����� ������������ ������ - �������
///HACK: ���� �������� ������� � ���-������. ��������� �������� �������� � 8 �� 4.
#define  OS_TASK_TOUCH_PRIO                4  //���-������ ������ ����������� ������.
#define  OS_TASK_GSM_UART_PRIO             5  //����� ������������ - ����� ����� 32 �����
#define  OS_TASK_TIMER_PRIO                6  //� ������ �������� ������ ���� ������������ ���������
#define  OS_TASK_SHLF_PRIO                 7  //������ � �������� �������� ������ � ��� ����������
#define  OS_TASK_INDIC_PRIO                8  //������ ��������� �� ������� - ������� �� ������ �� ����������
#define  OS_TASK_SOUND_PRIO                9  //���� ������ ������� ��������� - ����� ����� �������
#define  OS_TASK_BUTTONS_PRIO              10  //������� �� ������ ������������ ���������
#define  OS_TASK_GSM_PRIO                  11  //������ � GSM �������. ��������� ��������� ������, ����� �������� ��������� �� �����������
#define  OS_TASK_UART0_PRIO                12  //���������� ������ ee_interface
//13
//14
//15
#define  OS_TASK_SHLF_REACT_PRIO           16  //������� �� ������
//17
#define  OS_TASK_USB_PRIO                  4 //
#define  OS_TASK_USB_TALK_PRIO             19 //
//������� � ����� ���������� ������ � ��������� ����� ��� ������ ������� �����
#define  OS_TASK_LWIP_TASK_PRIO            20
//������ ������� ����� � ��������� �������� ������ � Ethernet (������� DMA)
#define  OS_TASK_LWIP_INIT_PRIO            25
#define  OS_TASK_GPRS_PRIO                 26
#define  OS_TASK_MODBUS_PRIO               27
#define  OS_TASK_MAIN_PRIO                 28
//29
//������ ����������� SSL
#define  OS_SSL_PRIO                       30
#define  OS_SSL_SERV_PRIO                  31
//����� �������������� ������, ��������� ��� ��������
#define  OS_TASK_START_PRIO                (OS_CFG_STAT_TASK_PRIO - 1)

/*       OS_TASK_STAT_PRIO       OS_LOWEST_PRIO - 1                                                         */
/*       OS_TASK_IDLE_PRIO       OS_LOWEST_PRIO                                                             */

//����������� ������ MAILBOX ��� lwIP
#define  MAILBOX_DEFAULT_SIZE              (12)

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN             DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN              DEF_ENABLED
#define  LIB_MEM_CFG_ALLOC_EN                    DEF_DISABLED

#endif  // APP_CFG_H
