#include "Descriptors.h"
#include "dfu.h"

const USB_Descriptor_Device_t DfuDeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(02.00),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x0483,
	.ProductID              = 0xDF11,
	.ReleaseNumber          = 0,

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_DfuConfiguration_t DfuConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_DfuConfiguration_t),
			.TotalInterfaces        = 1,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},

	.DFU_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 0,

			.Class                  = 0xFE,
			.SubClass               = 0x01,
			.Protocol               = 0x02,

			.InterfaceStrIndex      = 0x03
		},

	.DFU_Functional =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_DFU_Functional_t), .Type = DTYPE_DFUFunctional},

			.Attributes             = (ATTR_MANEFESTATION_TOLLERANT | ATTR_WILL_DETATCH | ATTR_CAN_UPLOAD | ATTR_CAN_DOWNLOAD),

			.DetachTimeout          = 65535,
			.TransferSize           = DFU_TRANSFER_SIZE,

			.DFUSpecification       = 0x011A
		}
};

/** Language descriptor structure. This descriptor, located in SRAM memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t LanguageString =
{
	.Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},

	.UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t ManufacturerString =
{
  .Header                 = {.Size = USB_STRING_LEN(5), .Type = DTYPE_String},
  .UnicodeString          = 
    {
      'P',
      'R',
      'O',
      'F',
      'I'
    }
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t ProductString =
{
  .Header                 = {.Size = USB_STRING_LEN(12), .Type = DTYPE_String},
  .UnicodeString          = 
    {
      'R',
      'E',
      'D',
      'U',
      'T',
      ' ',
      'D',
      'E',
      'L',
      'T',
      'A',
      '1'
    }
};

/*
@Target Memory Name/Start Address/Sector(1)_Count*Sector(1)_Size
Sector(1)_Type,Sector(2)_Count*Sector(2)_SizeSector(2)_Type,...
...,Sector(n)_Count*Sector(n)_SizeSector(n)_Type
Another example, for STM32 Flash microcontroller, is shown below:
@Internal Flash /0x08000000/12*001Ka,116*001Kg" in case of
STM3210B-EVAL board.
@Internal Flash /0x08000000/6*002Ka,250*002Kg" in case of
STM3210E-EVAL board.
@Internal Flash /0x08000000/6*002Ka,122*002Kg" in case of
STM3210C-EVAL board.
@Internal Flash /0x08000000/48*256 a,464*256 g" in case of
STM32L152-EVAL board.
@Internal Flash /0x08000000/48*256 a,1488*256 g" in case of
STM32L152D-EVAL board.

– @: To detect that this is a special mapping descriptor (to avoid decoding standard
descriptor)
– /: for separator between zones
– Maximum 8 digits per address starting by “0x”
– /: for separator between zones
– Maximum of 2 digits for the number of sectors
– *: For separator between number of sectors and sector size
– Maximum 3 digits for sector size between 0 and 999
– 1 digit for the sector size multiplier. Valid entries are: B (byte), K (Kilo), M (Mega)
– 1 digit for the sector type as follows:
a (0x41): Readable
b (0x42): Erasable
c (0x43): Readable and Erasable
d (0x44): Writeable
e (0x45): Readable and Writeable
f (0x46): Erasable and Writeable
g (0x47): Readable, Erasable and Writeable
*/

const char flash_info_str[] = 
  "@Flash /0x00000000/16*004Kg,14*032Kg";

USB_Descriptor_String_t NameString =
{
  .Header                 = {.Size = USB_STRING_LEN(sizeof(flash_info_str)), .Type = DTYPE_String}
};

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
        DROP(wIndex);
        
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
                  Address = &DfuDeviceDescriptor;                  
                  Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
                  Address = &DfuConfigurationDescriptor;
                  Size    = sizeof(USB_Descriptor_DfuConfiguration_t);			
			break;
		case DTYPE_String:
			if (DescriptorNumber == 0x00)
			{
				Address = &LanguageString;
				Size    = LanguageString.Header.Size;
			}
			else if (DescriptorNumber == 0x01)
			{
				Address = &ManufacturerString;
				Size    = ManufacturerString.Header.Size;
			}
                        else if (DescriptorNumber == 0x02)
			{
				Address = &ProductString;
				Size    = ProductString.Header.Size;
			}
                        else if (DescriptorNumber == 0x03)
			{
                          unsigned int i;
                          
                          for(i = 0; i < sizeof(flash_info_str); i++)
                          {
                            NameString.UnicodeString[i] = flash_info_str[i];
                          }
                                                    
                          Address = &NameString;
                          Size    = NameString.Header.Size;
			}

			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

