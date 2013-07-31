#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils\utils_srv.h"

//��������� ��� �������������� �������� ������� from_char � ������ to_char
typedef struct
{
  unsigned char from_char;
  unsigned char to_char;
}sCharLink;

const sCharLink CharMap[] = 
{
  {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   
  {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'}, 
  {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'}, 
  {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'}, 
  {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'},   {'�', '�'}, 
  {'�', '�'},   {'�', '�'},   {'�', '�'}
};


//�-�� �������������� hex � char
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

//�-�� �������������� char � hex
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

//����� � ������� ������������ ��� ������� �������
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

//������� ������� � ������� �������
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

//�������������� �� ������� �������� � ����������� ����������.
//���������� �������� ������, ���� �� ����� �������������
unsigned char utils_transliterate_ru_to_en(unsigned char a)
{
  if(a == '�')
    return 'A';
  if(a == '�')
    return 'B';
  if(a == '�')
    return 'E';
  if(a == '�')
    return 'K';
  if(a == '�')
    return 'M';
  if(a == '�')
    return 'H';
  if(a == '�')
    return 'O';
  if(a == '�')
    return 'P';
  if(a == '�')
    return 'C';
  if(a == '�')
    return 'T';
  if(a == '�')
    return 'X';
  
  return a;
}

// ������������ ��������� ���� ��������
// ���� ������ ���������� �� FLASH, ������ ������ buf_ptr
// partial_search = true - ���������� true ��� ����� ���� ONxx � ������ ������ ON
// ���� ��������� ������� - ������� ���������� true, � ��������� ������ - false
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
  
  //���� ��������� ��������� �����, �� �� ��������� ��������� ������
  if(partial_search)
    return true;
  
  //���� ��������� ����� �� ��������, �� ��������� ��������� ����� ����� ����
  if(*buf_ptr != 0)
    return false;
  
  return true;
}

//������������ ���������� CRC ��� ���������� ������
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

//������������ ���������� CRC ��� ���������� ������� ������ � ������ ��� ������������� crc
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



