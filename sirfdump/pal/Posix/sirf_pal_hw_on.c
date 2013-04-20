/**
 * @addtogroup sirf_pal_posix
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
 * @file   sirf_pal_hw_reset.c
 *
 * @brief  Tracker reset control module.
 */
#include <stddef.h>
#include "sirf_types.h"
#include "sirf_pal.h"
#include "sirf_pal_hw_on.h"
#include <fcntl.h>

#define PULLDOWN  2
#define PULLUP    3


/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

/** a global to hold the handler for tracker On Pulse port
 * This is purposfully not published as it is strictly for setting by the
 * host application.  It is also not needed in all implemenations.
 */
tSIRF_COM_HANDLE SIRF_PAL_TrackerOnPort = NULL;

/**
 * @brief Set the On line to high or low 
 * @param[in] level              SIRF_PAL_HW_ON_HIGH or SIRF_PAL_HW_ON_LOW
 * @return                       SIRF_SUCCESS or SIRF_FAILURE
 */
tSIRF_RESULT SIRF_PAL_HW_TrackerOnLine ( tSIRF_UINT32 level )
{
   int fd=-1;
   int result=-1;
   tSIRF_RESULT returnValue = SIRF_FAILURE;
   /* The GSD3T Tracker inverts these signals so ON_OFF HIGH is 0 volts
    * and reset LO is 3 volts.  Thus we should ClrRTS to bring the 
    * signal HIGH. */
   //if ( NULL != SIRF_PAL_TrackerOnPort )
   {
      if( level == SIRF_PAL_HW_ON_LOW)
      {
         //returnValue = SIRF_PAL_COM_SetRTS( SIRF_PAL_TrackerOnPort );
          fd = open("/dev/misc/misc_gpio", O_RDWR);
          if(-1 == fd) printf("open gpiodrv error!\n");
          if(fd)
          {
           result=ioctl(fd, PULLDOWN, 0);
           if(result != 0)
           {
               printf("onoff ioctl err \n");	
           }	
           close(fd); 
           printf("ON_OFF %d\n", level);
          }
         
      } 
      else if (level == SIRF_PAL_HW_ON_HIGH) 
      {
         //returnValue = SIRF_PAL_COM_ClrRTS( SIRF_PAL_TrackerOnPort );
          fd = open("/dev/misc/misc_gpio", O_RDWR);
          if(-1 == fd) printf("open gpiodrv error!\n");
          if(fd)
          {
           result=ioctl(fd, PULLUP, 0);
           if(result != 0)
           {
               printf("onoff ioctl err \n");	
           }	
           close(fd); 
           printf("ON_OFF %d\n", level);
          }
         
      } /* else returnValue == SIRF_FAILURE from initial value */
      //printf("Tracker ON-OFF %d\n");

   }
   returnValue = SIRF_SUCCESS;
   return returnValue;
} /* SIRF_PAL_HW_TrackerOnLine() */

/**
 * @}
 * End of file.
 */


