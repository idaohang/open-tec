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
 * FILENAME:  sirf_proto_common.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the SiRF System/F protocol from and to the Tracker API 
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <stdio.h>
#include "sirf_types.h"

#include "sirf_codec.h"
#include "sirf_proto_common.h"

/***************************************************************************
 * Description:   Calculate a "sum and drop carry" checksum
 * Parameters:   buffer pointer and byte count
 * Returns:      returns a 15-bit checksum
 ***************************************************************************/

tSIRF_UINT16 SIRF_PROTO_ComputeChksum(tSIRF_UINT8 * pBuf, tSIRF_UINT32 Cnt)
{
   tSIRF_UINT16 Chksum = 0;
   if (NULL != pBuf)
   {
      for(;Cnt > 0; Cnt--)
      {
         /* note: use a=(cast)a+b instead of a+=b to avoid integer overflow */
         Chksum = (tSIRF_UINT16) (Chksum + *pBuf++);
      }
   }
   return Chksum & 0x7FFF;
}


/***************************************************************************
 * Description:   
 * Parameters:   
 * Returns:      
 ***************************************************************************/

tSIRF_UINT32 SIRF_PROTO_AddHeaderFooter(tSIRF_UINT8 *header,tSIRF_UINT8 *footer,tSIRF_UINT32 msg_len)
{
   tSIRF_UINT16 Chksum;
   header[0] = SIRF_MSG_HEAD0;
   header[1] = SIRF_MSG_HEAD1;
   header[2] = (tSIRF_UINT8) (msg_len >> 0x08);
   header[3] = (tSIRF_UINT8) (msg_len &  0xFF);

   /* checksum and trailer */
   Chksum = SIRF_PROTO_ComputeChksum(header+4, msg_len);
   SIRFBINARY_EXPORT16(Chksum,         footer);
   SIRFBINARY_EXPORT8(SIRF_MSG_TAIL0,  footer);
   SIRFBINARY_EXPORT8(SIRF_MSG_TAIL1,  footer);
   
   /* return length: */
   return footer - header;
}


/**
 * @func SIRF_PROTO_Wrapper
 *
 * @param [in]  tSIRF_UINT8  *payload - input data buffer
 * @param [in]  tSIRF_UINT32 payload_length - input size
 * @param [out] tSIRF_UINT8  *data - output data buffer
 * @param [out] tSIRF_UINT32 *data_length - output size
 * 
 * @brief convert a data buffer to A0A2 <len> <data><chksm>B0B3 format
 *
 * @return  SIRF_SUCCESS, SIRF_FAILURE
 */

tSIRF_RESULT SIRF_PROTO_Wrapper( tSIRF_UINT8 *payload, tSIRF_UINT32 payload_length,
                                 tSIRF_UINT8 *data, tSIRF_UINT32 *data_length )
{
   tSIRF_UINT8  *ptr_in  = payload;
   tSIRF_UINT8  *ptr_out = data;
   tSIRF_UINT16  sum = 0;
   tSIRF_UINT32  i;

   /* validate the input parameters */
   if ((NULL == payload) || (NULL == data) || (NULL == data_length))
   {
      return SIRF_FAILURE;
   }

   *ptr_out++ = SIRF_MSG_HEAD0;
   *ptr_out++ = SIRF_MSG_HEAD1;
   *ptr_out++ = (tSIRF_UINT8)((payload_length>>8) & 0xFF);
   *ptr_out++ = (tSIRF_UINT8) (payload_length     & 0xFF);

   for ( i=0; i<payload_length; i++ )
   {
      *ptr_out++  = *ptr_in;
      /* note: use a=(cast)(a+b) over a+=b to avoid data overflow */
      sum         = (tSIRF_UINT16) (sum + *ptr_in++);
   }

   sum &= 0x7FFF;

   /* note: cast the next two to avoid data overflow */
   *ptr_out++ = (tSIRF_UINT8) ((sum>>8) & 0xFF);
   *ptr_out++ = (tSIRF_UINT8) (sum     & 0xFF);
   *ptr_out++ = SIRF_MSG_TAIL0;
   *ptr_out   = SIRF_MSG_TAIL1;

   *data_length = payload_length + 8;

   return SIRF_SUCCESS;

} /* SIRF_PROTO_Wrapper */


/***************************************************************************
 * Description:   
 * Parameters:   
 * Returns:      
 ***************************************************************************/

tSIRF_RESULT SIRF_PROTO_Unwrapper( tSIRF_UINT8 *data, tSIRF_UINT32 data_length,
                                   tSIRF_UINT8 *payload, tSIRF_UINT32 *payload_length )
{
   tSIRF_UINT8  *ptr_in  = data;
   tSIRF_UINT8  *ptr_out = payload;
   tSIRF_UINT16  len, checksum, sum = 0;
   tSIRF_UINT32  i;

   if ( data_length < 9 )
      return 1;

   if ( *ptr_in++ != SIRF_MSG_HEAD0 )
      return 2;

   if ( *ptr_in++ != SIRF_MSG_HEAD1 )
      return 3;

   len  = (*ptr_in++) << 8;
   len |= (*ptr_in++);

   if ( len > 4096 )
      return 4;

   for ( i=0; i<len; i++ )
   {
      *ptr_out++  = *ptr_in;
      sum        += *ptr_in++;
   }

   sum &= 0x7FFF;

   checksum  = (*ptr_in++) << 8;
   checksum |= (*ptr_in++);

   if ( sum != checksum )
      return 5;

   if ( *ptr_in++ != SIRF_MSG_TAIL0 )
      return 6;

   if ( *ptr_in   != SIRF_MSG_TAIL1 )
      return 7;

   *payload_length = len;

   return SIRF_SUCCESS;

} /* SIRF_PROTO_SSB_Unwrapper */

/**
 * @}
 * End of file.
 */
