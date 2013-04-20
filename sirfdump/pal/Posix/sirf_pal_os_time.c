/**
 * @addtogroup platform_src_sirf_pal_posix
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
 * @file   sirf_pal_os_time.c
 *
 * @brief  SiRF PAL time module.
 */

#include <stdio.h>
#include <time.h>

#include "sirf_types.h"
#include "sirf_pal.h"



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Get the current time from the system in milliseconds.
 * @return                    Relative milliseconds counter.
 */
tSIRF_UINT32 SIRF_PAL_OS_TIME_SystemTime( tSIRF_VOID )
{
   struct timespec time;

   if ( clock_gettime( CLOCK_REALTIME, &time ) != 0 )
      return 0;

   return ( time.tv_sec * 1000L + time.tv_nsec / 1000000L );

} /* SIRF_PAL_OS_TIME_SystemTime() */


/**
 * @brief Get the real-time clock value.
 * @param[out] date_time      Structure to populate with the RTC values.
 * @return                    Result code.
 */
tSIRF_RESULT SIRF_PAL_OS_TIME_DateTime( tSIRF_DATE_TIME *date_time )
{
   struct tm local;
   struct timespec ts;

   DEBUGCHK(date_time);

   if ( clock_gettime( CLOCK_REALTIME, &ts ) != 0 )
      return SIRF_FAILURE;
   
   local = *localtime(&ts.tv_sec);
   
   date_time->year         = 1900+local.tm_year;
   date_time->month        = 1+local.tm_mon;
   date_time->day          = local.tm_mday;
   date_time->minute       = local.tm_min;
   date_time->hour         = local.tm_hour;
   date_time->second       = local.tm_sec;
   date_time->milliseconds = ts.tv_nsec / 1000000L;

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_TIME_DateTime() */


 /**
 * @brief Get the UTC time.
 * @param[out] date_time      Structure to populate with the time values.
 * @return                    SIRF_FAILURE until function is implemented
 */
tSIRF_RESULT SIRF_PAL_OS_TIME_UTCDateTime( tSIRF_DATE_TIME *date_time )
{
   (void)date_time;
   return SIRF_FAILURE;
}


/**
 * @}
 * End of file.
 */


