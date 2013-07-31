#ifndef _MAIN_H_
#define _MAIN_H_

#include <os.h>

/*
MODULE #3
Глобальные макросы и определения.
*/

#define  SOFTPACK_VERSION  "1.0.0.0"

//-------------------------------------------- Delta modules
//#define  DELTA_USE_ETHERNET
#define  DELTA_USE_USB

#define  MAIN_CODES_MAX    32

//таймаут на блокировку семафора
#define LOCK_TIMEOUT_TICKS    2000

//Удобный макрос для отключения warning для неиспользованной переменной
#define DROP(x)        ((void) (x))
//Макрос для определения размера статического массива
#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

#define USER_PROGRAM_START              0x00010000

//Структура для передачи параметров в бутлоадер
typedef struct share_mem
{
  char boot_mode_magic[8];
  bool boot_mode_request;
  char section_end;
} share_mem_t;

//блокировка ОС
//в случае ошибки выводит в терминал error_description
void main_os_lock(OS_SEM  *pevent, char const *error_description);

//разблокировка ОС
void main_os_unlock(OS_SEM  *pevent);

//пауза на ticks тиков ОС
void main_os_delay(int ticks);

void user_app_start_hard();

void user_app_start_user();

#endif  //_MAIN_H_
