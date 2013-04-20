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
 * @file   sirf_pal_os_mutex.h
 *
 * @brief  SiRF PAL mutex API.
 */

#ifndef SIRF_PAL_OS_MUTEX_H_INCLUDED
#define SIRF_PAL_OS_MUTEX_H_INCLUDED



/* ----------------------------------------------------------------------------
 *    IncludedFiles
 * ------------------------------------------------------------------------- */



#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Types
 * ------------------------------------------------------------------------- */



/** SiRF Mutex type. */
typedef tSIRF_HANDLE tSIRF_MUTEX;



/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * @brief Create a mutex and return a handle to it.
 *
 * This function creates a mutex and returns a handle to it in the 
 * mx_handle output parameter.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - A valid mutex handle was returned.
 *   - SIRF_PAL_OS_ERROR       - Out of resources.
 *
 * Once this function returns successfully, the following operations
 * may be performed:
 *   - SIRF_PAL_OS_MUTEX_Delete
 *   - SIRF_PAL_OS_MUTEX_Enter
 *
 * @param[out] mx_handle      Created mutex handle.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_MUTEX_Create(
      tSIRF_MUTEX *mx_handle);

/**
 * @brief Delete an allocated mutex.
 *
 * This function deletes a mutex previously created by the 
 * SIRF_PAL_OS_MUTEX_Create function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The mutex was delete.
 *   - SIRF_PAL_OS_ERROR       - The mutex was invalid.
 *
 * @param[in] mx_handle       Mutex to delete.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_MUTEX_Delete(
      tSIRF_MUTEX mx_handle);

/**
 * @brief Enter a mutex.
 *
 * This function enters a mutex created by the SIRF_PAL_OS_MUTEX_Create.
 * function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The mutex was entered.
 *   - SIRF_PAL_OS_ERROR       - The mutex is invalid.
 *
 * Once this function returns successfully, only operation that may be
 * performed on the mutex is SIRF_PAL_OS_MUTEX_Exit.
 *
 * @param[in] mx_handle       Mutex to enter.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_MUTEX_Enter(
      tSIRF_MUTEX mx_handle);

/**
 * @brief Exit a mutex.
 *
 * This function exits a mutex previously entered by the
 * SIRF_PAL_OS_MUTEX_Enter function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The mutex was exited.
 *   - SIRF_PAL_OS_ERROR       - The mutex is invalid.
 *
 * Once this function returns successfully, the following operations
 * may be performed:
 *   - SIRF_PAL_OS_MUTEX_Delete
 *   - SIRF_PAL_OS_MUTEX_Enter
 *
 * @param[in] mx_handle       Mutex to enter.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_MUTEX_Exit(
      tSIRF_MUTEX mx_handle);



/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_OS_MUTEX_H_INCLUDED */

/**
 * @}
 * End of file.
 */


