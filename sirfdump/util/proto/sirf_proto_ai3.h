/**
 * @addtogroup platform_src_sirf_util_proto
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
 * @file   sirf_proto_ai3.h
 */

#ifndef __SIRF_PROTO_AI3_H__
#define __SIRF_PROTO_AI3_H__



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */

#include "sirf_types.h"

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */
/* Some of these are published in the header file for unit testing */
/** Maximum size of a compressed segment */
#define FRAG_SIZE (SIRF_MSG_AI3_MAX_MESSAGE_LEN - RAW_HEADER_OFFSET - AI3_SEG_START - RAW_HEADER_OFFSET)

#define AI3_LC_POS                 (0) /**< Index of the logical channel */
#define AI3_MSG_ID_POS             (1) /**< Index of the msg_id */
#define AI3_NUM_SEG_POS            (2) /**< Index numumber of segments */
#define AI3_SEG_IND_POS            (3) /**< Index of segment index */
#define AI3_SEG_START              (4) /**< Index where the segment starts */

#define RAW_HEADER_OFFSET          (4) /**< a0,a2, 2 bytes of length */


#ifdef MS_ASSIST_COLOC
#  define COLOC_MASK 0x02 /**< BIT 1 of the NW enhance type */
#endif /* MS_ASSIST_COLOC */



/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

typedef tSIRF_RESULT (*tSIRF_send_func)(tSIRF_VOID *,tSIRF_UINT32);
typedef tSIRF_RESULT (*tSIRF_input_func)(tSIRF_UINT32, tSIRF_VOID *, tSIRF_UINT32);

tSIRF_VOID SIRF_PROTO_AI3_Init( tSIRF_send_func send_func,
                                tSIRF_input_func input_func );
tSIRF_VOID SIRF_PROTO_AI3_Close( tSIRF_VOID );

tSIRF_RESULT SIRF_PROTO_AI3_PacketInput(tSIRF_UINT8 *pMsg, tSIRF_UINT32 Len);
tSIRF_RESULT SIRF_PROTO_AI3_Output(tSIRF_UINT32 msg_id, tSIRF_VOID *msg, tSIRF_UINT32 msg_len);
tSIRF_UINT16 SIRF_PROTO_AI3_compressAI3Msg(tSIRF_UINT8 *in_buf, tSIRF_UINT8 *out_buf, tSIRF_UINT16 in_len);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* SIRF_PROTO_AI3_H */

/**
 * @}
 */


