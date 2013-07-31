#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils\utils_srv.h"

//структура для преобразования входного символа from_char в символ to_char
typedef struct
{
  unsigned char from_char;
  unsigned char to_char;
}sCharLink;

const sCharLink CharMap[] = 
{
  {'а', 'А'},   {'б', 'Б'},   {'в', 'В'},   {'г', 'Г'},   {'д', 'Д'},   {'е', 'Е'},   
  {'ё', 'Ё'},   {'ж', 'Ж'},   {'з', 'З'},   {'и', 'И'},   {'й', 'Й'},   {'к', 'К'}, 
  {'л', 'Л'},   {'м', 'М'},   {'н', 'Н'},   {'о', 'О'},   {'п', 'П'},   {'р', 'Р'}, 
  {'с', 'С'},   {'т', 'Т'},   {'у', 'У'},   {'ф', 'Ф'},   {'х', 'Х'},   {'ц', 'Ц'}, 
  {'ч', 'Ч'},   {'ш', 'Ш'},   {'щ', 'Щ'},   {'ъ', 'Ъ'},   {'ы', 'Ы'},   {'Ь', 'Ь'}, 
  {'э', 'Э'},   {'ю', 'Ю'},   {'я', 'Я'}
};


//П-пр преобразования hex в char
unsigned char utils_hex2chr(unsigned char data)
{
  unsigned char tmp;

  tmp = 0x0f & data;

  if ( tmp < 0x0a )
    tmp += '0';
  else	
    tmp += ('a' - 10);
	
  return( tmp );
}

//П-пр преобразования char в hex
unsigned char utils_chr2hex(unsigned char *data_ptr)
{
  unsigned char tmp;

  if((*data_ptr >= '0') && (*data_ptr <= '9'))
    tmp = 0x0f & (*data_ptr - '0');
  else if((*data_ptr >= 'A') && (*data_ptr <= 'F'))
    tmp = 10 + (0x0f & (*data_ptr - 'A'));
  else if((*data_ptr >= 'a') && (*data_ptr <= 'f'))
    tmp = 10 + (0x0f & (*data_ptr - 'a'));

  data_ptr++;
  tmp <<= 4;

  if((*data_ptr >= '0') && (*data_ptr <= '9'))
    tmp += (0x0f & (*data_ptr - '0'));
  else if((*data_ptr >= 'A') && (*data_ptr <= 'F'))
    tmp += (10 + (0x0f & (*data_ptr - 'A')));
  else if((*data_ptr >= 'a') && (*data_ptr <= 'f'))
    tmp += (10 + (0x0f & (*data_ptr - 'a')));

  return( tmp );
}

//поиск в таблице соответствия для данного символа
unsigned char find_char_in_map(unsigned char symbol)
{
  unsigned char i;
  
  for(i = 0; i < (sizeof(CharMap)/sizeof(sCharLink)); i++)
  {
    if(CharMap[i].from_char == symbol)
    {
      return CharMap[i].to_char;
    }
  }
  
  return 0;
}

//перевод символа в верхний регистр
unsigned char utils_to_upper(unsigned char a)
{
  unsigned char result;
  
  if((a >= 'a') && (a <= 'z'))
    return toupper(a);
  if((a >= 'A') && (a <= 'Z'))
    return a;
  
  result = find_char_in_map(a);
    
  if(result == 0)
  {
    return a;
  }
  else
  {
    return result;
  }
}

//преобразование из русских символов в аналогичные английские.
//вовзращает исходный символ, если не может преобразовать
unsigned char utils_transliterate_ru_to_en(unsigned char a)
{
  if(a == 'А')
    return 'A';
  if(a == 'В')
    return 'B';
  if(a == 'Е')
    return 'E';
  if(a == 'К')
    return 'K';
  if(a == 'М')
    return 'M';
  if(a == 'Н')
    return 'H';
  if(a == 'О')
    return 'O';
  if(a == 'Р')
    return 'P';
  if(a == 'С')
    return 'C';
  if(a == 'Т')
    return 'T';
  if(a == 'Х')
    return 'X';
  
  return a;
}

// Подпрограмма сравнения двух массивов
// Один массив расположен во FLASH, другой массив buf_ptr
// partial_search = true - возвращает true для строк вида ONxx и строке поиска ON
// Если сравнение успешно - функция возвращает true, в противном случае - false
bool utils_compare_string(bool partial_search, unsigned const char *ptr, unsigned char *buf_ptr)
{
  unsigned char a;
  unsigned char b;
  
  while( *ptr )
  {    
    a = *buf_ptr++;
    a = utils_to_upper(a);
    
    b = *ptr++;
    
    if((b != a) && (utils_transliterate_ru_to_en(b) != a))
    {
      return false;
    }
  }
  
  //если позволяем частичный поиск, то не проверяем окончание строки
  if(partial_search)
    return true;
  
  //если частичный поиск не позволен, то проверяем окончание обеих строк нулём
  if(*buf_ptr != 0)
    return false;
  
  return true;
}

//Подпрограмма вычисления CRC для указанного буфера
unsigned char utils_calc_crc( unsigned char *from_buffer, int buffer_size )
{
  unsigned char crc, tmp, tmp1;
  int loop;
  int i;

  crc = 0;

  for(loop = 0; loop < buffer_size; loop++)
  {
    tmp1 = *from_buffer++;
    tmp = tmp1;

    for(i = 0; i < 8; i++)
    {
      tmp ^= crc;

      if(0x01 & tmp)
      {
        crc >>= 1;
        crc ^= 0x8c;
      }
      else
      {
        crc >>= 1;
      }

      tmp1 >>= 1;
      tmp = tmp1;
    }
  }

  return crc;
}

//Подпрограмма вычисления CRC для указанного символа буфера с учётом уже рассчитанного crc
unsigned char utils_calc_crc_feed( unsigned char from_buffer_char, unsigned char crc_old )
{
  unsigned char tmp, tmp1;
  int i;

  tmp1 = from_buffer_char;
  tmp = tmp1;

  for(i = 0; i < 8; i++)
  {
    tmp ^= crc_old;

    if(0x01 & tmp)
    {
      crc_old >>= 1;
      crc_old ^= 0x8c;
    }
    else
    {
      crc_old >>= 1;
    }

    tmp1 >>= 1;
    tmp = tmp1;
  }

  return crc_old;
}



