#include <includes.h>
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"

#include "bsp\bsp.h"
#include "debug\debug.h"

#include "iap.h"

//-------------------------------------------- defines

typedef struct SLayout {
  int sectors;
  int size;
} Layout;

//==============================================
//==============================================
//                32k flash, CortexM3
// LPC1751
//==============================================
//==============================================
//                64k flash, CortexM3
// LPC1752
//==============================================
//==============================================
//                128k flash, CortexM3
// LPC1754
// LPC1764
//==============================================
//==============================================
//                256k flash, CortexM3
// LPC1756
// LPC1765
// LPC1766
//==============================================
//==============================================
//                512k flash, CortexM3
// LPC1758
// LPC1768
//==============================================
//==============================================
//                32k flash, CortexM3/M0
// LPC1x13
// LPC1343
//==============================================
//==============================================
//                16k flash, CortexM3
// LPC1342
//==============================================
//==============================================
//                8k flash, CortexM3/M0
// LPC1x11
//==============================================

//-------------------------------------------- data

const Layout flashLayout[] =
{
  {16,  4096},
  {14, 32768},
  { 0,     0}
};

const int allowedWriteSizes[] =
{
  4096,
  1024,
  512,
  256,
  0
};

//-------------------------------------------- prototypes

/** internal functions **/
static int ExecuteCommand(unsigned long* cmd, unsigned long* status);

/** private data **/
static const IAP iap_entry = (IAP)kIAPentry; // MCU flash firmware interface function.
// The CPU clock speed (CCLK), the default value is used if no clock option is found.

//-------------------------------------------- modules

static int ExecuteCommand(unsigned long* cmd, unsigned long* status)
{
  int ret;
  CPU_SR_ALLOC();
  
  OS_CRITICAL_ENTER();

  for (;;)
  {
    iap_entry(cmd, status);
    ret = status[0];
    if (ret != STATUS_BUSY)
    {
      OS_CRITICAL_EXIT();
      return ret;
    }
    // Try again if busy.
    debug_hard("ExecuteCommand: busy\r\n");
  }
}

static int sector_by_address(unsigned long address)
{
  int sector = 0;
  
  ///HACK: LPC1788/LPC1768 only
  if(address < 0x00010000)
  {
    sector = address / 0x1000;
  }
  else
  {
    address -= 0x00010000;
    sector = address / 0x8000;
    sector += 16;
  }
  
  return sector;
}

//Запись буфера во флэш
bool iap_write(unsigned long address, int count, unsigned char *buffer)
{
  int ret;
  unsigned long cmd[5];
  unsigned long status[3];
  
  int sector = sector_by_address(address);

  cmd[0] = CMD_PREPARE_SECTORS;
  cmd[1] = sector;
  cmd[2] = sector;

  ret = ExecuteCommand(cmd, status);

  if (ret != STATUS_CMD_SUCCESS)
  {
    debug(DEBUG_LEVEL_FATAL, "CMD_PREPARE_SECTORS: %d\r\n", ret);
    //message CMD_PREPARE_SECTORS failed.
    return false;
  }

  cmd[0] = CMD_COPY_RAM_TO_FLASH;
  cmd[1] = address;
  cmd[2] = (unsigned long)buffer;
  cmd[3] = count;
  cmd[4] = SystemCoreClock/1000;

  ret = ExecuteCommand(cmd, status);

  if (ret != STATUS_CMD_SUCCESS)
  {
    debug(DEBUG_LEVEL_FATAL, "CMD_COPY_RAM_TO_FLASH: %d\r\n", ret);
    //message CMD_COPY_RAM_TO_FLASH failed.
    return false;
  }

  return true;
}

//стирание сектора
bool iap_erase(unsigned long address)
{
  int ret;
  unsigned long cmd[4];
  unsigned long status[3];
  
  int sector = sector_by_address(address);

  // Prepare sector for erase.
  cmd[0] = CMD_PREPARE_SECTORS;
  cmd[1] = sector;
  cmd[2] = sector;
  
  ret = ExecuteCommand(cmd, status);

  if (ret != STATUS_CMD_SUCCESS)
  {
    debug(DEBUG_LEVEL_FATAL, "CMD_PREPARE_SECTORS: %d\r\n", ret);
    //message CMD_PREPARE_SECTORS failed.
    return false;
  }

  // Erase sector.
  cmd[0] = CMD_ERASE_SECTORS;
  cmd[1] = sector;
  cmd[2] = sector;
  cmd[3] = SystemCoreClock/1000;

  ret = ExecuteCommand(cmd, status);

  if (ret != STATUS_CMD_SUCCESS)
  {
    debug(DEBUG_LEVEL_FATAL, "CMD_ERASE_SECTORS: %d\r\n", ret);
    //message CMD_ERASE_SECTORS failed.
    return false;
  }

  return true;
}

//проверка сектора на пустоту
//возвращает true, если сектор не пуст
bool iap_blank_check(unsigned long address)
{
  int ret;
  unsigned long cmd[4];
  unsigned long status[3];
  
  int sector = sector_by_address(address);

  // Prepare sector for erase.
  cmd[0] = CMD_BLANK_CHECK_SECTORS;
  cmd[1] = sector;
  cmd[2] = sector;

  ret = ExecuteCommand(cmd, status);

  if (ret == STATUS_SECTOR_NOT_BLANK)
  {
    return true;
  }

  return false;
}

