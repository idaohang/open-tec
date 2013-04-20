/**
 * @addtogroup platform_src_sirf_util_proto
 * @{
 */

 /***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2008 by SiRF Technology, Inc.  All rights reserved.    *
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
 * MODULE:  HOST
 *
 * FILENAME:  sirf_proto_common.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_PROTO_COMMON_H__
#define __SIRF_PROTO_COMMON_H__

#include "sirf_types.h"

/***************************************************************************
   Macro Definitions
***************************************************************************/

#define SIRF_MSG_HEAD0   (0xA0)
#define SIRF_MSG_HEAD1   (0xA2)
#define SIRF_MSG_TAIL0   (0xB0)
#define SIRF_MSG_TAIL1   (0xB3)

/***************************************************************************
   Prototype Definitions
***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* Compute the checksum of a message */
tSIRF_UINT16 SIRF_PROTO_ComputeChksum(tSIRF_UINT8 *pBuf, tSIRF_UINT32 Cnt);

/* Add the header and footer involving the a0,a2,length, ... checksum,b0,b3 */
tSIRF_UINT32 SIRF_PROTO_AddHeaderFooter(tSIRF_UINT8* header,tSIRF_UINT8*footer,tSIRF_UINT32 msg_len);

tSIRF_RESULT SIRF_PROTO_Wrapper( tSIRF_UINT8 *payload, tSIRF_UINT32 payload_length,
                                 tSIRF_UINT8 *data, tSIRF_UINT32 *data_length );
tSIRF_RESULT SIRF_PROTO_Unwrapper( tSIRF_UINT8 *data, tSIRF_UINT32 data_length,
                                   tSIRF_UINT8 *payload, tSIRF_UINT32 *payload_length );

#ifdef __cplusplus
}
#endif

#endif /* __SIRF_PROTO_COMMON_H__ */


/**
 * @}
 * End of file.
 */
