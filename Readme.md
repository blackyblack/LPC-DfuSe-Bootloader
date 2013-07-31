A working DfuSe bootloader for LPC178X. IAR 6.5x workspace.

Board is set to 12 MHz crystal with debug on UART3 (P0.0 = TX, P0.1 = RX, P5.0 = DBG_EN pin).
Bootloader is placed at 0x00070000-0x0007FFFF address range.
User application is started from 0x00010000 address.
Bootloader has shared memory at 0x1000FF10 address.
Bootloader is run from RAM so it can update itself (or restored with UART Bootloader eg FlashMagic
if corrupted).

App is run in bootloader mode if BOOT_MODE pin (P0.21) is active, or if flash is empty at user app
address, or if a boot request is set in shared memory (0x1000FF18 == 1) and valid boot magic
value is set (0x1000FF10 == "boot001\0").

USB is detected as DFU device with VID = 0x0483, PID = 0xDF11.
Driver from /driver folder should be installed to work with the device in Windows (RedutDfuse.inf). 
Otherwise DFU could be accessed with WinUsb driver in Windows, STTub driver and STM Demonstrator in 
Windows or native libusb driver in Linux.

Linux: works with dfu-util 0.5 and higher.
Windows: works with dfu-util 0.5 and higher NOT in runtime mode. To work in runtime mode under Windows
dfu-util and libusbx should be taken from:
	https://github.com/blackyblack/dfu-util
	https://github.com/blackyblack/libusbx

Precompiled Windows version available at https://github.com/blackyblack/dfu-util.
