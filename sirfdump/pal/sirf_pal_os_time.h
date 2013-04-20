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
 * @file   sirf_pal_os_time.h
 *
 * @brief  SiRF PAL time API.
 */

#ifndef SIRF_PAL_OS_TIME_H_INCLUDED
#define SIRF_PAL_OS_TIME_H_INCLUDED


#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Types
 * ------------------------------------------------------------------------- */



/** SiRF Date & Time structure. */
typedef struct tSIRF_DATE_TIME_tag
{
   tSIRF_UINT16 year;         /**< Year (1970, 2000, etc) */
   tSIRF_UINT8  month;        /**< Month (1 = January, 12 = December) */
   tSIRF_UINT8  day;          /**< Day of Month (1 - 31) */
   tSIRF_UINT8  hour;         /**< Hour of day (0 - 23) */
   tSIRF_UINT8  minute;       /**< Minute (0 - 59) */
   tSIRF_UINT8  second;       /**< Second (0 - 59) */
   tSIRF_UINT16 milliseconds; /**< Millisecond (0 - 999) */

} tSIRF_DATE_TIME;

typedef tSIRF_INT64 tSIRF_TIME_T;

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

tSIRF_UINT32 SIRF_PAL_OS_TIME_SystemTime( tSIRF_VOID );

tSIRF_RESULT SIRF_PAL_OS_TIME_DateTime( tSIRF_DATE_TIME *date_time );

tSIRF_RESULT SIRF_PAL_OS_TIME_UTCDateTime( tSIRF_DATE_TIME *date_time );

tSIRF_RESULT SIRF_PAL_OS_TIME_RTCRead( tSIRF_UINT16 *weekno, tSIRF_INT32 *timeOfWeek );


/* Utility time functions normally available in LIBC */

tSIRF_DATE_TIME* SIRF_PAL_OS_TIME_gmtime(const tSIRF_TIME_T *timer);

tSIRF_TIME_T SIRF_PAL_OS_TIME_mktime(tSIRF_DATE_TIME *tmbuf);


/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_OS_TIME_H_INCLUDED */

/**
 * @}
 */


