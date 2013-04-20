/**
 * @addtogroup platform_src_sirf_util_codec
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
 * MODULE:  HOST
 *
 * FILENAME:  sirf_codec_ssb.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_SSB_H__
#define __SIRF_CODEC_SSB_H__

#include "sirf_types.h"

/* Return values ========================================================== */

#define SIRF_CODEC_SSB_NULL_POINTER        0x7000
#define SIRF_CODEC_SSB_INVALID_MSG_ID      0x7001
#define SIRF_CODEC_SSB_LENGTH_ERROR        0x7002


/* =============================================================================
 * Prototype Definitions
 * -------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

tSIRF_RESULT SIRF_CODEC_SSB_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length );

tSIRF_RESULT SIRF_CODEC_SSB_Decode( tSIRF_UINT8* payload,
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 *message_length );

tSIRF_RESULT SIRF_CODEC_SSB_Decode_Ex( tSIRF_UINT8* payload,
                                    tSIRF_UINT32 payload_length,
				    tSIRF_UINT32 sirf_flags,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 *message_length );

#ifdef __cplusplus
}
#endif

#endif /* __SIRF_CODEC_SSB_H__ */

/**
 * @}
 * End of file.
 */

