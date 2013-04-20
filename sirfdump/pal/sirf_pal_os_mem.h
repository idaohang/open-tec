/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_os_mem.h
 *
 * @brief  SiRF PAL dynamic memory module.
 */

#ifndef SIRF_PAL_OS_MEM_H_INCLUDED
#define SIRF_PAL_OS_MEM_H_INCLUDED

#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */




/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * @brief Initialize the memory allocation subsystem
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Init(void);

/**
 * @brief Teardown the memory allocation subsystem
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Destroy(void);

/**
 * @brief Allocate a block of memory
 *
 * This function allocates a block of memory and returns the start
 * of the allocation.
 *
 * @return  Address of allocated memory or NULL
 */
void *SIRF_PAL_OS_MEM_Alloc(tSIRF_UINT32 size);

/**
 * @brief Release an allocated memory block
 *
 * This function releases previously allocated memory
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The memory was released.
 *   - SIRF_FAILURE            - An error occurred.
 *
 * @param[in] pMem            Memory to release
 * @return                    Return value (see table)
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Free(void *pMem);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* !SIRF_PAL_OS_MEM_H_INCLUDED */

/**
 * @}
 * End of file.
 */


