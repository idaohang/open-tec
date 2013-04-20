/**
 * @addtogroup platform_src_sirf_pal_common
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
 * @file   sirf_pal_os_time_libc.c
 *
 * @brief  SiRF PAL implementation of time utility functions based on the native LIBC time functions.
 */

#include "sirf_types.h"
#include "sirf_pal.h"
#include <time.h>


/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

static tSIRF_DATE_TIME pal_time_struct;
 
/**
 * @brief Commom implementation of the LIBC gmtime function.
 *
 * Convert a time value to a structure. 
 *
 *
 * @param timer Pointer to stored time. The time is represented as seconds 
 *              elapsed since midnight (00:00:00), January 1, 1970, 
 *              coordinated universal time (UTC).
 *
 * @return Pointer to a structure of type tSIRF_DATE_TIME. The fields of the 
 *         returned structure hold the evaluated value of the timer argument 
 *         in UTC rather than in local time.
 */
tSIRF_DATE_TIME* SIRF_PAL_OS_TIME_gmtime(const tSIRF_TIME_T *timer)
{
   struct tm       *libc_struct;
   time_t          libc_time_t;

   libc_time_t = *timer;

   libc_struct = gmtime(&libc_time_t);

   /* add casting to avoid run time overflow */
   pal_time_struct.year          = (tSIRF_UINT16) (libc_struct->tm_year + 1900);
   pal_time_struct.month         = (tSIRF_UINT8) (libc_struct->tm_mon + 1);
   pal_time_struct.day           = (tSIRF_UINT8) (libc_struct->tm_mday);
   pal_time_struct.hour          = (tSIRF_UINT8) (libc_struct->tm_hour);
   pal_time_struct.minute        = (tSIRF_UINT8) (libc_struct->tm_min);
   pal_time_struct.second        = (tSIRF_UINT8) (libc_struct->tm_sec);
   pal_time_struct.milliseconds  = 0;

   return &pal_time_struct;
}
 
/**
 * @brief Commom implementation of the LIBC mktime function
 *
 * Convert the local time to a calendar value.
 *
 * @param timeptr Pointer to time structure of type tSIRF_DATE_TIME.
 *
 * @return the specified calendar time encoded as a value of type tSIRF_TIME_T.
 */
tSIRF_TIME_T SIRF_PAL_OS_TIME_mktime(tSIRF_DATE_TIME *timeptr)
{
   struct tm  libc_struct;

   libc_struct.tm_year  = timeptr->year - 1900;
   libc_struct.tm_mon   = timeptr->month - 1;
   libc_struct.tm_mday  = timeptr->day;
   libc_struct.tm_hour  = timeptr->hour;
   libc_struct.tm_min   = timeptr->minute;
   libc_struct.tm_sec   = timeptr->second;
   libc_struct.tm_isdst = 0;

   return (tSIRF_TIME_T) mktime(&libc_struct);
}


/**
 * @}
 * End of file.
 */


