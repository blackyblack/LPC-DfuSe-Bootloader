@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"D:\IAR\Embedded Workbench 6.5\common\bin\cspybat" "D:\IAR\Embedded Workbench 6.5\arm\bin\armproc.dll" "D:\IAR\Embedded Workbench 6.5\arm\bin\armsim2.dll"  %1 --plugin "D:\IAR\Embedded Workbench 6.5\arm\bin\armbat.dll" --macro "D:\IAR\Embedded Workbench 6.5\arm\config\debugger\NXP\Trace_LPC177x_LPC178x.dmac" --backend -B "--endian=little" "--cpu=Cortex-M3" "--fpu=None" "-p" "D:\IAR\Embedded Workbench 6.5\arm\CONFIG\debugger\NXP\LPC1788.ddf" "--semihosting=none" "--device=LPC1788" 


