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
 * @file   sirf_pal_os_semaphore.h
 *
 * @brief  SiRF PAL semaphore API.
 */

#ifndef SIRF_PAL_OS_SEMAPHORE_H_INCLUDED
#define SIRF_PAL_OS_SEMAPHORE_H_INCLUDED



/* ----------------------------------------------------------------------------
 *    Included Files
 * ------------------------------------------------------------------------- */



/* Include the standard SiRF types. */
#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */



/** Max count of a semaphore: */
#define SIRF_PAL_OS_SEMAPHORE_MAXCOUNT          32000L

/** Return code indicating timeout waiting for semaphore. */
#define SIRF_PAL_OS_SEMAPHORE_WAIT_TIMEOUT   0x2202

/* Infinite timeout: */
#define SIRF_PAL_OS_TIMEOUT_INFINITE            0xFFFFFFFF



/* ----------------------------------------------------------------------------
 *    Types
 * ------------------------------------------------------------------------- */



/** SiRF Semaphore type. */
typedef tSIRF_HANDLE tSIRF_SEMAPHORE;



/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/



/**
 * @brief Create a semaphore.
 *
 * This function creates a semaphore and returns a handle to it in the 
 * sem_handle output parameter.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - A valid semaphore handle was returned.
 *   - SIRF_PAL_OS_ERROR       - Out of resources.
 * 
 * @param[out] sem_handle     Handle to hold the new semaphore.
 * @param[in]  init_value     Initial semaphore count.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Create(
      tSIRF_SEMAPHORE *sem_handle, 
      tSIRF_UINT32     init_value);

/**
 * @brief Delete an allocated semaphore.
 *
 * This function deletes a semaphore previously created by the 
 * SIRF_PAL_OS_SEMAPHORE_Create function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS           - The semaphore was delete.
 *   - SIRF_PAL_OS_ERROR      - The semaphore was invalid.
 *
 * @param[in] sem_handle      Handle to the semaphore to delete.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Delete(
      tSIRF_SEMAPHORE sem_handle );

/**
 * @brief Wait on a semaphore.
 *
 * This function waits on a semaphore. On success the calling thread now
 * owns a semaphore reference count that should be released using the
 * SIRF_PAL_OS_SEMAPHORE_Release function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS                       - The wait succeeded.
 *   - SIRF_PAL_OS_SEMAPHORE_WAIT_TIMEOUT - The wait timed out.
 *   - SIRF_PAL_OS_ERROR                  - An error occured.
 *
 * @param[in] sem_handle      Handle to the semaphore to wait on.
 * @param[in] timeout         Timeout value for the wait operation.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Wait(
      tSIRF_SEMAPHORE sem_handle, 
      tSIRF_UINT32    timeout);

/**
 * @brief Release a semaphore reference count.
 *
 * This function releases a single semaphore reference count.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS                       - The wait succeeded.
 *   - SIRF_PAL_OS_ERROR                  - An error occured.
 *
 * @param[in] sem_handle      Handle to the semaphore to release.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Release(
      tSIRF_SEMAPHORE sem_handle);



/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_OS_SEMAPHORE_H_INCLUDED */



/**
 * @}
 * End of file.
 */


