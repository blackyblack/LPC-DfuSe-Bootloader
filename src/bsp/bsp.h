#ifndef  BSP_PRESENT
#define  BSP_PRESENT

#include <stdint.h>
#include <stdbool.h>

/*
MODULE #2
������, �������������� �������������� � ������ � ������������ ����� ����������.
�� �������� ��������� �������!
������� �� CMSIS.
*/

//������������� BSP.
void BSP_init();

//������������� ��������
void BSP_watchdog_init();

void BSP_watchdog_off();

void BSP_watchdog_on();

//����� ��������
void BSP_watchdog_reset();

#endif  //BSP_PRESENT

 
