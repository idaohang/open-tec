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
 * @file   sirf_pal_tcpip.c
 *
 * @brief  SiRF PAL networking module.
 */

#include "sirf_pal.h"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/socket.h>



/* ----------------------------------------------------------------------------
 *   Local Functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Test if a socket handle is valid.
 * @param[in] socket                   Socket handle to test.
 * @return                             Non-zero if valid.
 */
static int IsValidSocket(int socket)
{
   return socket>=0;

} /* IsValidSocket() */



/* ----------------------------------------------------------------------------
 *   Functions
 * ------------------------------------------------------------------------- */

/** 
 * Global initialization of the Network Module
 * 
 * @return SIRF_SUCCESS if successful.  Refer to error codes for failure
 */
tSIRF_RESULT SIRF_PAL_NET_Init(tSIRF_VOID)
{
   /* Nothing to be done for non-secure Posix sockets */
   return SIRF_SUCCESS;
}

/** 
 * Global shutdown of the network module *
 * 
 * @return SIRF_SUCCESS if successful.  Refer to error codes for failure
 */
tSIRF_RESULT SIRF_PAL_NET_Destroy(tSIRF_VOID)
{
   /* Nothing to be done for non-secure Posix sockets */
   return SIRF_SUCCESS;
}

/**
 * @brief Create a socket of the specified type.
 * @param socket_handle[out]           Handle to the socket.
 * @param socket_type[in]              Type of socket to create.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_CreateSocket( tSIRF_SOCKET *socket_handle, 
                                        tSIRF_UINT32 socket_type )
{
   char           opt_c;
   int            result = 0;

   DEBUGCHK(socket_handle);
   *socket_handle = (tSIRF_SOCKET)socket(AF_INET, SOCK_STREAM, 0);

   if (!IsValidSocket(*socket_handle))
   {
      DEBUGMSG(1,(DEBUGTXT("socket() failed, errno=%d\n"), errno));
      *socket_handle = SIRF_PAL_NET_INVALID_SOCKET;
      return SIRF_PAL_NET_ERROR;
   }

   /* Set all socket options here: */
   switch (socket_type)
   {
   case SIRF_PAL_NET_SOCKET_TYPE_BLOCKING:
      /* Sockets are blocking by default */
      break;
   case SIRF_PAL_NET_SOCKET_TYPE_DEFAULT:
   {
      do 
      {
         result = fcntl( *socket_handle, F_SETFL, O_NONBLOCK );
      } while (result!=0 && errno==EINTR);

      if (0<result) 
      {
         DEBUGMSG(1,(DEBUGTXT("fcntl() failed, errno=%d\n"), errno));
      }
      break;
   }
   default:
      return SIRF_PAL_NET_ERROR;
   }

   opt_c = 1;
   result = setsockopt(*socket_handle, 
                       SOL_SOCKET, 
                       SO_REUSEADDR, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      SIRF_PAL_NET_CloseSocket(*socket_handle);
      return SIRF_PAL_NET_ERROR;
   }

   opt_c = 1;
   result = setsockopt(*socket_handle, 
                       SOL_SOCKET, 
                       SO_DONTLINGER, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      SIRF_PAL_NET_CloseSocket(*socket_handle);
      return SIRF_PAL_NET_ERROR;
   }

   opt_c = 1;
   result = setsockopt(*socket_handle, 
                       IPPROTO_TCP, 
                       TCP_NODELAY, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      SIRF_PAL_NET_CloseSocket(*socket_handle);
      return SIRF_PAL_NET_ERROR;
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_NET_CreateSocket() */


/**
 * @brief Close a socket.
 * @param[in] socket_handle            Handle to the socket to close.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_CloseSocket(tSIRF_SOCKET socket_handle)
{
   int result;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);

   do
   {
      shutdown(socket_handle, 2);

      result = close(socket_handle);

   } while (0 > result && EINTR == errno);

   return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_CloseSocket() */


/**
 * @brief Select on socket(s) to wait for activity.
 * @param read[in]            Null-terminated array of sockets for reading.
 * @param write[in]           Null-terminated array of sockets for writing. 
 * @param excpt[in]           Null-terminated array of sockets for excpting.
 * @param timeout[in]         Timeout in milliseconds.
 *
 * The arrays are modified on exit to contain only those sockets that were 
 * triggered.
 *
 * @return                             Success code.
 */
tSIRF_RESULT SIRF_PAL_NET_Select( tSIRF_SOCKET *read, 
                                  tSIRF_SOCKET *write, 
                                  tSIRF_SOCKET *excpt, 
                                  tSIRF_UINT32 timeout)
{
   struct timeval tv;
   fd_set r, w, e;
   int result, i;
   tSIRF_SOCKET fdmax;

   fdmax = 0;

   FD_ZERO(&r);
   FD_ZERO(&w);
   FD_ZERO(&e);

   if (read)
   {
      for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
      {
         FD_SET(read[i], &r);
         if (read[i]>fdmax) fdmax=read[i];
      }
   }

   if (write)
   {
      for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
      {
         FD_SET(write[i], &w);
         if (write[i]>fdmax) fdmax=write[i];
      }
   }

   if (excpt)
   {
      for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
      {
         FD_SET(excpt[i], &e);
         if (excpt[i]>fdmax) fdmax=excpt[i];
      }
   }

   tv.tv_sec = timeout / 1000;
   tv.tv_usec = (timeout % 1000) * 1000;


   do
   {
      result = select(fdmax+1, &r, &w, &e, &tv);
   }
   while ( 0 > result && EINTR == errno );

   if (0<result)
   {
      if (read)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
         {
            if (!FD_ISSET(read[i], &r))
            {
               read[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }
      if (write)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
         {
            if (!FD_ISSET(write[i], &w))
            {
               write[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }
      if (excpt)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
         {
            if (!FD_ISSET(excpt[i], &e))
            {
               excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }
   }

   if (0 < result)
   {
      return SIRF_SUCCESS;
   }
   else if (0 == result)
   {
      return SIRF_PAL_NET_TIMEOUT;
   }
   else
   {
      return SIRF_PAL_NET_ERROR;
   }
   
} /* SIRF_PAL_NET_Select() */


/**
 * @brief Read data from a socket.
 * @param socket_handle[in]            Handle to the socket to read from.
 * @param buffer[out]                  Buffer to read data into.
 * @param size[in]                     Size of the data buffer.
 * @param bytes_read[out]              Number of bytes read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Read( tSIRF_SOCKET socket_handle, 
                                tSIRF_UINT8 *buffer, 
                                tSIRF_UINT32 size, 
                                tSIRF_UINT32 *bytes_read )
{
   int result = 0;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);
   DEBUGCHK(bytes_read);

   *bytes_read = 0;

   do {
      result = recv(socket_handle, (char*)(buffer+*bytes_read), size-*bytes_read, 0);
   } while ( 0 > result && EINTR == errno );

   if ( 0 > result )
   {
      DEBUGMSG(1,(DEBUGTXT("recv() failed, errno=%d, size=%d, *bytes_read=%d\n"), errno, size, *bytes_read));
      SIRF_PAL_OS_THREAD_Sleep(100);
      return SIRF_PAL_NET_ERROR;
   }

   *bytes_read = result;

   return 0 < result ? SIRF_SUCCESS : SIRF_PAL_NET_CONNECTION_CLOSED;

} /* SIRF_PAL_NET_Read() */


/**
 * @brief Write data to a socket.
 * @param[in] socket_handle            Handle to the socket to write into.
 * @param[in] packet                   Data to write.
 * @param[in] length                   Length of the data to write.
 * @param[out] bytes_written           Number of bytes written.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Write( tSIRF_SOCKET socket_handle, 
                                 tSIRF_UINT8 *packet, 
                                 tSIRF_UINT32 length, 
                                 tSIRF_UINT32 *bytes_written )
{
   int  result;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);
   DEBUGCHK(bytes_written);

   *bytes_written=0;

   do 
   {
      result = send( socket_handle, (char*)(packet+*bytes_written), length-*bytes_written, 0);
   } while ( result<0 && errno==EINTR );

   if ( 0 >= result )
   {
      DEBUGMSG(1,(DEBUGTXT("send() failed, errno=%d\n"), errno));
      SIRF_PAL_OS_THREAD_Sleep(100);
      return SIRF_PAL_NET_ERROR;
   }

   *bytes_written = result;

   return SIRF_SUCCESS;

} /* SIRF_PAL_NET_Write() */


/**
 * @brief Set the socket to run in listening mode.
 * @param[in] socket_handle            Handle to the socket.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Listen(tSIRF_SOCKET socket_handle)
{
   int result;
   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);

   do
   {
      result = listen(socket_handle, SOMAXCONN);
   } while (result!=0 && errno==EINTR);

   return 0==result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Listen() */


/**
 * @brief Bind a socket to the specified address.
 * @param[in] socket_handle            Handle to the socket.
 * @param[in] name                     Socket address to bind to.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Bind( tSIRF_SOCKET socket_handle, 
                                tSIRF_SOCKADDR *name )
{
   int result;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle&&name);

   name->reserved_0 = AF_INET;
   result = bind(socket_handle, (struct sockaddr*)name, sizeof(tSIRF_SOCKADDR));
   if (0 != result)
   {
      DEBUGMSG(1,(DEBUGTXT("bind() failed, errno=%d\n"), errno));
   }

   return 0==result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Bind() */


/**
 * @brief Accept an incoming connection on a listening socket.
 * @param[in] listener_handle          Listening socket.
 * @param[out] socket_handle           Received socket connection.
 * @param[in] name                     Address of received socket.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Accept( tSIRF_SOCKET listener_handle, 
                                  tSIRF_SOCKET *socket_handle, 
                                  tSIRF_SOCKADDR *name,
                                  tSIRF_UINT32 security)
{
   int result;
   int namelen = sizeof(tSIRF_SOCKADDR);

   DEBUGCHK( socket_handle );
   *socket_handle = SIRF_PAL_NET_INVALID_SOCKET;

   if (SIRF_PAL_NET_SECURITY_NONE != security)
   {
      return SIRF_PAL_NET_SECURITY_NOT_SUPPORTED;
   }

   do
   {
      result = accept(listener_handle, (struct sockaddr*)name, (name ? &namelen : NULL) );
   } while ( 0 > result && EINTR == errno );

   if (IsValidSocket(result))
   {
      *socket_handle = result;
   }

   return 0 < result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Accept() */


/**
 * @brief Connect to the specified address/port.
 * @param[in] socket_handle            Socket handle.
 * @param[in] addr                     Address to connect to.
 * @param[in] port                     Port to connect to.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Connect( tSIRF_SOCKET socket_handle, 
                                   tSIRF_CHAR *addr, 
                                   tSIRF_UINT16 port, 
                                   tSIRF_UINT32 security )
{
   unsigned long **     p_addr;
   int                  result = 0;
   tSIRF_SOCKADDR       sockaddr;
   struct hostent *     host_addrs;

   DEBUGCHK((SIRF_PAL_NET_INVALID_SOCKET != socket_handle) && addr);

   if (SIRF_PAL_NET_SECURITY_NONE != security)
   {
      return SIRF_PAL_NET_SECURITY_NOT_SUPPORTED;
   }

   /* convert from dotted notation to sockaddr: */
   { 
      /* Use TCP if DNS lookup needed */
      sethostent(1);

      do
      {
         host_addrs = gethostbyname( addr );
      }
      while (NULL == host_addrs && TRY_AGAIN == h_errno);

      endhostent();

      if (NULL == host_addrs) return SIRF_PAL_NET_ERROR;
   }

   p_addr = (unsigned long **)host_addrs->h_addr_list;
   do
   {
      sockaddr.reserved_0 = AF_INET;
      sockaddr.sin_addr   = **p_addr++;
      sockaddr.sin_port   = htons(port);

      do
      {
         result = connect(socket_handle, (struct sockaddr*)&sockaddr, sizeof(tSIRF_SOCKADDR));

         if (0 != result)
         {
            if (EINPROGRESS==errno || EWOULDBLOCK==errno)
            {
               return SIRF_SUCCESS;/* connection request sent, waiting for reply */
            }

            if ( EINTR == errno )
            {
               continue;
            }
            else
            {
               DEBUGMSG(1,(DEBUGTXT("connect() to IP %s failed, errno=%d trying next\n"),
                  inet_ntoa(sockaddr), errno));
               break;
            }
         }
      }
      while ( 0 != result );
   }
   while ((0 != *p_addr) && (0 != result)); 

   if (0 != result)
   {
      DEBUGMSG(1,(DEBUGTXT("connect() failed, errno\n")));
      return SIRF_PAL_NET_ERROR;
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_NET_Connect() */


/**
 * @brief Get the socket peer name.
 * @param socket_handle[in]            Socket handle.
 * @param name[in]                     Name of the peer.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_GetPeerName( tSIRF_SOCKET socket_handle, 
                                       tSIRF_SOCKADDR *name )
{
   int result, len;
   tSIRF_SOCKADDR tempname;
   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);

   len = sizeof(tSIRF_SOCKADDR);
   result = getpeername( socket_handle, 
                         (struct sockaddr*)(name?name:&tempname), 
                         &len );

   return 0==result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_GetPeerName() */

/**
 * @}
 * End of file.
 */


