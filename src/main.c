#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "lpc_types.h"
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"

#include "bsp\bsp.h"
#include "bsp\bsp_int.h"
#include "debug\debug.h"
#include "utils\utils_srv.h"
#include "iap\iap.h"

#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_pinsel.h"

#include "src\usb_task\usb_task.h"

#include "main.h"

//-------------------------------------------- defines

///TODO: переназначить на выделенный пин входа в бутлоадер
#define BOOT_ON_PORT     (0)
#define BOOT_ON_PIN      (21)

#define BOOT_ON_TEST()   !CHECKBIT(FIO_ReadValue(BOOT_ON_PORT), BOOT_ON_PIN)

//Поместить это значение в область share ram для передачи параметров в бутлоадер
//place @ 0x1000FF10
#define BOOT_SHARE_MAGIC "boot001"

typedef void (* func)(void);

//-------------------------------------------- data

///TODO: подумать о размещении кода защиты: в бутлоадере или в приложении
//Установка CRP2 для релизной версии
/*#ifndef OS_DEBUG_VERSION
  __root  const  INT32U  CRP_value @ 0x2FC = {0x12345678};
#endif*/

static OS_TCB  TaskTCB;
static OS_STK  TaskStk[OS_TASK_START_STK_SIZE];

#pragma location = "share_section"
__root __no_init share_mem_t boot_mem;

//-------------------------------------------- prototypes

static void  AppTaskCreate (void);
static void  AppTaskStart(void *p_arg);
static void bootmode_start(void);

//-------------------------------------------- modules

static void inline jump(uint32_t addr)
{
  asm("LDR      SP,[R0]");
  asm("LDR      R0,[R0, #+4]");
  asm("BX       R0");
  
  DROP(addr);
}

void print_menu(void)
{  
  debug_hard("LPC1788 DFU BOOT @ %u MHz PCLK = %u MHz USBCLK = %u MHz\r\n", 
          SystemCoreClock / 1000000, PeripheralClock / 1000000, USBClock / 1000000);
}

//Принудительный режим бутлоадера
static bool boot_mode_requested()
{
  bool tmp = false;
  
  if(strncmp(boot_mem.boot_mode_magic, BOOT_SHARE_MAGIC, strlen(BOOT_SHARE_MAGIC)) == 0)
  {
    tmp = boot_mem.boot_mode_request;
  }
  
  if(!tmp)
  {
    debug_hard("boot_mode_request = %d\r\n", boot_mem.boot_mode_request);
  }
  
  return (tmp || BOOT_ON_TEST());
}

//Проверка наличия закаченного приложения в памяти
static bool app_magic_is_existing()
{
  bool tmp = iap_blank_check(USER_PROGRAM_START);
  
  debug_hard("app exist = %d\r\n", tmp);
  
  return tmp;
}

void main(void)
{  
  BSP_IntDisAll();

  debug_hard_init();
  BSP_init();
  SystemCoreClockUpdate();
  
  //Настройка аппаратного входа в бутлоадер
  PINSEL_ConfigPin(BOOT_ON_PORT, BOOT_ON_PIN, 0);
  GPIO_SetDir(BOOT_ON_PORT, BOOT_ON_PIN, GPIO_DIRECTION_INPUT);
  
  //если нет требования на вход в бутлоадер и если приложение уже
  //установлено, переходим к нему
  if(!boot_mode_requested() && app_magic_is_existing())
  {
    user_app_start_hard();
    return;
  }
  
  //если запрошен режим бутлоадера, переходим к нему.
  bootmode_start();
}

void user_app_start_hard()
{
  ///HACK: Нельзя вызывать системные вызовы тут!!!  
  debug_hard("Starting Application @ %08X\r\n", USER_PROGRAM_START);
  
  //stop OS timer
  BSP_IntDisAll();
  CPU_REG_NVIC_ST_CTRL  = CPU_REG_NVIC_ST_CTRL_CLKSOURCE;
  
  ///HACK: flush uart text
  debug_hard("\n");
  jump(USER_PROGRAM_START);
  
  while(1){ }; //you application was never executed
}

void user_app_start_user()
{  
  BSP_IntDisAll();
  CPU_REG_NVIC_ST_CTRL  = CPU_REG_NVIC_ST_CTRL_CLKSOURCE;
  
  ///HACK: flush uart text
  debug_hard("\n");
  
  //jump to bootloader and to user app
  NVIC_SystemReset();
}

#pragma section="share_section"
static void bootmode_start(void)
{
  OS_ERR err;
  
  unsigned int share_start = (unsigned int)__segment_begin("share_section");
  unsigned int share_end = (unsigned int)__segment_end("share_section");
  memset((void *)share_start, 0, share_end - share_start);

  usb_task_hard_init();
  print_menu();
  
  OSInit(&err);
    
  OSTaskCreate((OS_TCB    *)&TaskTCB,
                 (CPU_CHAR  *)"Start Task",
                 (OS_TASK_PTR) AppTaskStart,
                 (void      *) 0,
                 (OS_PRIO    )OS_TASK_START_PRIO,
                 (CPU_STK   *)&TaskStk[0],
                 0,
                 (OS_STK_SIZE)OS_TASK_START_STK_SIZE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
    
  AppTaskCreate();

  OS_CPU_SysTickInit(SystemCoreClock / OS_CFG_TICK_RATE_HZ);

  OSStart(&err);
  
  //Ucos3 started!
  DROP(err);
}

//Создание задач ОС
static  void  AppTaskCreate (void)
{
  debug_init();
  usb_task_init();
}

//Основная задача ОС
static void  AppTaskStart (void *p_arg)
{
  OS_ERR err;
  
  DROP(p_arg);

  while (1) 
  {      
    OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_DLY | OS_OPT_TIME_HMSM_STRICT, &err);
    DROP(err);
    debug_hard(".");
  }
}

//блокировка ОС
//в случае ошибки выводит в терминал error_description
void main_os_lock(OS_SEM  *pevent, char const *error_description)
{
  OS_ERR err;
  
  OSSemPend(pevent, LOCK_TIMEOUT_TICKS, OS_OPT_PEND_BLOCKING, (CPU_TS*)0, &err);

  if(err != OS_ERR_NONE)
  {
    //не используем debug, так как не хотим использовать системные вызовы
    if(debug_hard_is_on())
    {
      debug_hard_string(error_description);
    }
  }
  
  switch(err)
  {
  case OS_ERR_NONE:
    break;
  case OS_ERR_OBJ_DEL:
    debug_hard_string("\r\nOS_ERR_OBJ_DEL\r\n");
    break;
  case OS_ERR_OBJ_PTR_NULL:
    debug_hard_string("\r\nOS_ERR_OBJ_PTR_NULL\r\n");
    break;
  case OS_ERR_OBJ_TYPE:
    debug_hard_string("\r\nOS_ERR_OBJ_TYPE\r\n");
    break;
  case OS_ERR_OPT_INVALID:
    debug_hard_string("\r\nOS_ERR_OPT_INVALID\r\n");
    break;
  case OS_ERR_PEND_ABORT:
    debug_hard_string("\r\nOS_ERR_PEND_ABORT\r\n");
    break;
  case OS_ERR_PEND_ISR:
    debug_hard_string("\r\nOS_ERR_PEND_ISR\r\n");
    break;
  case OS_ERR_PEND_WOULD_BLOCK:
    debug_hard_string("\r\nOS_ERR_PEND_WOULD_BLOCK\r\n");
    break; 
  case OS_ERR_SCHED_LOCKED:
    debug_hard_string("\r\nOS_ERR_SCHED_LOCKED\r\n");
    break;
   case OS_ERR_STATUS_INVALID:
    debug_hard_string("\r\nOS_ERR_STATUS_INVALID\r\n");
    break; 
  case OS_ERR_TIMEOUT:
    debug_hard_string("\r\nOS_ERR_TIMEOUT\r\n");
    break;
  default:
    debug_hard_string("\r\nundefined\r\n");
    break;  
  }
  
  (void)error_description;
}

//разблокировка ОС
void main_os_unlock(OS_SEM  *pevent)
{
  OS_ERR err;
  
  OSSemPost(pevent, OS_OPT_POST_1, &err);
  
  switch(err)
  {
  case OS_ERR_NONE:
    break;
  case OS_ERR_OBJ_PTR_NULL:
    debug_hard_string("\r\nOS_ERR_OBJ_PTR_NULL\r\n");
    break;
  case OS_ERR_OBJ_TYPE:
    debug_hard_string("\r\nOS_ERR_OBJ_TYPE\r\n");
    break;
  case OS_ERR_SEM_OVF:
    debug_hard_string("\r\nOS_ERR_SEM_OVF\r\n");
    break;
  default:
    debug_hard_string("\r\nundefined\r\n");
    break;  
  }
  
  (void)err;
}

//пауза на ticks тиков ОС
void main_os_delay(int ticks)
{
  OS_ERR err;
  
  OSTimeDly(ticks, OS_OPT_TIME_DLY, &err);
  
  switch(err)
  {
  case OS_ERR_NONE:
    break;
  case OS_ERR_OPT_INVALID:
    debug_hard_string("\r\nOS_ERR_OPT_INVALID\r\n");
    break;
  case OS_ERR_SCHED_LOCKED:
    debug_hard_string("\r\nOS_ERR_SCHED_LOCKED\r\n");
    break;
  case OS_ERR_TIME_DLY_ISR:
    debug_hard_string("\r\nOS_ERR_TIME_DLY_ISR\r\n");
    break;
  case OS_ERR_TIME_ZERO_DLY:
    debug_hard_string("\r\nOS_ERR_TIME_ZERO_DLY\r\n");
    break;
  default:
    debug_hard_string("\r\nundefined\r\n");
    break;  
  }
  
  (void)err;
}


