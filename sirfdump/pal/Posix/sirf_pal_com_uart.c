/**
 * @addtogroup platform_src_sirf_pal_posix
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_com_uart.c
 *
 * @brief  SiRF PAL serial communications module.
 */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#include "sirf_types.h"
#include "sirf_pal.h"


const char *g_port_format = "COM%d:" ;

typedef struct
{
   int         fd;
   tSIRF_MUTEX m_WriteSection;
   int         m_iPortNum;
   int         m_iBaudRate;
   tSIRF_BOOL        m_iHwFc; /* hardware flow control */
} M_Comm;

#define SERIAL ((M_Comm*)port_handle)


/* 1 input port, 3 debug ports */
M_Comm  comm_ports[4];
size_t  port_count=0;



/* ----------------------------------------------------------------------------
 *   Local Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Test if the given serial port is open.
 * @param[in] port_handle              Handle to the serial port.
 * @return                             Non-zero if the port is open.
 */
inline static int IsPortOpen( tSIRF_COM_HANDLE port_handle)
{
   /* returns non-zero if port is open */
   return port_handle && SERIAL->fd != -1;

} /* IsPortOpen() */



/* ----------------------------------------------------------------------------
 *   Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Set UART port.
 * @param[in] port_handle              Handle to UART port.
 * @param[in] flow_ctrl                    Hardware flow control.
 * @param[in] baud_rate                New baud rate.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_SetUart( tSIRF_COM_HANDLE port_handle, tSIRF_UINT32 flow_ctrl, tSIRF_UINT32 baud_rate )
{
   /* Unused Parameters */
   (void)(port_handle);
   (void)(flow_ctrl);
   (void)(baud_rate);
   
   /* This function is not needed in current implementation. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_SetUart() */


/**
 * @brief Create a new serial port.
 * @param[out] port_handle             New serial port handle.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Create( tSIRF_COM_HANDLE *port_handle )
{
   if ( port_count >= sizeof(comm_ports)/sizeof(M_Comm) )
      return SIRF_PAL_COM_ERROR;

   *port_handle = &(comm_ports[port_count]);

   memset( *port_handle, 0, sizeof(M_Comm) );

   if ( SIRF_PAL_OS_MUTEX_Create( &(comm_ports[port_count].m_WriteSection) ) != SIRF_SUCCESS )
      return SIRF_PAL_COM_ERROR;

   comm_ports[port_count].fd = -1;

   port_count++;

   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_Create() */


/**
 * @brief Delete a serial port handle.
 * @param[in] port_handle              Serial port handle to delete.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Delete( tSIRF_COM_HANDLE port_handle )
{
   /* note: ports should be deleted in the same order they were ceated */
   port_count--; 

   SIRF_PAL_OS_MUTEX_Delete( SERIAL->m_WriteSection );

   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_Delete() */


/**
 * @brief Open a serial port.
 * @param[in] port_handle              Serial port handle to open.
 * @param[in] port_num                 Serial port device number.
 * @param[in] flow_ctrl                    hardware flow control.
 * @param[in] baud_rate                Serial baud rate.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Open( tSIRF_COM_HANDLE port_handle, tSIRF_UINT32 port_num,
   tSIRF_UINT32 flow_ctrl, tSIRF_UINT32 baud_rate )
{
   struct   termios options;
   char     port_str[30]="";
   int      brate;
   int      fd;

#ifdef OS_QNX
   sprintf( port_str, "/dev/ser%ld", port_num );
#else
   sprintf(port_str, "/dev/ttyAMA%ld", port_num); 
   printf("no OS_QNX\n");
   printf("open:%s \n",port_str);
 #endif 
   
   SERIAL->m_iPortNum = port_num;
   SERIAL->m_iBaudRate = baud_rate;

   /* open the port: */
   fd = open( port_str, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
   if ( fd == -1 )
   {
      printf("open ttyAMAx error: \n");
      DEBUGMSG(1, (DEBUGTXT("error: could not open port; errno=%d\n"), errno));
      return SIRF_PAL_COM_ERROR;
   }
   printf("open ttyAMAx ok \n");
  
   #ifdef OS_LINUX
      fcntl( fd, F_SETFL, FNDELAY );
   #endif

   /* Get the current options for the port: */
   if ( tcgetattr(fd, &options) != 0 )
   {
      DEBUGMSG(1, (DEBUGTXT("error: could not get port attributes; errno=%d\n"), errno));
      return SIRF_PAL_COM_ERROR;
   }

   bzero (&options, sizeof (options)); 
   
   switch (baud_rate)
   {
   case 115200: brate = B115200; break;
   case  57600: brate = B57600;  break;
   case  38400: brate = B38400;  break;
   case  19200: brate = B19200;  break;
   case   9600: brate = B9600;   break;
   case   4800: brate = B4800;   break;
   }

   options.c_cflag = CLOCAL | CREAD | CS8;    
   options.c_iflag = IGNPAR;
   options.c_oflag = 0;
   options.c_lflag = 0;
   
   if (flow_ctrl)
   {
      /* Depending on the Posix/Unix/Linux flavor you may need one of these */
      /* options.c_cflag |= CNEW_RTSCTS; */
      /* Cygwin supports this one */
      options.c_cflag |= CRTSCTS;
   }

   cfsetispeed(&options, brate);
   cfsetospeed(&options, brate);

   options.c_cc[VTIME] = 0;   /* inter-character timer unused */
   options.c_cc[VMIN] = 1; /* blocking read until 1 char received */ 

   /* Set the new options for the port: */
   if ( tcsetattr(fd, TCSANOW, &options) != 0 )
   {
      DEBUGMSG(1, (DEBUGTXT("error: could not set port attributes; errno=%d\n"), errno));
      return SIRF_PAL_COM_ERROR;
   }  

   tcflush(fd, TCIOFLUSH);

   SERIAL->fd = fd;

   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_Open() */


/**
 * @brief Wait for serial port data.
 * @param[in] port_handle              Serial port handle to open.
 * @param[in] timeout                  Timeout to wait for data.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Wait( tSIRF_COM_HANDLE port_handle, tSIRF_UINT32 timeout )
{
   fd_set         sel_set;
   struct timeval sel_timeout;
   int            sel_result=0;

   if ( !IsPortOpen(port_handle) )
   {
      DEBUGMSG(1, (DEBUGTXT("SIRF_PAL_COM_WaitInput: port not open\n")));
      SIRF_PAL_OS_THREAD_Sleep(500);
      return SIRF_PAL_COM_ERROR;
   }

   FD_ZERO(&sel_set);
   FD_SET(SERIAL->fd, &sel_set);

   sel_timeout.tv_sec  = timeout / 1000;
   sel_timeout.tv_usec = (timeout % 1000)*1000;

   sel_result = select(SERIAL->fd+1, &sel_set, NULL, NULL, &sel_timeout);

   if (sel_result < 0)
   {
      DEBUGMSG(1,(DEBUGTXT("SIRF_PAL_COM_Wait: error in select; errno=%d\n"), errno));
      SIRF_PAL_OS_THREAD_Sleep( 100 );
      return SIRF_PAL_COM_ERROR;
   }

   else if (sel_result == 0)
   {
      return SIRF_PAL_COM_TIMEOUT;
   }

   /* else - we have input: */
   DEBUGCHK(sel_result==1);

   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_Wait() */


/**
 * @brief Read data from the serial port.
 * @param[in] port_handle              Serial port handle to read from.
 * @param[out] pbyBuffer               Buffer to read into.
 * @param[in] ulbBufSize               Size of the buffer.
 * @param[out] pulBytesRead            Number of bytes read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Read( tSIRF_COM_HANDLE port_handle, tSIRF_UINT8 *pbyBuffer, tSIRF_UINT32 ulBufSize, tSIRF_UINT32 * pulBytesRead )
{
   int result=0;
   int i=0;
   struct timeval timems;

   if ( !IsPortOpen(port_handle) )
   {
      *pulBytesRead = 0;
      return SIRF_PAL_COM_ERROR;
   }

   do {
      result = read( SERIAL->fd, pbyBuffer, ulBufSize );
   } while ( result==-1 && errno==EINTR);

   *pulBytesRead = result>0 ? result : 0;
   gettimeofday(&timems,NULL);
   if(result)
	//printf("%d %d\n", timems.tv_sec*1000+timems.tv_usec/1000, result);
/* (should have waited for at least one byte before read() ) */
   return result>0 ? SIRF_SUCCESS : SIRF_PAL_COM_ERROR; 

} /* SIRF_PAL_COM_Read() */


/**
 * @brief Write data to the serial port.
 * @param[in] port_handle              Serial port handle to write into.
 * @param[in] pbyData                  Data to write.
 * @param[in] ulLength                 Amount of data to write.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Write( tSIRF_COM_HANDLE port_handle, tSIRF_UINT8 * pbyData, tSIRF_UINT32 ulLength )
{
   int    ok = 1;
   size_t ulTotalWritten = 0u;
   int    err = 0;
   fd_set sel_set;
   int    sel_result=0;    

   if ( !IsPortOpen(port_handle) )
      return SIRF_PAL_COM_ERROR;

   if ( ulLength==0 )
      return SIRF_SUCCESS;

   SIRF_PAL_OS_MUTEX_Enter( SERIAL->m_WriteSection );

   do
   {
      long lBytesWritten;
      
      FD_ZERO(&sel_set);
      FD_SET(SERIAL->fd, &sel_set);
      sel_result = select(SERIAL->fd+1, NULL, &sel_set, NULL, NULL);
      lBytesWritten = write(SERIAL->fd, pbyData+ulTotalWritten, ulLength-ulTotalWritten);
      
      if ( lBytesWritten > 0 )
      {
         fsync(SERIAL->fd);
         ulTotalWritten += lBytesWritten; /* written at least a byte */
         //printf("lBytesWritten %d\n", lBytesWritten);
      }
      else
      {
         /* break loop unless signals or full buffers or no error */
         if ( !(lBytesWritten==0 || errno==EINTR || errno==EAGAIN) )
         { 
            ok = 0;
            err = errno;
         }
         fsync(SERIAL->fd);
         /** @todo   SIRF_PAL_OS_THREAD_Sleep( 10 ); retry after a delay */
      }
   }
   while ( ulTotalWritten < ulLength && ok && IsPortOpen(port_handle) );

   SIRF_PAL_OS_MUTEX_Exit( SERIAL->m_WriteSection );

   if (!ok)
   {
      DEBUGMSG(2, (DEBUGTXT("SIRF_PAL_COM_Write: error sending on SERIAL port, errno=%d\n"), err));
   }

   return ok ? SIRF_SUCCESS : SIRF_PAL_COM_ERROR;

} /* SIRF_PAL_COM_Write() */


/**
 * @brief Close an open serial port.
 * @param[in] port_handle              Serial port handle to close.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Close( tSIRF_COM_HANDLE port_handle )
{
   int fd;
   int result = 0;

   if ( !IsPortOpen(port_handle) )
      return SIRF_PAL_COM_ERROR;

   fd = SERIAL->fd;

   SERIAL->fd = -1;
   do 
   {
      result = close(fd);
   } 
   while (result==-1 && errno==EINTR);

   return result==0 ? SIRF_SUCCESS : SIRF_PAL_COM_ERROR;

} /* SIRF_PAL_COM_Close() */


/**
 * @brief Re-open a closed serial port.
 * @param[in] port_handle              Serial port handle to re-open.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_Reopen( tSIRF_COM_HANDLE port_handle )
{
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_Reopen() */



/* ============================================================================
 * Functions below are used to reset a GSD3t tracker only.
 * They are not used for any communication handshake. */



/**
 * @brief Clear serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_ClrRTS( tSIRF_COM_HANDLE port_handle )
{
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_ClrRTS() */


/**
 * @brief Set serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_SetRTS( tSIRF_COM_HANDLE port_handle )
{
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_SetRTS() */


/**
 * @brief Clear serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_ClrDTR( tSIRF_COM_HANDLE port_handle )
{
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_ClrDTR() */


/**
 * @brief Set serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_SetDTR( tSIRF_COM_HANDLE port_handle )
{
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_COM_SetDTR() */


/**
 * @brief Clear the serial port CTS line.
 *
 * This function clears the serial port CTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrCTS(tSIRF_COM_HANDLE port_handle){
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_FAILURE;
}

/**
 * @brief Set the serial port CTS line.
 *
 * This function sets the serial port CTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetCTS(tSIRF_COM_HANDLE port_handle){
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_FAILURE;
}

/**
 * @brief Clear the SPI port SS line.
 *
 * This function clears the serial port SS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrSS(tSIRF_COM_HANDLE port_handle){
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_FAILURE;
}
/**
 * @brief Set the SPI port SS line.
 *
 * This function sets the serial port SS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetSS(tSIRF_COM_HANDLE port_handle){
   /* Unused Parameter. */
   (void)port_handle;
   
   /* This function is not needed in current implementation */
   return SIRF_FAILURE;
}

/**
 * @}
 */


