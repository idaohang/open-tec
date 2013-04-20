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


static tSIRF_DATE_TIME pal_time_struct;

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR   (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY    (SECONDS_PER_HOUR * 24)
#define SECONDS_PER_YEAR   (SECONDS_PER_DAY * 365)

static struct tm mytm;

static tSIRF_INT32 DMonth[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };
static tSIRF_INT32 monthCodes[12] = { 6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

static tSIRF_INT32 calc_day_of_week(const struct tm* nTM);
static tSIRF_INT32 is_leap_year(tSIRF_INT32 year);
static tSIRF_INT32 num_leap_years(tSIRF_INT32 years);
static struct tm*  my_gmtime(const time_t *tim);
static time_t      my_mktime(struct tm *t);

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */


/**
 * @brief PAL WinCE implementation for the gmtime function.
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
   struct tm* res;
   time_t timer_t = (time_t)*timer;

   res = my_gmtime(&timer_t);

   pal_time_struct.year          = res->tm_year + 1900;
   pal_time_struct.month         = res->tm_mon + 1;
   pal_time_struct.day           = res->tm_mday;
   pal_time_struct.hour          = res->tm_hour;
   pal_time_struct.minute        = res->tm_min;
   pal_time_struct.second        = res->tm_sec;
   pal_time_struct.milliseconds  = 0;

   return &pal_time_struct;
}
 
/**
 * @brief PAL WinCE implementation for the mktime function
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

   return (tSIRF_TIME_T) my_mktime(&libc_struct);
}


/**
 * @brief Computes the day of the week
 *
 * Computes the day of the week for a tm structure.
 *
 * @param nTM Pointer to the tm structure to which the day of the week will be calculated.
 *
 * @return An integer (from 0 to 6) representing the calculated day of the week.
 */
static tSIRF_INT32 calc_day_of_week(const struct tm* nTM)
{
   tSIRF_INT32 day;
   
   day = (nTM->tm_year%100);
   day += day/4;
   day += monthCodes[nTM->tm_mon];
   day += nTM->tm_mday;
   while(day>=7)
      day -= 7;

   return day;
}

/**
 * 
 */
/**
 * @brief Computes the number of leap years to the current date.
 *
 * Takes in the number of years since 1900, then returns the number of years
 * that were leap years since 1900.
 *
 * @param years number of years since 1900.
 *
 * @return number of years that were leap years since 1900.
 */
static tSIRF_INT32 num_leap_years(tSIRF_INT32 years)
{
   tSIRF_INT32 four_years, hundred_years, four_hundred_years, leap_years;
   /* first add to the nearest 400 year mark */

   years += 300;  /* Set years to elapsed years since 1600 */
   four_years = years / 4;
   hundred_years = years / 100;
   four_hundred_years = years / 400;
   leap_years = four_years - hundred_years + four_hundred_years - 72;
   return leap_years; 
}

/**
 * @brief Determines if a year is a leap year or not.
 *
 * Determines if the specified year is a leap year or not.
 *
 * @param year The number of the year to be determined, since 1900.
 *
 * @return 1 if the specified year is a leap year; 0 otherwise.
 */
static tSIRF_INT32 is_leap_year(tSIRF_INT32 year)
{
   tSIRF_INT32 four_years, hundred_years, four_hundred_years, leap_year;
   /* first add to the nearest 400 year mark */
   year += 300;  /* Set years to elapsed years since 1600 */
   four_years = (year % 4) == 0;
   hundred_years = (year % 100) == 0;
   four_hundred_years = (0 == (year % 400));
   leap_year = four_years && (!hundred_years || four_hundred_years);
   return leap_year; 
}

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
 * @return Pointer to a structure of type struct tm. The fields of the 
 *         returned structure hold the evaluated value of the timer argument 
 *         in UTC rather than in local time.
 */
static struct tm *my_gmtime(const time_t *tim)
{
   tSIRF_TIME_T timer = (tSIRF_TIME_T)*tim;
   tSIRF_INT32 yday, yrs;
   tSIRF_INT32 lpyr = 0; 

   /* First figure out the year since 1900.  Ignore leap years first */
   yrs = (tSIRF_INT32)((timer / SECONDS_PER_YEAR) + 70);
   timer -= (yrs - 70) * SECONDS_PER_YEAR;
   /* correct for leap years, don't include current year */
   timer -= (num_leap_years(yrs-1) - 17) * SECONDS_PER_DAY;
  
   /* Check for underflow */
   if (timer < 0) 
   {
      timer += SECONDS_PER_YEAR;
      yrs--;
      if (is_leap_year(yrs))
      {
         timer += SECONDS_PER_DAY;
         lpyr++;
      }
   } 
   else if (is_leap_year(yrs)) 
   {
      lpyr++;
   }

   mytm.tm_year = yrs;
   /* Number of days since January 1 is yday */
   mytm.tm_yday = (tSIRF_INT32)(timer / SECONDS_PER_DAY);
   /* Number of seconds left in that day */
   timer -= mytm.tm_yday * SECONDS_PER_DAY;

   /* Convert for the month array, wich is a normal year */
   yday = mytm.tm_yday;
   if (mytm.tm_yday > (DMonth[2] + 1))
   {
      yday -= lpyr;
   }

   /* incriment the month appropriately */
   for (mytm.tm_mon = 0; DMonth[mytm.tm_mon] < yday; mytm.tm_mon++) ;

   mytm.tm_mon--; /* Correct for 0 based month; */
   
   /* Set the month day */
   mytm.tm_mday = yday - DMonth[mytm.tm_mon];
   
   /* mday currently has num of days since beginnign of month.  Add 1 */
   mytm.tm_mday++;
   
   /* Minutes and seconds */
   mytm.tm_hour = (tSIRF_INT32)(timer / SECONDS_PER_HOUR);
   timer -= mytm.tm_hour * SECONDS_PER_HOUR;
   mytm.tm_min = (tSIRF_INT32)(timer / SECONDS_PER_MINUTE);
   timer -= mytm.tm_min * SECONDS_PER_MINUTE;
   mytm.tm_sec = (tSIRF_INT32)timer;
   
   /* Calculate day of week */
   mytm.tm_wday = calc_day_of_week(&mytm);
   mytm.tm_isdst = 0;
   
   return &mytm;
}

static tSIRF_INT32 month_to_day[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/**
 * @brief Commom implementation of the LIBC mktime function
 *
 * Convert the local time to a calendar value.
 *
 * @param t Pointer to time structure of type struct tm.
 *
 * @return the specified calendar time encoded as a value of type time_t.
 */
static time_t my_mktime(struct tm *t)
{
   tSIRF_INT16  month, year;
   time_t result;

   month = t->tm_mon;
   year = t->tm_year + month / 12 + 1900;
   month %= 12;

   if (month < 0)
   {
      year -= 1;
      month += 12;
   }
   result = (year - 1970) * 365 + (year - 1969) / 4 + month_to_day[month];
   result = (year - 1970) * 365 + month_to_day[month];
   if (month <= 1)
   {
      year -= 1;
   }

   result += (year - 1968) / 4;
   result -= (year - 1900) / 100;
   result += (year - 1600) / 400;
   result += t->tm_mday;
   result -= 1;
   result *= 24;
   result += t->tm_hour;
   result *= 60;
   result += t->tm_min;
   result *= 60;
   result += t->tm_sec;

   return result;
}

/*****/

/**
 * @}
 * End of file.
 */


