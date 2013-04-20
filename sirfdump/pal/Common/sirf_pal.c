/**
 * @addtogroup platform_src_sirf_pal_common
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2008-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal.c
 *
 * @brief  SiRF PAL high level common functions.
 */

#include "sirf_types.h"
#include "sirf_pal.h"



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Inititalize all PAL subsystems
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_Init(void)
{
   tSIRF_RESULT result;

   result = SIRF_PAL_OS_MEM_Init();
   if (SIRF_SUCCESS != result)
   {
      return result;
   }

   result = SIRF_PAL_NET_Init();
   if (SIRF_SUCCESS != result)
   {
      /* ignore the result from SIRF_PAL_OS_MEM_Destory as we want to return
       * the error from SIRF_PAL_NET_Init */
      (void)SIRF_PAL_OS_MEM_Destroy();
      return result;
   }

   return result;
}

/**
 * @brief Perform teardown (if necessary) on all PAL subsystems
 *
 * @return SIRF_SUCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_Destroy(void)
{
   tSIRF_RESULT first_failure = SIRF_SUCCESS;
   tSIRF_RESULT result = SIRF_SUCCESS;

   result = SIRF_PAL_NET_Destroy();
   if (SIRF_SUCCESS != result)
   {
      DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_NET_Destroy error %d\n"),result));
      first_failure = result;
   }

   result = SIRF_PAL_OS_MEM_Destroy();
   if (SIRF_SUCCESS != result)
   {
      DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_OS_MEM_Destroy error %d\n"),result));
      if (SIRF_SUCCESS == first_failure)
      {
         first_failure = result;
      }
   }

   return first_failure;
}

/**
 * @}
 * End of file.
 */


