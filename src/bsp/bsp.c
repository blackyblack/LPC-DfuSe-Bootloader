#include <stdbool.h>
#include <intrinsics.h>

#include "system_LPC177x_8x.h"

#include "lpc177x_8x_wwdt.h"
#include "lpc177x_8x_pinsel.h"

#include "bsp_int.h"
#include "debug\debug.h"
#include "main.h"

#include "bsp.h"

//-------------------------------------------- defines

#define WDT_TIMEOUT (5000000UL)

//-------------------------------------------- data

//-------------------------------------------- prototypes

//-------------------------------------------- modules

//Инициализация BSP. Происходит до запуска ОС - не использовать системных
//вызовов!
void  BSP_init (void)
{
  //тут ничего особенного не делается - задаётся название процессора
  CPU_Init();
  //тут настраиваются вектора прерываний - пустые по умолчанию
  BSP_IntInit();
  //Turn off WDT (if enabled from user app)
  WWDT_Enable(DISABLE);
  WWDT_Cmd(DISABLE);
  WWDT_ClrTimeOutFlag();
  WWDT_Feed ();
  
  NVIC_SetPriorityGrouping(0);
}

