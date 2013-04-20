/**
 * @addtogroup platform_src_sirf_util_proto
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *  Copyright (c) 2007-2008 by SiRF Technology, Inc. All rights reserved.  *
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
 * FILENAME:  sirf_proto_parse.h
 *
 * DESCRIPTION: This file include the data structures and the functions to
 *              implement the registration of protocols with UI mdoule
 *
 ***************************************************************************/

#ifndef __SIRF_PROTO_PARSE_H__
#define __SIRF_PROTO_PARSE_H__

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include "sirf_types.h"

typedef enum {
   PARSER_SSB,
   PARSER_NMEA,
   PARSER_SIRFLOC
} tSIRF_ParserType;

/* Define a callback function that will handle the message processing */
typedef tSIRF_RESULT (*t_callback_func)(tSIRF_UINT8 *, tSIRF_UINT32, tSIRF_ParserType);

tSIRF_VOID SIRF_PROTO_Parse_Register( t_callback_func callback_func );

tSIRF_VOID SIRF_PROTO_SLParse( tSIRF_UINT8 *Buf, tSIRF_UINT32 BytesRead );
tSIRF_VOID SIRF_PROTO_Parse( tSIRF_UINT8 *Buf, tSIRF_UINT32 BytesRead );
tSIRF_VOID SIRF_PROTO_NMEAParse( tSIRF_UINT8 *Buf, tSIRF_UINT32 BytesRead );

#endif /* __SIRF_PROTO_PARSE_H__ */


/**
 * @}
 * End of file.
 */

