#ifndef  BSP_PRESENT
#define  BSP_PRESENT

#include <stdint.h>
#include <stdbool.h>

/*
MODULE #2
Модуль, обеспечивающий взаимодействие с платой и определенным типом процессора.
Не содержит системных вызовов!
Зависит от CMSIS.
*/

//Инициализация BSP.
void BSP_init();

//инициализация вотчдога
void BSP_watchdog_init();

void BSP_watchdog_off();

void BSP_watchdog_on();

//сброс вотчдога
void BSP_watchdog_reset();

#endif  //BSP_PRESENT

 
