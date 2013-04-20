/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2007-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal.h
 *
 * @brief  SiRF PAL API.
 */

/* Protect against multiple inclusions. */
#ifndef SIRF_PAL_H_INCLUDED
#define SIRF_PAL_H_INCLUDED



/* ----------------------------------------------------------------------------
 *   Included Files
 * ------------------------------------------------------------------------- */



/* Include the platform specific PAL header file. */
/* Thread declarations --------------------------------------------------------*/
#if defined ( OS_WIN32 ) || defined ( OS_WIN32_MFC) || defined ( OS_WINCE ) || \
    defined ( OS_QNX ) || defined ( OS_LINUX ) || defined ( OS_POSIX ) || defined ( OS_SYMBIAN ) || \
    defined ( OS_UCOSII )
#else
   #error No OS specified
#endif

/* Include the individual PAL header files. */
#include "sirf_pal_os_thread.h"
#include "sirf_pal_os_mem.h"
#include "sirf_pal_os_mutex.h"
#include "sirf_pal_os_semaphore.h"
#include "sirf_pal_os_time.h"
#include "sirf_pal_com.h"
#include "sirf_pal_storage.h"
#include "sirf_pal_hw_reset.h"
#include "sirf_pal_hw_on.h"
#include "sirf_pal_debug.h"
#include "sirf_pal_log.h"
#include "sirf_pal_tcpip.h"
#include "sirf_threads.h"
#include "sirf_pal_hw_frequency_aiding.h"



/* ----------------------------------------------------------------------------
 *   Preprocessor Definitions
 * ------------------------------------------------------------------------- */



/** Generic PAL error code. */
#define SIRF_PAL_OS_ERROR                   (0x2201)
/** API usage error code.  This is returned if a NULL pointer is passed or
 * an invalid parameter is used to a PAL function
 */
#define SIRF_PAL_API_ERROR                  (0x2202)

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * @brief Inititalize all PAL subsystems
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_Init(void);

/**
 * @brief Perform teardown (if necessary) on all PAL subsystems
 *
 * @return SIRF_SUCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_Destroy(void);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* !SIRF_PAL_H_INCLUDED */

/**
 * @}
 * End of file.
 */



