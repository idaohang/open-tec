/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2008 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_threads.h
 *
 * @brief  SiRF threads.
 */

#ifndef SIRF_THREADS_H_INCLUDED
#define SIRF_THREADS_H_INCLUDED



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */



#include "sirf_types.h"



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/



/* ----------------------------------------------------------------------------
 *   Types
 * ------------------------------------------------------------------------- */



/**
 * @brief Thread definition structure.
 */
typedef struct
{
   tSIRF_UINT32 thread_id;
   tSIRF_INT32  thread_priority;
   tSIRF_UINT32 thread_quantum;
   tSIRF_UINT32 thread_stack_size;

} tSIRF_THREAD_TABLE;



/* ----------------------------------------------------------------------------
 *   Global Variables
 * ------------------------------------------------------------------------- */



extern tSIRF_THREAD_TABLE  SIRF_THREAD_Table[];



/* ----------------------------------------------------------------------------
 *   Function Prototypes
 * ------------------------------------------------------------------------- */



extern tSIRF_UINT32 SIRF_THREAD_MaxThreads( tSIRF_VOID );



/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_THREADS_H_INCLUDED */

/**
 * @}
 * End of file.
 */


