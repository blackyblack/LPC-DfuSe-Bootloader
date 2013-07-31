#ifndef UTILS_SRV_H
#define UTILS_SRV_H

/*
MODULE #4
Полезные функции.
Ни от чего не зависит.
*/

#include <stdbool.h>

// Команды установки, сброса и контроля битов-флажков в байте флажков
#define SETBIT(ADDRESS,BIT)   (ADDRESS |= (1<<BIT))    // Set bit in byte ADDRESS
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= (uint32_t)~(1<<BIT)) // Clear bit in  ADDRESS
#define COMPLBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))  // Complement bit
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))   // Check bit in byte ADDRESS

#define U32_DIFF(a, b) (((a) >= (b)) ? ((a) - (b)) : (((a) + ((b) ^ 0xFFFFFFFF) + 1)))

//П-пр преобразования hex в char
unsigned char utils_hex2chr(unsigned char data);

//П-пр преобразования char в hex
unsigned char utils_chr2hex(unsigned char *data_ptr);

// Подпрограмма сравнения двух массивов
// Один массив расположен во FLASH, другой массив buf_ptr
// partial_search = DEF_TRUE - возвращает DEF_TRUE для строк вида ONxx и строке поиска ON
// Если сравнение успешно - функция возвращает DEF_TRUE, в противном случае - DEF_FALSE
// Выполняет сравнение независимо от регистра и поддерживает русские буквы
bool utils_compare_string(bool partial_search, unsigned const char *ptr, unsigned char *buf_ptr);

//Подпрограмма вычисления CRC для указанного буфера
unsigned char utils_calc_crc( unsigned char *from_buffer, int buffer_size );

//Подпрограмма вычисления CRC для указанного символа буфера с учётом уже рассчитанного crc
unsigned char utils_calc_crc_feed( unsigned char from_buffer_char, unsigned char crc_old );

#endif  // UTILS_SRV_H
