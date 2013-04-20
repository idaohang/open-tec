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
 * FILENAME:  sirf_codec_ascii.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_ASCII_H__
#define __SIRF_CODEC_ASCII_H__

#include "sirf_types.h"

/* Return values ========================================================== */
#define SIRF_CODEC_ASCII_UNKNOWN_MESSAGE_ID     0x6000
#define SIRF_CODEC_ASCII_BAD_FORMAT             0x6001
#define SIRF_CODEC_ASCII_NO_NUMBER              0x6002
#define SIRF_CODEC_ASCII_NO_COMMA               0x6003
#define SIRF_CODEC_ASCII_LENGTH_ERROR           0x6004

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Prototypes ============================================================= */

tSIRF_RESULT SIRF_CODEC_ASCII_Encode( tSIRF_UINT32  message_id,
                                      tSIRF_VOID   *message_structure,
                                      tSIRF_UINT32  message_length,
                                      tSIRF_CHAR   *lpszText,
                                      tSIRF_UINT32 *lpszText_length );

tSIRF_RESULT SIRF_CODEC_ASCII_Decode( tSIRF_CHAR   *linestr,
                                      tSIRF_UINT32 *message_id,
                                      tSIRF_VOID   *message_structure,
                                      tSIRF_UINT32 *message_length );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SIRF_CODEC_ASCII_H__ */

/**
 * @}
 * End of file.
 */

