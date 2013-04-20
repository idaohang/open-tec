/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2008 by SiRF Technology, Inc. All rights reserved.*
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology Inc.’s confidential and       *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.’s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your signed written agreement with   *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************
 *
 * FILE: sirf_msg.h
 *
 ***************************************************************************/
#ifndef __SIRF_MSG_H__
#define __SIRF_MSG_H__

#include "sirf_types.h"

/*=============================================================================
 * SiRF Input and Output Logical Channel Definitions
 *---------------------------------------------------------------------------*/

/*#ifdef SIRF_LOC*/
   #define SIRF_LC_UNKNOWN  (0x00)
   #define SIRF_LC_AI3      (0x01)
   #define SIRF_LC_F        (0x02)
   #define SIRF_LC_TRACKER  (0x44)
   #define SIRF_LC_LPLC     (0xB1)
   #define SIRF_LC_STATS    (0xBB)
   #define SIRF_LC_NMEA     (0xDD)
   #define SIRF_LC_SSB      (0xEE)
   #define SIRF_LC_DEBUG    (0xFF)
/*#else
   #define SIRF_LC_SSB      ( 0x00 )
#endif*/


/* Macros ------------------------------------------------------------------- */

#define SIRF_MAKE_MSG_ID( logical_channel, message_id, message_sub_id ) \
    ( (((logical_channel) & 0xFF) << 16)\
    | (( (message_sub_id) & 0xFF) << 8)\
    |  (     (message_id) & 0xFF))

#define SIRF_GET_LC(msgid)     ((tSIRF_UINT8)((msgid) >> 16 & 0xFF))
#define SIRF_GET_SUB_ID(msgid) ((tSIRF_UINT8)((msgid) >> 8  & 0xFF))
#define SIRF_GET_MID(msgid)    ((tSIRF_UINT8)((msgid)       & 0xFF))


#define SIRF_MSG_ERROR_INVALID_MSG_ID      0x0001
#define SIRF_MSG_ERROR_INVALID_MSG_LENGTH  0x0002


#include "sirf_msg_ssb.h"
#ifdef SIRF_LOC
   #include "sirf_msg_f.h"
   #include "sirf_msg_ai3.h"
#endif /* SIRF_LOC */


#endif /* __SIRF_MSG_H__ */

/**
 * @}
 * End of file.
 */

