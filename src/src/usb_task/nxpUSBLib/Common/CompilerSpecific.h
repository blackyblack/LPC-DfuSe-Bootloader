/*
* Copyright(C) NXP Semiconductors, 2011
* All rights reserved.
*
* Copyright (C) Dean Camera, 2011.
*
* LUFA Library is licensed from Dean Camera by NXP for NXP customers 
* for use with NXP's LPC microcontrollers.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and 
* all warranties, express or implied, including all implied warranties of 
* merchantability, fitness for a particular purpose and non-infringement of 
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in 
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no 
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
* 
* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' and its 
* licensor's relevant copyrights in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
*/



/** \file
 *  \brief Compiler specific definitions for code optimization and correctness.
 *
 *  \copydetails Group_CompilerSpecific
 *
 *  \note Do not include this file directly, rather include the Common.h header file instead to gain this file's
 *        functionality.
 */

/** \ingroup Group_Common
 *  \defgroup Group_CompilerSpecific Compiler Specific Definitions
 *  \brief Compiler specific definitions for code optimization and correctness.
 *
 *  Compiler specific definitions to expose certain compiler features which may increase the level of code optimization
 *  for a specific compiler, or correct certain issues that may be present such as memory barriers for use in conjunction
 *  with atomic variable access. 
 *
 *  Where possible, on alternative compilers, these macros will either have no effect, or default to returning a sane value
 *  so that they can be used in existing code without the need for extra compiler checks in the user application code.
 *
 *  @{
 */

#ifndef __NXPUSBLIB_COMPILERSPEC_H__
#define __NXPUSBLIB_COMPILERSPEC_H__

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_COMMON_H)
			#error Do not include this file directly. Include nxpUSBlib/Common/Common.h instead to gain this functionality.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			#if defined(__GNUC__) || defined(__DOXYGEN__)
				/** Forces GCC to use pointer indirection (via the device's pointer register pairs) when accessing the given
				 *  struct pointer. In some cases GCC will emit non-optimal assembly code when accessing a structure through
				 *  a pointer, resulting in a larger binary. When this macro is used on a (non \c const) structure pointer before
				 *  use, it will force GCC to use pointer indirection on the elements rather than direct store and load
				 *  instructions.
				 *
				 *  \param[in, out] StructPtr  Pointer to a structure which is to be forced into indirect access mode.
				 */
				#define GCC_FORCE_POINTER_ACCESS(StructPtr)   __asm__ __volatile__("" : "=b" (StructPtr) : "0" (StructPtr))

				/** Forces GCC to create a memory barrier, ensuring that memory accesses are not reordered past the barrier point.
				 *  This can be used before ordering-critical operations, to ensure that the compiler does not re-order the resulting
				 *  assembly output in an unexpected manner on sections of code that are ordering-specific.
				 */
				#define GCC_MEMORY_BARRIER()                  // FIXME __asm__ __volatile__("" ::: "memory");
				
				/** Evaluates to boolean true if the specified value can be determined at compile time to be a constant value
				 *  when compiling under GCC.
				 *
				 *  \param[in] x  Value to check compile time constantness of.
				 *
				 *  \return Boolean true if the given value is known to be a compile time constant, false otherwise.
				 */
				#define GCC_IS_COMPILE_CONST(x)               __builtin_constant_p(x)

                                #if !defined(__DOXYGEN__)
                                        #define __COMPILER_SPECIFIC(x)            GCC_ ## x
                                #endif
			#else
				#define GCC_FORCE_POINTER_ACCESS(StructPtr)
				#define GCC_MEMORY_BARRIER()
				#define GCC_IS_COMPILE_CONST(x)               0       
			#endif



                        #if defined(__ICCARM__)
                                #include <intrinsics.h>
                        
                                #define IAR_MEMORY_BARRIER()                  __asm("")
                                #if !defined(__DOXYGEN__)
                                        #define __IAR_ISR_PRAGMA(...)             _Pragma(#__VA_ARGS__)
                                #endif
                                
                                #define IAR_ISR(Name, Group, Level)           __IAR_ISR_PRAGMA(handler = Group, Level) __interrupt void Name (void)
                                
                                #define IAR_READ_SYS_REGISTER(Reg)            __get_system_register(Reg)
                                #define IAR_WRITE_SYS_REGISTER(Reg, Val)      __set_system_register(Reg, Val)
                                #define IAR_CLEAR_STATUS_FLAG(Bitmask)        __clear_status_flag(Bitmask)
                                #define IAR_SET_STATUS_FLAG(Bitmask)          __set_status_flag(Bitmask)

                                #if !defined(__DOXYGEN__)
                                        #define __COMPILER_SPECIFIC(x)            IAR_ ## x
                                #endif
                        #else
                                #define IAR_IS_PART_DEFINED(x)                0
                                #define IAR_ISR(Name, ...)                    
                                #define IAR_READ_SYS_REGISTER(Reg)       
                                #define IAR_WRITE_SYS_REGISTER(Reg, Val)      
                                #define IAR_CLEAR_STATUS_FLAG(Bitmask)        
                                #define IAR_SET_STATUS_FLAG(Bitmask)          
                        #endif

#endif

/** @} */

