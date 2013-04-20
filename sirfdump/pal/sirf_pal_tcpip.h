/**
 * @addtogroup platform_src_sirf_pal 
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
 * @file   sirf_pal_tcpip.h
 *
 * @brief  SiRF PAL networking API.
 *
 * This follows fairly closely the berkely socket model.
 */

#ifndef SIRF_PAL_NET_H_INCLUDED
#define SIRF_PAL_NET_H_INCLUDED

#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *   Preprocessor Definitions
 * ------------------------------------------------------------------------- */



/** Default socket type. */
#define SIRF_PAL_NET_SOCKET_TYPE_DEFAULT     (0)
#define SIRF_PAL_NET_SOCKET_TYPE_BLOCKING    (1)

/** SiRF PAL network error codes. */
#define SIRF_PAL_NET_ERROR                       (0x2601)
#define SIRF_PAL_NET_TIMEOUT                     (0x2602) /* notification */
#define SIRF_PAL_NET_CONNECTION_CLOSED           (0x2603) /* notification */
#define SIRF_PAL_NET_MAX_SOCKETS_ALLOCATED       (0x2604)
#define SIRF_PAL_NET_SECURITY_NOT_SUPPORTED      (0x2605)
#define SIRF_PAL_NET_SECURITY_VERSION_ERROR      (0x2606)
#define SIRF_PAL_NET_SECURITY_PEER_REFUSED       (0x2607)
#define SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR  (0x2608)
#define SIRF_PAL_NET_SECURITY_ERROR              (0x2609)

#define SIRF_PAL_NET_SECURITY_NONE           (0)
/* Type of security is platform dependent */
#define SIRF_PAL_NET_SECURITY_USE_ALWAYS     (1)

#define SIRF_PAL_NET_INVALID_SOCKET          ((tSIRF_SOCKET) (-1))

/* ----------------------------------------------------------------------------
 *   Types
 * ------------------------------------------------------------------------- */



/** Socket handle type. */
#define tSIRF_SOCKET tSIRF_UINT32

/**
 * @brief Socket address structure.
 */
typedef struct
{
   tSIRF_INT16   reserved_0;
   tSIRF_UINT16  sin_port;
   tSIRF_UINT32  sin_addr;
   tSIRF_CHAR    reserved_1[8];
} tSIRF_SOCKADDR;



/* ----------------------------------------------------------------------------
 *   Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif

/* This function should be called by SIRF_PAL_Init */
tSIRF_RESULT SIRF_PAL_NET_Init(tSIRF_VOID);
/* This function should be called by SIRF_PAL_Destroy */
tSIRF_RESULT SIRF_PAL_NET_Destroy(tSIRF_VOID);

/*----------------------------------------------------------------------
 * Create Close
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_CreateSocket( tSIRF_SOCKET *socket_handle, tSIRF_UINT32 socket_type );
tSIRF_RESULT SIRF_PAL_NET_CloseSocket(  tSIRF_SOCKET  socket_handle );


/*----------------------------------------------------------------------
 * Client function
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Connect( tSIRF_SOCKET socket_handle, 
                                   tSIRF_CHAR *addr, 
                                   tSIRF_UINT16 port, 
                                   tSIRF_UINT32 security);

/*----------------------------------------------------------------------
 * Server functions bind, listen, accept
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Bind(    tSIRF_SOCKET socket_handle, tSIRF_SOCKADDR *name );
tSIRF_RESULT SIRF_PAL_NET_Listen(  tSIRF_SOCKET socket_handle );
tSIRF_RESULT SIRF_PAL_NET_Accept(  tSIRF_SOCKET listener_handle, 
                                   tSIRF_SOCKET *socket_handle, 
                                   tSIRF_SOCKADDR *name, 
                                   tSIRF_UINT32 security );

/*----------------------------------------------------------------------
 * Common functions select, send->Write recv->Read
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Select( tSIRF_SOCKET *read, tSIRF_SOCKET *write, tSIRF_SOCKET *excpt, tSIRF_UINT32 timeout );

tSIRF_RESULT SIRF_PAL_NET_Read( tSIRF_SOCKET socket_handle, tSIRF_UINT8 *buffer, tSIRF_UINT32 size, tSIRF_UINT32 *bytes_read );
tSIRF_RESULT SIRF_PAL_NET_Write( tSIRF_SOCKET socket_handle, tSIRF_UINT8 *packet, tSIRF_UINT32 length, tSIRF_UINT32 *bytes_written );

tSIRF_RESULT SIRF_PAL_NET_GetPeerName( tSIRF_SOCKET socket_handle, tSIRF_SOCKADDR *name );

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif



#endif /* !SIRF_PAL_NET_H_INCLUDED */

/**
 * @}
 * End of file.
 */


