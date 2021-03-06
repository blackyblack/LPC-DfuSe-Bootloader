#ifndef __IAP_H_
#define __IAP_H_

#include <stdbool.h>

/*
*/

#define kIAPentry 0x1FFF1FF1


enum {
  CMD_PREPARE_SECTORS = 50,
  CMD_COPY_RAM_TO_FLASH,
  CMD_ERASE_SECTORS,
  CMD_BLANK_CHECK_SECTORS,
  CMD_READ_PART_ID,
  CMD_READ_BOOT_CODE_VERSION,
  CMD_COMPARE
};

enum {
  STATUS_CMD_SUCCESS = 0,
  STATUS_INVALID_COMMAND,
  STATUS_SRC_ADDR_ERROR,
  STATUS_DST_ADDR_ERROR,
  STATUS_SRC_ADDR_NOT_MAPPED,
  STATUS_DST_ADDR_NOT_MAPPED,
  STATUS_COUNT_ERROR,
  STATUS_INVALID_SECTOR,
  STATUS_SECTOR_NOT_BLANK,
  STATUS_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,
  STATUS_COMPARE_ERROR,
  STATUS_BUSY
};

typedef void (__thumb *IAP)(void*, void*);

//�������� �������
bool iap_erase(unsigned long address);

//�������� ������� �� �������
//���������� TRUE, ���� ������ �� ����
bool iap_blank_check(unsigned long address);

//������ ������ �� ����
bool iap_write(unsigned long address, int count, unsigned char *buffer);

#endif //__IAP_H_
