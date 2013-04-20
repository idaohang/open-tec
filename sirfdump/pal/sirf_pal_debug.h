/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2008 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_pal_debug
 *
 * @brief  SiRF PAL debugging API.
 */

#ifndef SIRF_PAL_DEBUG_H_INCLUDED
#define SIRF_PAL_DEBUG_H_INCLUDED



/* ----------------------------------------------------------------------------
 *   Preprocessor Definitions
 * ------------------------------------------------------------------------- */

#define SIRF_PAL_DEBUG_MSGBUFF_LENGTH        (300)


/******************************************************************************
 DEBUGMSG(l,m)

 Outputs a Debug message to standard device. On Window CE, debug output can be
 routed to file, console, or serial port using a debug service. On Nucleus,
 debug output goes to auxiliary serial port.

 Parameters:
   l - message level:   (ignore for now)
   m - printf-style parameter list enclosed in parentheses, for example
         (DEBUGTXT("size=%d"), lSize)

 Example:   DEBUGMSG(1, (DEBUGTXT("Error: Packet length exceeds %d"), bufsize));

 --------------------------------------------------------------------------

 DEBUGCHK(x)

 Description: checks if x is TRUE, and if not => dumps a debug message and
   condition that failed.
 Parameters:
 x - condition to be evaluated - must be evaluated EXACTLY ONCE.

 NOTE: Each platform has to define DEBUGCHK(x) that executes x exactly once.
 For example: #define DEBUGCHK(x) if (!x) printf("error: assertion failed\n")

*******************************************************************************/

#ifdef DEBUG

   #if defined( OS_QNX )

      #include <stdio.h>
      #include <sys/slog.h>
      #include <assert.h>

      #define DEBUGMSG(l,m) slogf m
      #define DEBUGTXT(x) 0, _SLOG_WARNING, x
      #define DEBUGCHK(x) assert((x))


   #elif defined( OS_LINUX ) || defined( OS_POSIX ) || defined( OS_SYMBIAN )

      #include <stdio.h>
      #include <syslog.h>
      #include <assert.h>

      #define DEBUGMSG(l,m) syslog m
      #define DEBUGTXT(x) LOG_WARNING, x
      #define DEBUGCHK(x) assert((x))


   #elif defined( OS_WINCE )

      #include <DbgApi.h>
      #define DEBUGTXT(x) TEXT(x)
      #if   defined( CPU_MIPS )
         #define DEBUGBREAK __asm("break 1")
      #elif defined( CPU_i486 ) || defined( CPU_X86 )
         #define DEBUGBREAK __asm int 3
      #else
         #define DEBUGBREAK DebugBreak()
      #endif
      /* note: DEBUGMSG must not be called on Win32 while thread is exiting */

   #elif defined( OS_WIN32 ) || defined ( OS_WIN32_MFC )
      #if defined(_ATL_DLL) || defined(_ATL_STATIC_REGISTRY)
         /* ATL debug: */
         #define DEBUGMSG(l,m) ATLTRACE m
         #define DEBUGCHK(x) ATLASSERT(x)
         #define DEBUGTXT(x) TEXT(x)
      #else
#         include "user_interface.h"
#         include <assert.h>
#         define DEBUGMSG(l,m) if(l) GUI_Print m
#         define DEBUGCHK(x) assert(x)
         #define DEBUGTXT(x) (x)
      #endif
      /* note: DEBUGMSG must not be called on Win32 while thread is exiting */

      #define DEBUGBREAK __asm int 3


   #elif defined( OS_NUCLEUS )

      #define DEBUGTXT(s) debug_buffer, s
      extern int umDebugPrintf( char *pFormat, ... );

      #define DEBUGMSG(l, m) \
      { \
         char debug_buffer[300]; \
         sprintf m;            \
         umDebugPrintf( debug_buffer );\
      }

      #define DEBUGCHK(x) if (!(x)) while(1)

   #elif defined( OS_ECOS )

      #include <cyg/kernel/kapi.h>

      #define DEBUGTXT(s) debug_buffer, s
      extern int umDebugPrintf( char *pFormat, ... );

      extern cyg_sem_t debug_msg_sem;

      #define DEBUGMSG(l, m) \
      { \
         char debug_buffer[300]; \
         cyg_semaphore_wait( &debug_msg_sem); \
         sprintf m; \
         umDebugPrintf( debug_buffer ); \
         cyg_semaphore_post( &debug_msg_sem); \
      }

      #define DEBUGCHK(x) if (!(x)) while(1)

   #elif defined( OS_UCOSII )

      #define DEBUGTXT(s) debug_buffer, s
      extern int umDebugPrintf( char *pFormat, ... );

      #define DEBUGMSG(l, m) \
      { \
         char debug_buffer[300]; \
         sprintf m;            \
         umDebugPrintf( debug_buffer );\
      }

      #define DEBUGCHK(x) if (!(x)) while(1)

   #elif defined( OS_RTXC )

      #define DEBUGTXT(s) debug_buffer, s
      extern int umDebugPrintf( char *pFormat, ... );

      #define DEBUGMSG(l, m) \
      { \
         char debug_buffer[300]; \
         sprintf m;            \
         umDebugPrintf( debug_buffer );\
      }

      #define DEBUGCHK(x) if (!(x)) while(1)

   #elif defined( OS_FREERTOS )

      #define DEBUGTXT(s) debug_buffer, s
      extern int umDebugPrintf( char *pFormat, ... );

      #define DEBUGMSG(l, m) \
      { \
         char debug_buffer[300]; \
         sprintf m;            \
         umDebugPrintf( debug_buffer );\
      }

      #define DEBUGCHK(x) if (!(x)) while(1)

   #elif defined( OS_VDK )

      #define DEBUGMSG(l,m) printf m
      #define DEBUGTXT(x)  x
      #define DEBUGCHK(x) (void)0;

   #else

      #error No OS specified

   #endif

#else

   #ifndef DEBUGMSG
      #if defined(TOOLCHAIN_VC8)
         #define DEBUGMSG(l,m) (void) 0
      #else
         #define DEBUGMSG(l,m) do {} while (0)
      #endif
      #define DEBUGTXT(s) (void)0
   #endif

   #ifndef DEBUGCHK
      #if defined(TOOLCHAIN_VC8)
         #define DEBUGCHK(x) (void)0
      #else
         #define DEBUGCHK(x) do {} while (0)
      #endif
   #endif
   
   #ifndef VERIFY
      #if defined(TOOLCHAIN_VC8)
         #define DEBUGBREAK (void)0
         #define VERIFY(x) ((void)(x))
      #else
         #define DEBUGBREAK do {} while (0)
         #define VERIFY(x) do {} while (0)
      #endif
   #endif

#endif /* DEBUG */



#endif /* !SIRF_PAL_DEBUG_H_INCLUDED */

/**
 * @}
 * End of file.
 */


