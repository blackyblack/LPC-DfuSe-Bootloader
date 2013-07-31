#include <includes.h>

#include <string.h>
#include <stdio.h>

#include "USB.h"
#include "Descriptors.h"

#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_pinsel.h"

#include "dfu.h"
#include "iap\iap.h"

#include "usb_task.h"

//-------------------------------------------- defines

#ifdef USB_BASE_MODE

#define USB_DATA_PORT                   (0)
#define USB_DP_PIN                      (29)
#define USB_DM_PIN                      (30)

#define USB_CONNECT_PORT                (0)
#define USB_CONNECT_PIN                 (14)

//-------------------------------------------- data

static OS_TCB  TaskTCB;
static OS_STK  TaskStk[OS_TASK_USB_STK_SIZE];

/** Current DFU state machine state, one of the values in the DFU_State_t enum. */
static uint8_t DFU_State = dfuIDLE;

/** Status code of the last executed DFU command. This is set to one of the values in the DFU_Status_t enum after
 *  each operation, and returned to the host when a Get Status DFU request is issued.
 */
static uint8_t DFU_Status = OK;

#pragma location = "USB_RAM"
ATTR_ALIGNED(64) uint8_t firmware_data[DFU_TRANSFER_SIZE];

//static int firmware_offset = 0;
static uint32_t CurrentAddr = 0x0000;

static bool need_detach = false;

//-------------------------------------------- prototypes

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

extern void USB_IRQHandler (void);

static void task_loop(void *p_arg);

//-------------------------------------------- modules

// Event handler for the library USB Connection event
void EVENT_USB_Device_Connect(void)
{
}

// Event handler for the library USB Disconnection event
void EVENT_USB_Device_Disconnect(void)
{
}

// Event handler for the library USB Configuration Changed event
void EVENT_USB_Device_ConfigurationChanged(void)
{
}

// Event handler for the library USB Control Request reception event.
void EVENT_USB_Device_ControlRequest(void)
{    
  if (!(Endpoint_IsSETUPReceived()))
    return;
    
    /* Ignore any requests that aren't directed to the DFU interface */
  if ((USB_ControlRequest.bmRequestType & (CONTROL_REQTYPE_TYPE | CONTROL_REQTYPE_RECIPIENT)) !=
        (REQTYPE_CLASS | REQREC_INTERFACE))
  {
    return;
  }

  int wValue = USB_ControlRequest.wValue;
  int size = USB_ControlRequest.wLength;

  Endpoint_ClearSETUP();
  
  switch (USB_ControlRequest.bRequest)
  {
  case DFU_REQ_DNLOAD:
    if(wValue == 0)
    {
      if(Endpoint_WaitUntilReady() != ENDPOINT_READYWAIT_NoError)
      {
          debug(DEBUG_LEVEL_ERROR, "Endpoint not ready\r\n");
          Endpoint_ClearOUT();
          Endpoint_ClearStatusStage();
          return;
      }
      
      Endpoint_Read_Stream_LE(firmware_data, size, NULL);
      Endpoint_ClearOUT();
      
      //GET COMMANDS
      if((size > 1) && (firmware_data[0] == 0x00))
      {
        //debug(DEBUG_LEVEL_INFO, "DFUSE: GET COMMANDS\r\n");
        DFU_State = dfuDNBUSY;
        DFU_Status = OK;
      }
      
      //SET ADDRESS
      if((firmware_data[0] == 0x21))
      {
        unsigned long address = 0;
        
        address = 
          (firmware_data[1] << 0) +
          (firmware_data[2] << 8) +
          (firmware_data[3] << 16) +
          (firmware_data[4] << 24);
        
        //debug(DEBUG_LEVEL_INFO, "DFUSE: SET ADDRESS @ %08X\r\n", address);        
        DFU_State = dfuDNBUSY;
        DFU_Status = OK;

        CurrentAddr = address;
      }
      
      //ERASE SECTOR
      if((firmware_data[0] == 0x41))
      {
        unsigned long address = 0;
        
        address = 
          (firmware_data[1] << 0) +
          (firmware_data[2] << 8) +
          (firmware_data[3] << 16) +
          (firmware_data[4] << 24);
        
        //debug(DEBUG_LEVEL_INFO, "DFUSE: ERASE SECTOR @ %08X\r\n", address);
        DFU_State = dfuDNBUSY;
        DFU_Status = OK;
        
        bool result = iap_erase(address);
        
        if(!result)
        {
          debug(DEBUG_LEVEL_FATAL, "iap_erase error\r\n");
          DFU_State = dfuERROR;
          DFU_Status = errERASE;
        }
      }
    }
    
    if(wValue >= 2)
    {
      //debug(DEBUG_LEVEL_INFO, "DFUSE: DNLOAD [%d]\r\n", wValue);
      
      if ((DFU_State == dfuDNLOAD_IDLE) && (size == 0))
      {
        DFU_State = dfuIDLE;
        
        Endpoint_ClearOUT();
        Endpoint_ClearStatusStage();
        break;
      }
      
      memset(firmware_data, 0, DFU_TRANSFER_SIZE);
      int firmware_offset = 0;
      int toread = 0;
      
      while(size)
      {
        if(size < FIXED_CONTROL_ENDPOINT_SIZE)
        {
          toread = size;
        }
        else
        {
          toread = FIXED_CONTROL_ENDPOINT_SIZE;
        }

        if(Endpoint_WaitUntilReady() != ENDPOINT_READYWAIT_NoError)
        {
          debug(DEBUG_LEVEL_ERROR, "Endpoint not ready\r\n");
          Endpoint_ClearOUT();
          Endpoint_ClearStatusStage();
          return;
        }
    
        Endpoint_Read_Stream_LE(firmware_data + firmware_offset, toread, NULL);
        Endpoint_ClearOUT();
        
        firmware_offset += toread;
        size -= toread;
        
        if(size <= 0)
        {
          size = 0;
          break;
        }
      }
      
      DFU_State = dfuDNBUSY;
      DFU_Status = OK;
      
      //Write to FLASH here
      bool write_result = iap_write(CurrentAddr, DFU_TRANSFER_SIZE, firmware_data);
      
      if(!write_result)
      {
        debug(DEBUG_LEVEL_FATAL, "iap_write error\r\n");
        DFU_State = dfuERROR;
        DFU_Status = errWRITE;
      }
      
      CurrentAddr += DFU_TRANSFER_SIZE;
    }

    Endpoint_ClearOUT();
    Endpoint_ClearStatusStage();
    break;
    
  case DFU_REQ_UPLOAD:
    //debug(DEBUG_LEVEL_INFO, "DFUSE: UPLOAD [%d]\r\n", wValue);
    
    while (!(Endpoint_IsINReady()))
    {
      if (USB_DeviceState == DEVICE_STATE_Unattached)
        return;
      
      main_os_delay(5);
    }
    
    unsigned char *memory;
    memory = (unsigned char *)CurrentAddr;
    
    Endpoint_Write_Stream_LE(memory, size, NULL);
    memory += size;
    CurrentAddr += size;
    
    DFU_State = dfuDNBUSY;
    DFU_Status = OK;
    
    Endpoint_ClearIN();
    Endpoint_ClearStatusStage();
    break;
    
  case DFU_REQ_GETSTATUS:
    //debug(DEBUG_LEVEL_INFO, "DFUSE: DFU_REQ_GETSTATUS\r\n");
    
    Endpoint_Write_8(DFU_Status);
    
    // Write 24-bit poll timeout value
    Endpoint_Write_8(0);
    Endpoint_Write_16_LE(0);
    
    Endpoint_Write_8(DFU_State);
    
    // Write 8-bit state string ID number
    Endpoint_Write_8(0);
    
    Endpoint_ClearIN();
    Endpoint_ClearStatusStage();
    
    if(DFU_State == dfuDNBUSY)
      DFU_State = dfuDNLOAD_IDLE;
    
    break;
    
  case DFU_REQ_CLRSTATUS:
    //debug(DEBUG_LEVEL_INFO, "DFUSE: DFU_REQ_CLRSTATUS\r\n");
    
    DFU_Status = OK;
    DFU_State = dfuIDLE;
    Endpoint_ClearStatusStage();
    break;
    
  case DFU_REQ_GETSTATE:
    //debug(DEBUG_LEVEL_INFO, "DFUSE: DFU_REQ_GETSTATE\r\n");
    
    Endpoint_Write_8(DFU_State);
    Endpoint_ClearIN();
    Endpoint_ClearStatusStage();
    break;
    
  case DFU_REQ_ABORT:
    //debug(DEBUG_LEVEL_INFO, "DFUSE: DFU_REQ_ABORT\r\n");
    
    DFU_State = dfuIDLE;
    Endpoint_ClearStatusStage();
    break;
    
  case DFU_REQ_DETATCH:
    debug(DEBUG_LEVEL_INFO, "DFUSE: DFU_REQ_DETATCH\r\n");
    need_detach = true;
    Endpoint_ClearStatusStage();
    break;
  }
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
}

uint32_t CALLBACK_HAL_GetISOBufferAddress(const uint32_t EPNum, uint32_t* last_packet_size)
{
  DROP(EPNum);
  DROP(last_packet_size);
  
  ///HACK: no iso transfers
  return 0;
}

//инициализаци€ USB до запуска ќ—
void usb_task_hard_init()
{  
  PINSEL_ConfigPin(USB_DATA_PORT, USB_DP_PIN, 1);
  PINSEL_ConfigPin(USB_DATA_PORT, USB_DM_PIN, 1);
  
  PINSEL_ConfigPin(USB_CONNECT_PORT, USB_CONNECT_PIN, 0);
  GPIO_SetDir(USB_CONNECT_PORT, _BIT(USB_CONNECT_PIN), GPIO_DIRECTION_OUTPUT);
  GPIO_OutputValue(USB_CONNECT_PORT, _BIT(USB_CONNECT_PIN), 1);
  
  NVIC_SetPriority(USB_IRQn, 0);
  NVIC_ClearPendingIRQ(USB_IRQn);
}

//инициализаци€ задачи
void usb_task_init()
{
  OS_ERR  err;
  
  OSTaskCreate((OS_TCB    *)&TaskTCB,
                 (CPU_CHAR  *)"USB",
                 (OS_TASK_PTR) task_loop,
                 (void      *) 0,
                 (OS_PRIO    )OS_TASK_USB_PRIO,
                 (CPU_STK   *)&TaskStk[0],
                 (OS_STK_SIZE)0,
                 (OS_STK_SIZE)OS_TASK_USB_STK_SIZE,
                 (OS_MSG_QTY )0,
                 (OS_TICK    )0,
                 (void      *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
  
  DROP(err);
}

//основной цикл работы задачи
static void task_loop(void *p_arg)
{  
  (void)p_arg;
  
  BSP_IntVectSet(BSP_INT_SRC_NBR_USB, USB_IRQHandler);
  
  DFU_State = dfuIDLE;  
  DFU_Status = OK;
  
  USB_Init();
  
  //delay for USB configuration or set USB_CONFIGURE hook
  main_os_delay(100);
  
  while(1)
  {
    USB_USBTask();
    main_os_delay(1);
    
    if(need_detach)
    {
      need_detach = true;
      
      main_os_delay(500);
      HAL_USBConnect(0);
      main_os_delay(500);
      user_app_start_user();
    }
  }
}

#endif // USB_BASE_MODE
