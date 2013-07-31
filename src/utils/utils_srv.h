#ifndef UTILS_SRV_H
#define UTILS_SRV_H

/*
MODULE #4
�������� �������.
�� �� ���� �� �������.
*/

#include <stdbool.h>

// ������� ���������, ������ � �������� �����-������� � ����� �������
#define SETBIT(ADDRESS,BIT)   (ADDRESS |= (1<<BIT))    // Set bit in byte ADDRESS
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= (uint32_t)~(1<<BIT)) // Clear bit in  ADDRESS
#define COMPLBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))  // Complement bit
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))   // Check bit in byte ADDRESS

#define U32_DIFF(a, b) (((a) >= (b)) ? ((a) - (b)) : (((a) + ((b) ^ 0xFFFFFFFF) + 1)))

//�-�� �������������� hex � char
unsigned char utils_hex2chr(unsigned char data);

//�-�� �������������� char � hex
unsigned char utils_chr2hex(unsigned char *data_ptr);

// ������������ ��������� ���� ��������
// ���� ������ ���������� �� FLASH, ������ ������ buf_ptr
// partial_search = DEF_TRUE - ���������� DEF_TRUE ��� ����� ���� ONxx � ������ ������ ON
// ���� ��������� ������� - ������� ���������� DEF_TRUE, � ��������� ������ - DEF_FALSE
// ��������� ��������� ���������� �� �������� � ������������ ������� �����
bool utils_compare_string(bool partial_search, unsigned const char *ptr, unsigned char *buf_ptr);

//������������ ���������� CRC ��� ���������� ������
unsigned char utils_calc_crc( unsigned char *from_buffer, int buffer_size );

//������������ ���������� CRC ��� ���������� ������� ������ � ������ ��� ������������� crc
unsigned char utils_calc_crc_feed( unsigned char from_buffer_char, unsigned char crc_old );

#endif  // UTILS_SRV_H
