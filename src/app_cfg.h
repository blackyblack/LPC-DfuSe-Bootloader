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

///HACK: размеры стеков подбираются вручную, в зависимости от свободного места в них при
///      штатной работе программы. Размеры подбираются таким образом, чтобы в среднем 
///      оставался запас в 40 байт в стеках.

#define  OS_TASK_START_STK_SIZE          120
#define  OS_TASK_WATCHDOG_STK_SIZE       OS_TASK_DEFAULT_STK_SIZE
#define  OS_TASK_MAIN_STK_SIZE           120
#define  OS_TASK_INDIC_STK_SIZE          100
//для задачи обработки звука можно выделить меньше места в стеках.
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
//минимум 750 для SSL.
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
#define  OS_TASK_WATCHDOG_PRIO             3  //самая приоритетная задача - вотчдог
///HACK: Были замечены тормоза у тач-мемори. Приоритет временно поменяли с 8 на 4.
#define  OS_TASK_TOUCH_PRIO                4  //тач-мемори должен реагировать шустро.
#define  OS_TASK_GSM_UART_PRIO             5  //самый приоритетный - буфер всего 32 байта
#define  OS_TASK_TIMER_PRIO                6  //В циклах ожидания должны быть переключения контекста
#define  OS_TASK_SHLF_PRIO                 7  //работа с шлейфами работает хорошо и без прерываний
#define  OS_TASK_INDIC_PRIO                8  //работа индикации не заметна - джиттер не влияет на восприятие
#define  OS_TASK_SOUND_PRIO                9  //звук должен звучать правильно - иначе очень заметно
#define  OS_TASK_BUTTONS_PRIO              10  //реакция на кнопки приоритетнее индикации
#define  OS_TASK_GSM_PRIO                  11  //работа с GSM модулем. Выставлен приоритет повыше, чтобы передача сообщения не прерывалась
#define  OS_TASK_UART0_PRIO                12  //обработчик команд ee_interface
//13
//14
//15
#define  OS_TASK_SHLF_REACT_PRIO           16  //реакция на шлейфы
//17
#define  OS_TASK_USB_PRIO                  4 //
#define  OS_TASK_USB_TALK_PRIO             19 //
//Начиная с этого приоритета резерв в несколько задач для разных потоков стека
#define  OS_TASK_LWIP_TASK_PRIO            20
//задача запуска стека и обработки входящих данных с Ethernet (поллинг DMA)
#define  OS_TASK_LWIP_INIT_PRIO            25
#define  OS_TASK_GPRS_PRIO                 26
#define  OS_TASK_MODBUS_PRIO               27
#define  OS_TASK_MAIN_PRIO                 28
//29
//Задача обеспечения SSL
#define  OS_SSL_PRIO                       30
#define  OS_SSL_SERV_PRIO                  31
//самая неприоритетная задача, требуется для вотчдога
#define  OS_TASK_START_PRIO                (OS_CFG_STAT_TASK_PRIO - 1)

/*       OS_TASK_STAT_PRIO       OS_LOWEST_PRIO - 1                                                         */
/*       OS_TASK_IDLE_PRIO       OS_LOWEST_PRIO                                                             */

//стандартный размер MAILBOX для lwIP
#define  MAILBOX_DEFAULT_SIZE              (12)

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN             DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN              DEF_ENABLED
#define  LIB_MEM_CFG_ALLOC_EN                    DEF_DISABLED

#endif  // APP_CFG_H
