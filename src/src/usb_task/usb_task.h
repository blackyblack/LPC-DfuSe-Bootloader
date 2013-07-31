#ifndef _USB_TASK_H_
#define _USB_TASK_H_

/*
*/

#include <stdbool.h>
#include "main.h"

#ifdef DELTA_USE_USB
  #define USB_BASE_MODE
#endif

//-------------------------------------------- Dummy module interface

#ifndef USB_BASE_MODE

  #define usb_task_hard_init()                  while(0)
  #define usb_task_init()                       while(0)

#endif  // USB_BASE_MODE

//-------------------------------------------- Normal module interface

#ifdef USB_BASE_MODE

extern volatile bool RuntimeMode;

//инициализация USB до запуска ОС
void usb_task_hard_init();

//инициализация задачи
void usb_task_init();

#endif // USB_BASE_MODE

#endif //_USB_TASK_H_


