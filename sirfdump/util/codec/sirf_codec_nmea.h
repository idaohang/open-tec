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
 * FILENAME:  sirf_codec_nmea.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_NMEA_H__
#define __SIRF_CODEC_NMEA_H__

#include "sirf_types.h"
#include "sirf_msg.h"

/***************************************************************************
   Macro Definitions
***************************************************************************/

#define SIRF_MSG_SSB_DOP_LSB ( 0.2F )
#define   CR         (0x0D)       /* End of Sentence indicator */
#define   LF         (0x0A)       /* End of sentence indicator */

/* NMEA Message IDs. These are translated to GPS Message IDs defined in gps_messages.h */

#define NMEA_EE_REQUEST_EPHEMERIS        ( 151 )
#define NMEA_EE_INTEGRITY_WARNING        ( 152 )
#define NMEA_EE_MSG_ACK                  ( 154 )
#define NMEA_EE_PROVIDE_EPHEMERIS        ( 107 )
#define NMEA_EE_PROVIDE_IONO_PARAMS      ( 108 )
#define NMEA_EE_DEBUG_MSG                ( 110 )
#define NMEA_EE_CLK_BIAS_ADJUSTMENT      ( 155 )
#define NMEA_EE_SET_MSG_RATE             ( 130 )      
#define NMEA_EE_SUBFRAME_NAV_BITS_OUTPUT ( 140 )

/***************************************************************************
   Prototype Definitions
***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GGA(tSIRF_MSG_SSB_GEODETIC_NAVIGATION * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_RMC(tSIRF_MSG_SSB_GEODETIC_NAVIGATION * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GLL(tSIRF_MSG_SSB_GEODETIC_NAVIGATION * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSA(tSIRF_MSG_SSB_GEODETIC_NAVIGATION * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_VTG(tSIRF_MSG_SSB_GEODETIC_NAVIGATION * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSV(tSIRF_MSG_SSB_MEASURED_TRACKER   * data, tSIRF_CHAR * buf);
tSIRF_RESULT SIRF_CODEC_NMEA_Export(tSIRF_UINT32 message_id, void *message_structure, tSIRF_UINT32 message_length,
                                    tSIRF_UINT8* packet, tSIRF_UINT32 *packet_length, tSIRF_UINT8 numSV_nmea);
tSIRF_RESULT SIRF_CODEC_NMEA_Import( tSIRF_UINT8* payload, tSIRF_INT32 payload_length, tSIRF_UINT32 *message_id, 
                                    void *message_structure, tSIRF_UINT32 *message_length);

#ifdef __cplusplus
} /* extern "C" { */
#endif
#endif /*__SIRF_CODEC_NMEA_H__ */

/**
 * @}
 * End of file.
 */

