/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_os_thread.h
 *
 * @brief  SiRF PAL thread module.
 */

#ifndef SIRF_PAL_OS_THREAD_H_INCLUDED
#define SIRF_PAL_OS_THREAD_H_INCLUDED

#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */

/* Thread compatibility declarations */
#if defined ( OS_WIN32 ) || defined ( OS_WIN32_MFC ) || defined ( OS_WINCE )
   #define tSIRF_THREAD                   tSIRF_HANDLE
   #define SIRF_PAL_OS_THREAD_PARAMS      void *param
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  unsigned long
   #define SIRF_PAL_OS_THREAD_CALLTYPE    __stdcall
   #define SIRF_PAL_OS_THREAD_UNUSED     (void)param;
   #define SIRF_PAL_OS_THREAD_START()
   #define SIRF_PAL_OS_THREAD_RETURN()    return 0;
   /* note: DEBUGMSG must not be called on Win32 while thread is exiting */
#elif defined ( OS_NUCLEUS )
   #define tSIRF_THREAD                   tSIRF_HANDLE
   #define SIRF_PAL_OS_THREAD_PARAMS      unsigned int argc, void *argv
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED     (void)argc; (void)argv;
   #define SIRF_PAL_OS_THREAD_START
   #define SIRF_PAL_OS_THREAD_RETURN
#elif defined ( OS_ECOS )
   #define tSIRF_THREAD                   tSIRF_UINT32
   #define SIRF_PAL_OS_THREAD_PARAMS      unsigned int index
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED     (void)index;
   #define SIRF_PAL_OS_THREAD_START()
   #define SIRF_PAL_OS_THREAD_RETURN
#elif defined ( OS_QNX ) || defined( OS_POSIX ) || defined ( OS_LINUX )
   #define tSIRF_THREAD                   tSIRF_HANDLE
   #define SIRF_PAL_OS_THREAD_PARAMS      void *param
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void *
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED     (void)param;
   #define SIRF_PAL_OS_THREAD_START()     SIRF_PAL_OS_THREAD_Entry();
   #define SIRF_PAL_OS_THREAD_RETURN()    return SIRF_PAL_OS_THREAD_Return();
#elif defined( OS_SYMBIAN )
   #define tSIRF_THREAD                   int
   #define SIRF_PAL_OS_THREAD_PARAMS      void *param
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  int
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED
   #define SIRF_PAL_OS_THREAD_START()     void *ThreadCleanupStack; SIRF_PAL_OS_THREAD_Entry( &ThreadCleanupStack );
   #define SIRF_PAL_OS_THREAD_RETURN()    return SIRF_PAL_OS_THREAD_Return( ThreadCleanupStack );
#elif defined ( OS_RTXC )
   #define tSIRF_THREAD                   tSIRF_HANDLE
   #define SIRF_PAL_OS_THREAD_PARAMS      void
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED     (void)param;
   #define SIRF_PAL_OS_THREAD_START
   #define SIRF_PAL_OS_THREAD_RETURN
#elif defined ( OS_UCOSII )
   #define tSIRF_THREAD                   unsigned char
   #define SIRF_PAL_OS_THREAD_PARAMS      unsigned int argc, void *argv
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED   
   #define SIRF_PAL_OS_THREAD_START()
   #define SIRF_PAL_OS_THREAD_RETURN()    SIRF_PAL_OS_THREAD_Return();
#elif defined ( OS_FREERTOS )
   #define tSIRF_THREAD                   tSIRF_HANDLE
   #define SIRF_PAL_OS_THREAD_PARAMS      unsigned int argc, void *argv
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_UNUSED      
   #define SIRF_PAL_OS_THREAD_START
   #define SIRF_PAL_OS_THREAD_RETURN
#elif defined ( OS_VDK )
   #define tSIRF_THREAD                   tSIRF_UINT32
   #define SIRF_PAL_OS_THREAD_PARAMS      void
   #define SIRF_PAL_OS_THREAD_RETURNTYPE  void
   #define SIRF_PAL_OS_THREAD_CALLTYPE
   #define SIRF_PAL_OS_THREAD_START
   #define SIRF_PAL_OS_THREAD_RETURN
#else
   #error No OS specified
#endif


/** Thread declaration macro. */
#define SIRF_PAL_OS_THREAD_DECL SIRF_PAL_OS_THREAD_RETURNTYPE SIRF_PAL_OS_THREAD_CALLTYPE



/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

tSIRF_RESULT SIRF_PAL_OS_THREAD_Create( tSIRF_UINT32 thread_id, tSIRF_HANDLE function_ptr, tSIRF_THREAD *thread_handle );
tSIRF_RESULT SIRF_PAL_OS_THREAD_Delete( tSIRF_THREAD thread_handle );

tSIRF_VOID SIRF_PAL_OS_THREAD_Sleep( tSIRF_UINT32 milliseconds );


/* Additional function(s): */

#ifdef OS_SYMBIAN
   void SIRF_PAL_OS_THREAD_Entry( void **cleanup_stack );
   SIRF_PAL_OS_THREAD_RETURNTYPE SIRF_PAL_OS_THREAD_Return( void *cleanup_stack );
#else
   tSIRF_VOID SIRF_PAL_OS_THREAD_Entry( tSIRF_VOID );
   SIRF_PAL_OS_THREAD_RETURNTYPE SIRF_PAL_OS_THREAD_Return( tSIRF_VOID );
#endif


/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


/* Operating System Includes for Windows */
#if defined ( OS_WIN32 ) || defined ( OS_WIN32_MFC ) || defined ( OS_WINCE )
   #ifndef STRICT
      #define STRICT
   #endif

   #if defined( OS_WINCE ) && defined( OS_WIN32 )
      #error ERROR: Both OS_WINCE and OS_WIN32 defined
   #endif

   #ifdef OS_WINCE
      #include <windows.h>
   #else
      #if defined(_ATL_DLL) || defined(_ATL_STATIC_REGISTRY)
         #define _ATL_FREE_THREADED
         #include <atlbase.h>
      #else
         /* #include <afx.h> */
      #endif
   #endif

#endif /* defined ( OS_WIN32 ) || defined ( OS_WIN32_MFC ) || defined ( OS_WINCE ) */



#endif /* !SIRF_PAL_OS_THREAD_H_INCLUDED */

/**
 * @}
 * End of file.
 */


