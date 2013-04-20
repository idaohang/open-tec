/**
 * @addtogroup platform_src_sirf_pal_posix
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
 *    This Software contains SiRF Technology Inc.�s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.�s  express written
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

#include <stdio.h>

#include "sirf_types.h"
#include "sirf_pal.h"
#include <unistd.h>
#include <fcntl.h>
#define PULLDOWN  0
#define PULLUP    1
//#define O_RDWR		     0x02
/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */


tSIRF_RESULT SIRF_PAL_HW_SetTrackerResetPort( tSIRF_HANDLE port )
{
   // This function is not needed.

   return SIRF_SUCCESS;

} // SIRF_PAL_HW_SetTrackerResetPort()

/* a global handler for the tracker reset port to control SRESET_N pin */
tSIRF_COM_HANDLE SIRF_PAL_TrackerResetPort = NULL;

/**
 * @brief Reset the tracker.
 * @param[in] level              Non-zero to put into reset.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_HW_TrackerReset( tSIRF_UINT32 level )
{
    int fd=-1;
    int result=-1;
    tSIRF_RESULT retVal = SIRF_SUCCESS;
   //char buf[1024];
   //if (NULL != SIRF_PAL_TrackerResetPort)
   {
      /* The GSD3T Tracker inverts these signals so reset HIGH is 0 volts
       * and reset LO is 3 volts.  Thus we should ClrDTR to bring the 
       * signal HIGH. */
      if ( SIRF_PAL_HW_RESET_HIGH == level ) 
      {
          fd = open("/dev/misc/misc_gpio", O_RDWR);
          if(-1 == fd) printf("open gpiodrv error!\n");
          //int fd=open("/proc/gpio/gpio_group_0",O_RDWR);
                    
#if 1       
          if(fd)
          {
              //write(fd, "0x1 0xcc4ff77", 13);
              //write(fd, "0x4 0x60",8);
              //memset(buf, 0, 1000);
              //read(fd, buf, 1000);
              //printf(buf);
               
              result=ioctl(fd, PULLUP, 0);
              if(result != 0)
              {
                  printf("reset ioctl err \n");	
              }	
              close(fd); 
              printf("reset %d\n", level);

              //pStart = strstr(buf, "");
              
          } 
#endif  
        //system("echo 0x2 0xcc4ff77>gpio_group_0");
        //system("echo 0x4 0x60 >gpio_group_0");
      }
      else if (SIRF_PAL_HW_RESET_LOW == level) 
      {
#if 1         
          fd = open("/dev/misc/misc_gpio", O_RDWR);
          if(-1 == fd) printf("open gpiodrv error!\n");
          
          if(fd)
          {
          //write(fd, "0x1 0xcc4ff77", 13);
          //write(fd, "0x4 0x20",8);
          //memset(buf, 0, 1000);
          //read(fd, buf, 1000);
          //printf(buf); 
           result=ioctl(fd, PULLDOWN, 0); 
           if(result != 0)
           {
               printf("reset ioctl err \n");	
           }	
           close(fd); 
           printf("reset %d\n", level);

          } 
#endif       
      } /* else retVal = SIRF_FAILURE as set at initialization */ 
     
   }
   return retVal;
} /* SIRF_PAL_HW_TrackerReset() */

/**
 * @}
 * End of file.
 */


