/**
 * @addtogroup platform_src_sirf_util_proto
 * @{
 */

/******************************************************************************
 *                                                                            *
 *                   SiRF Technology, Inc. GPS Software                       *
 *                                                                            *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.  *
 *                                                                            *
 *    This Software is protected by United States copyright laws and          *
 *    international treaties.  You may not reverse engineer, decompile        *
 *    or disassemble this Software.                                           *
 *                                                                            *
 *    WARNING:                                                                *
 *    This Software contains SiRF Technology Inc. s confidential and          *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,       *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED         *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this         *
 *    Software without SiRF Technology, Inc. s  express written               *
 *    permission.   Use of any portion of the contents of this Software       *
 *    is subject to and restricted by your signed written agreement with      *
 *    SiRF Technology, Inc.                                                   *
 *                                                                            *
 ******************************************************************************
 *
 * MODULE:  (SL)
 *
 * FILENAME:  sirf_proto_ai3.c
 *
 * DESCRIPTION: This file include the data structures and the functions that
 *              implement the ai3 manager sub module of SL module
 *
 ******************************************************************************/

#ifdef SIRF_LOC

/******************************************************************************
 *  INCLUDES
 ******************************************************************************/
#include <stdio.h>
#include <string.h> /* For memset and memcpy */

#include "sirf_msg.h"
#include "sirf_proto_ai3.h"
#include "sirf_codec_ai3.h"
#include "sirf_proto_common.h"

/******************************************************************************
 *  DEFINES
 ******************************************************************************/

/******************************************************************************
 *  TYPEDEFS
 ******************************************************************************/

/* enumeration for ai3 session open and close status */
typedef enum
{
   AI3_SESSION_CLOSED,
   AI3_SESSION_OPENED
} tAi3SesStatEnum;

/* enumeration for ai3 ack, nak and error */
typedef enum
{
   AI3_ACK       = 0x00,
   AI3_PARAM_ERR = 0xFD,
   AI3_ICD_ERR   = 0xFE,
   AI3_NAK       = 0xFF
} ai3AckNakEnum;

/* enumeration for ai3 SLC/CP message ack, nak and error */
typedef enum
{
   CPSLC_ACK        = 0x00,
   CPSLC_NBA_ERR    = 0xFD,    /* Nav bits aiding error */
   CPSLC_EPHREQ_ERR = 0xFE,    /* Ephemeris Request error */
   CPSLC_NACK       = 0xFF
} cpslcAckNakEnum;

/* states for ai3 message parsing state machine */
typedef enum
{
   FIRST_PKT_WAIT,
   MORE_PKTS_WAIT,
   ALL_PKTS_RCVD
}AI3bufStatus;

/******************************************************************************
 *  LOCAL DATA DECLARATIONS
 ******************************************************************************/

/* ai3 internal state information structure */
typedef struct AI3PacketInfo_tag
{
   tAi3SesStatEnum  ai3SesStat;     /* ai3 session is opened or closed         */
   tSIRF_UINT16     curByteInd;     /* index to bufffer being formed from pkts */
   tSIRF_UINT8      numPackets;     /* number of packets in an ai3 msg         */
   tSIRF_UINT8      curPacketInd;   /* packet number of the latest pkt received*/
   AI3bufStatus     bufStatus;      /* current buffer update state             */

   tSIRF_send_func  send_func;
   tSIRF_UINT8      ai3AcqExpected;
   tSIRF_input_func input_func;

   /* packet converted into compressed    */
   tSIRF_UINT8      compressedMsg[SIRF_MSG_AI3_MAX_STRUCT_SIZE];
} tAI3PacketInfo;

tAI3PacketInfo AI3PacketInfo;

 /*****************************************************************************
 *  FUNCTION PROTOTYPES
 ******************************************************************************/

/* referenced not defined */
tSIRF_VOID SIRF_PROTO_AI3_SendAckNakMsg( tSIRF_UINT8 reason );

#ifdef NAV_BIT_STRIPPING_HI_AI3
tSIRF_VOID SIRF_PROTO_AI3_SendEphAckNakMsg( tSIRF_UINT8 reason, tSIRF_UINT8 slcMsgId );
#endif

tSIRF_RESULT SIRF_PROTO_AI3_processPacket( tSIRF_UINT8 *pMsg, tSIRF_UINT32 Len,ai3AckNakEnum *pAckNackStatus );
tSIRF_VOID SIRF_PROTO_AI3_resetStateVariables( tSIRF_VOID );

tSIRF_UINT16 SIRF_PROTO_AI3_compressAI3Msg( tSIRF_UINT8 *in_buf,   tSIRF_UINT8 *out_buf, tSIRF_UINT16 in_len );
tSIRF_UINT32 SIRF_PROTO_AI3_decompressAI3Msg( tSIRF_UINT8 *input,
                                              tSIRF_UINT32 inputLen,
                                              tSIRF_UINT8 *output,
                                              tSIRF_UINT32 maxOutLen);
tSIRF_RESULT SIRF_PROTO_AI3_decompress_decode_inplace( tSIRF_UINT8  *msg_struct,
                                                       tSIRF_UINT32 *mid,
                                                       tSIRF_UINT32 *msg_length );

/******************************************************************************
 *  FUNCTION NAME: resetStateVariables
 *
 *  Input:   NONE
 *
 *  Output:   NONE
 *
 *  Description: resets the state variables of the message parsing state machine
 *
 ******************************************************************************/

tSIRF_VOID SIRF_PROTO_AI3_resetStateVariables( tSIRF_VOID )
{
   AI3PacketInfo.curPacketInd = 0;
   AI3PacketInfo.curByteInd   = 0;
   AI3PacketInfo.numPackets   = 0;
   AI3PacketInfo.bufStatus    = FIRST_PKT_WAIT;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_Init
 *
 *  Input:    None
 *
 *  Output:   NONE
 *
 *  Description: initializes the AI3 protocol status information
 *
 ******************************************************************************/

tSIRF_VOID SIRF_PROTO_AI3_Init( tSIRF_send_func send_func, tSIRF_input_func input_func )
{
   memset(&AI3PacketInfo,0,sizeof(AI3PacketInfo));
   SIRF_PROTO_AI3_resetStateVariables();
   AI3PacketInfo.send_func = send_func;
   AI3PacketInfo.input_func = input_func;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_Close
 *
 *  Input:    None
 *
 *  Output:   NONE
 *
 *  Description: Closes and cleans up the protocol variables
 *
 ******************************************************************************/

tSIRF_VOID SIRF_PROTO_AI3_Close( tSIRF_VOID )
{
   memset(&AI3PacketInfo,0,sizeof(AI3PacketInfo));
   SIRF_PROTO_AI3_resetStateVariables();
}

/******************************************************************************
 *  FUNCTION NAME: compressed_msg
 *
 *  Input:   NONE
 *
 *  Output:   SIRF_TRUE if the message ID is one that is compressed or needs
 *            compression, SIRF_FALSE otherwise
 *
 *  Description: Returns SIRF_TRUE if the message ID needs or is compressed, 
 *               SIRF_FALSE otherwise.
 *
 ******************************************************************************/
tSIRF_BOOL SIRF_PROTO_AI3_compressed_msg(tSIRF_UINT32 mid)
{
   switch (SIRF_GET_MID(mid))
   {
      case SIRF_GET_MID(SIRF_MSG_AI3_ACK_NACK):
#ifdef AI3_EPH_ALM_POLL_ENABLE
      case SIRF_GET_MID(SIRF_MSG_AI3_SLC_CP_ACK_NACK):
      case SIRF_GET_MID(SIRF_MSG_AI3_SLC_EPH_STATUS_REQ):
      case SIRF_GET_MID(SIRF_MSG_AI3_POLL_ALMANAC_REQ):
#endif
         return SIRF_FALSE;
      default:
         return SIRF_TRUE;
   }
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_PacketInput
 *
 *  Input:   pointer to the message payload and length of the message
 *
 *  Output:   SIRF_SUCCESS or MID of failed message
 *
 *  Description: this is the main entry function called by slMgr when an ai3
 *               message is received.
 *
 ******************************************************************************/
tSIRF_RESULT SIRF_PROTO_AI3_PacketInput(tSIRF_UINT8 *pMsg, tSIRF_UINT32 Len)
{
   tSIRF_RESULT retval = SIRF_SUCCESS;
   ai3AckNakEnum AckNackStatus = AI3_ACK;
   tSIRF_UINT8  *msg_struct;
   tSIRF_UINT32 mid;
   tSIRF_UINT32 msg_length;

   if (SIRF_PROTO_AI3_compressed_msg(pMsg[AI3_MSG_ID_POS]))
   {
      retval = SIRF_PROTO_AI3_processPacket(pMsg,Len,&AckNackStatus);
      if (retval != SIRF_SUCCESS) 
      {
         return SIRF_FAILURE;
      }
      msg_length = AI3PacketInfo.curByteInd;

      /* TODO: keep the GUI_Print calls? They used to call log_message */
#ifdef SL_DEBUG
      GUI_Print( "<%5lu> AI3:Msg id: %d, len: %d, num pkts : %d, pkt index: %d",
                 Timer_TimeSinceResetMs(),
                 pMsg[AI3_MSG_ID_POS],
                 Len,
                 pMsg[AI3_NUM_SEG_POS],
                 pMsg[AI3_SEG_IND_POS] );
#endif
   } 
   else 
   {
#ifdef SL_DEBUG
      GUI_Print( "<%5lu> AI3:Msg id: %d, len: %d",
                 Timer_TimeSinceResetMs(),
                 pMsg[AI3_MSG_ID_POS],
                 Len );
#endif

      /* This copy is always short, usually 2-3 bytes maxing out at about 8 */
      msg_length = Len - 1;
      /* Skip the logical channel */
      memcpy(AI3PacketInfo.compressedMsg,&pMsg[1],msg_length);
      AI3PacketInfo.bufStatus = ALL_PKTS_RCVD;
      retval = SIRF_SUCCESS;
   }

   if (retval != SIRF_SUCCESS)
   {
      /* There was an error so reset the state variables and send NAK  */
      SIRF_PROTO_AI3_resetStateVariables();

      if(AI3_ICD_ERR != AckNackStatus)
         SIRF_PROTO_AI3_SendAckNakMsg(AI3_NAK);

#ifdef SL_DEBUG
      GUI_Print ( "AI3:Error: last Pkt:%d, last pkts:%d rcvd pkt:%d, rcvd pkts:%d",
                  AI3PacketInfo.curPacketInd,
                  AI3PacketInfo.numPackets,
                  pMsg[AI3_SEG_IND_POS],
                  pMsg[AI3_NUM_SEG_POS] );
#endif
      retval = SIRF_FAILURE;
   } 
   else 
   {
      /* In the host software, this is a protocol ack, not a message ack
       * do it here instead of in the handler to keep the code seperate */
      if(AI3PacketInfo.bufStatus != ALL_PKTS_RCVD)
      {
#ifdef NAV_BIT_STRIPPING_HI_AI3
         if( SIRF_GET_MID(SIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV) == pMsg[AI3_MSG_ID_POS] ||
             SIRF_GET_MID(SIRF_MSG_AI3_NAV_SF_45_RSP) == pMsg[AI3_MSG_ID_POS] )
         {
            SIRF_PROTO_AI3_SendEphAckNakMsg(CPSLC_ACK, pMsg[AI3_MSG_ID_POS]);
         } 
         else 
         {
#endif
            SIRF_PROTO_AI3_SendAckNakMsg(AI3_ACK);
#ifdef NAV_BIT_STRIPPING_HI_AI3
         }
#endif
      }
   }

   /* If everything is good decompress and decode the message */
   if ((SIRF_SUCCESS == retval) && (ALL_PKTS_RCVD == AI3PacketInfo.bufStatus))
   {
      /* This is a trick used to reduce stack size, it wipes out the
       * original compressed message, but after we are done with it thus the
       * only stack needed is for the decompressedMsg
       */
      msg_struct = AI3PacketInfo.compressedMsg;
      if (SIRF_SUCCESS == SIRF_PROTO_AI3_decompress_decode_inplace(msg_struct,&mid,&msg_length))
      {
         if (AI3PacketInfo.input_func)
         {
            /* Check to see if this is an ack-nack message we should absorb
             * instead of sending to the AI3 SM.  This happens when we send
             * several packets for one message */
            if(AI3PacketInfo.ai3AcqExpected && SIRF_MSG_AI3_ACK_NACK == mid)
            {
               tSIRF_MSG_AI3_ACK_NACK *ack = (tSIRF_MSG_AI3_ACK_NACK *)msg_struct;
               AI3PacketInfo.ai3AcqExpected--;
               if (ack->acknak != AI3_ACK)
               {
                  /* This is a nak, pass it into the state machine */
                  retval = AI3PacketInfo.input_func(mid,msg_struct,msg_length);
               }
            } 
            else 
            {

               /* Input the message */
               retval = AI3PacketInfo.input_func(mid,msg_struct,msg_length);
            }
         }
      }
   }

   return retval;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_decompress_decode_inplace
 *
 *  Input:   Pointer to the message structure, a pointer to the message id,
 *           and a pointer to the message length.
 *
 *  Output:  SIRF_FAILURE on failures. SIRF_SUCCESS if the message uncompressed
 *           and decoded.
 *
 *  Description: Decompress and decode the AI3 message
 *
 ******************************************************************************/
tSIRF_RESULT SIRF_PROTO_AI3_decompress_decode_inplace( tSIRF_UINT8  *msg_struct,
                                                       tSIRF_UINT32 *mid,
                                                       tSIRF_UINT32 *msg_length)
{
   /* This is a large amount of stack space, and thus is declared here
    * and another if clause is used to remove it from the stack
    */
   tSIRF_UINT8 decompressedMsgBuf[SIRF_MSG_AI3_MAX_STRUCT_SIZE];
   tSIRF_UINT8 *decompressedMsg = decompressedMsgBuf;
   tSIRF_UINT32 decompress_length;
   tSIRF_RESULT retval = SIRF_SUCCESS;

   /* Grab the message id */
   *mid = msg_struct[0];

   /* AI3 Decode needs these */
   decompressedMsg[0] = SIRF_LC_AI3; /* Logical Channel */
   decompressedMsg[1] = msg_struct[0]; /* Message ID */

   if (SIRF_PROTO_AI3_compressed_msg(*mid))
   {
      /* Decompress the message */
      decompress_length =
         SIRF_PROTO_AI3_decompressAI3Msg( &msg_struct[1],
                                          *msg_length - 1,
                                          &decompressedMsg[2],
                                          SIRF_MSG_AI3_MAX_STRUCT_SIZE - 2 );

      /* It is an error if the length is zero */
      if ( 0 == decompress_length )
      {
         SIRF_PROTO_AI3_SendAckNakMsg(AI3_NAK);
#ifdef SL_DEBUG
         GUI_Print( "AI3:Error in decompressing AI3 Request message" );
#endif
         retval = SIRF_FAILURE;
      }
   } 
   else 
   {
      /* Skip the MID as it was copied above */
      memcpy(&decompressedMsg[2],&msg_struct[1],*msg_length - 1);
      /* Subtract 2 because decompressed length subtracts MID */
      decompress_length = (*msg_length) - 1;
   }

   if (SIRF_SUCCESS == retval) 
   {
      /* Plus 2 for Logical channel and msg_id */
      retval = SIRF_CODEC_AI3_Decode(decompressedMsg,(tSIRF_UINT32)(decompress_length)+2,mid,msg_struct,msg_length);
   }

   return retval;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_processPacket
 *
 *  Input:   pointer to the message and length of the message
 *
 *  Output:   NONE
 *
 *  Description: This function processes the ai3 packet when the state of the
 *               message parsing state machine is FIRST_PKT_WAIT
 *
 ******************************************************************************/

tSIRF_RESULT SIRF_PROTO_AI3_processPacket(tSIRF_UINT8 *pMsg, tSIRF_UINT32 Len, ai3AckNakEnum *pAckNackStatus)
{
   tSIRF_UINT8 mid = pMsg[AI3_MSG_ID_POS];
   tSIRF_UINT8 seg_num = pMsg[AI3_SEG_IND_POS];
   tSIRF_UINT8 num_seg = pMsg[AI3_NUM_SEG_POS];

   (void) pAckNackStatus; /* unused */

   /* Step one, verify this is the correct packet and in order, first we can
    * receive segment 1 at any time and it is valid, we just reset our states.
    * Otherwise the segment received has to match the segment expected and
    * fit our buffer size */
   if ((1 == seg_num) && ((Len-AI3_SEG_START) < SIRF_MSG_AI3_MAX_STRUCT_SIZE))
   {
      /* Reset the state variables */
      SIRF_PROTO_AI3_resetStateVariables();

      /* Update the receive state metrics */
      AI3PacketInfo.numPackets   = num_seg;
      memset( AI3PacketInfo.compressedMsg, 0, sizeof(AI3PacketInfo.compressedMsg) );
      AI3PacketInfo.curByteInd = 0;
      /* fill the message id when first segment is received */
      AI3PacketInfo.compressedMsg[AI3PacketInfo.curByteInd++]
         = mid;

   } else if ((AI3PacketInfo.bufStatus != MORE_PKTS_WAIT) ||
              (seg_num != AI3PacketInfo.curPacketInd + 1) ||
              (seg_num > num_seg) ||
              (num_seg != AI3PacketInfo.numPackets) ||
              /* Message too big */
              (((Len-AI3_SEG_START)+AI3PacketInfo.curByteInd) > SIRF_MSG_AI3_MAX_STRUCT_SIZE))
   {
      return SIRF_FAILURE;
   }

   /* Copy the compressed payload data into the compression buffer */
   memcpy(&AI3PacketInfo.compressedMsg[AI3PacketInfo.curByteInd],
          &pMsg[AI3_SEG_START],
          Len - AI3_SEG_START);

   AI3PacketInfo.curPacketInd = seg_num;
   AI3PacketInfo.curByteInd   += (tSIRF_UINT16)Len - AI3_SEG_START;

   /* If this is the first and last packet in the sequence, then process it as
    * the last one */
   if(seg_num == num_seg)
   {
      AI3PacketInfo.bufStatus = ALL_PKTS_RCVD;
   } 
   else 
   {
      AI3PacketInfo.bufStatus = MORE_PKTS_WAIT;
   }
   return SIRF_SUCCESS;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_Output
 *
 *  Input:   message id, pointer to structure
 *
 *  Output:   NONE
 *
 *  Description: This function is called from ai3 it encodes and sends
 *               an AI3 message to CP
 *
 ******************************************************************************/

tSIRF_RESULT SIRF_PROTO_AI3_Output(tSIRF_UINT32 msg_id, tSIRF_VOID *msg, tSIRF_UINT32 msg_len)
{
   tSIRF_UINT32 packet_len;
   /* Used to encode data, and to form the temporary message */
   tSIRF_UINT8 compBuf[SIRF_MSG_AI3_MAX_STRUCT_SIZE];
   /* Strictly used for the compressed message, after encoding the message
    * we can reuse the buffer as it will be at least as large as the size of
    * SIRF_MSG_AI3_MAX_STRUCT_SIZE */
   tSIRF_UINT8* encBuf = msg;
   tSIRF_UINT32 length, msgSize = msg_len;
   tSIRF_RESULT ret;
   tSIRF_UINT8  fragCnt,i;

   /* If we go over stack size this will stomp on data actively, really the
    * only good reason for keeping this around */
   memcpy(compBuf,msg,msg_len);

   /* Encoding a message will always be <= original size because this removes */
   /* All packing */
   ret = SIRF_CODEC_AI3_Encode(msg_id,compBuf,msg_len,encBuf,&msgSize);

   if (SIRF_SUCCESS != ret)
   {
      return ret;
   }

   if (SIRF_PROTO_AI3_compressed_msg(msg_id))
   {
      /* The Compressed message in the worst case is 2 bytes larger than the
       *  original message, so we cannot re-use the encBuf to compress into */
      length = SIRF_PROTO_AI3_compressAI3Msg(encBuf,compBuf,(tSIRF_UINT16)(msgSize));

#ifdef SL_DEBUG
      GUI_Print( "AI3:Resp: enc msgSize: %d, compressed length: %d", msgSize,length );
#endif
      /* fragment the message  */
      fragCnt = (tSIRF_UINT8)(length / FRAG_SIZE + 1);
      AI3PacketInfo.ai3AcqExpected = fragCnt -1;
      for(i = 0; i < fragCnt; i++)
      {
         tSIRF_INT32 curLength;
         /* Additional stack is needed here because we cannot guarantee that
         * the size of msg is large enough for a full segment after the header
         * and footer have been added */
         tSIRF_UINT8 send_buf[SIRF_MSG_AI3_MAX_MESSAGE_LEN];

         curLength = (length > FRAG_SIZE) ? FRAG_SIZE:length;
         length    = length - curLength;
         send_buf[RAW_HEADER_OFFSET+AI3_LC_POS]      = SIRF_GET_LC(msg_id);
         send_buf[RAW_HEADER_OFFSET+AI3_MSG_ID_POS]  = SIRF_GET_MID(msg_id);
         send_buf[RAW_HEADER_OFFSET+AI3_NUM_SEG_POS] = fragCnt;
         send_buf[RAW_HEADER_OFFSET+AI3_SEG_IND_POS] = i + 1;
         memcpy(&send_buf[RAW_HEADER_OFFSET+AI3_SEG_START],
                &compBuf[i*FRAG_SIZE],
                curLength);

         packet_len = SIRF_PROTO_AddHeaderFooter(
            send_buf,
            &send_buf[RAW_HEADER_OFFSET+AI3_SEG_START+curLength],
            curLength+AI3_SEG_START);

         if ( AI3PacketInfo.send_func )
         {
            if ( SIRF_FAILURE == AI3PacketInfo.send_func(send_buf, packet_len) )
            {
               return SIRF_FAILURE;
            }
         }
      }
   } 
   else 
   {

      length = msgSize;
      compBuf[RAW_HEADER_OFFSET+AI3_LC_POS]      = SIRF_GET_LC(msg_id);
      compBuf[RAW_HEADER_OFFSET+AI3_MSG_ID_POS]  = SIRF_GET_MID(msg_id);
      memcpy(&compBuf[RAW_HEADER_OFFSET+2],
             compBuf,
             length);

      packet_len = SIRF_PROTO_AddHeaderFooter(
         compBuf,
         &compBuf[RAW_HEADER_OFFSET+2+length],
         length+2);

      if ( AI3PacketInfo.send_func )
      {
         if ( SIRF_FAILURE == AI3PacketInfo.send_func(compBuf,packet_len) )
         {
            return SIRF_FAILURE;
         }
      }
   }

   return SIRF_SUCCESS;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_SendAckNakMsg
 *
 *  Input:   ACK/NAK
 *
 *  Output:   NONE
 *
 *  Description: This function send an Ack of Nak message to CP
 *
 ******************************************************************************/

tSIRF_VOID SIRF_PROTO_AI3_SendAckNakMsg(tSIRF_UINT8 reason)
{
   tSIRF_MSG_AI3_ACK_NACK AI3_AckNak;

   AI3_AckNak.acknak = reason;

   SIRF_PROTO_AI3_Output(SIRF_MSG_AI3_ACK_NACK,&AI3_AckNak,sizeof(AI3_AckNak));
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_SendEphAckNakMsg
 *
 *  Input:   ACK/NAK
 *
 *  Output:   NONE
 *
 *  Description: This function send an CP/SLC Ack or Nak message to CP
 *
 ******************************************************************************/
#ifdef NAV_BIT_STRIPPING_HI_AI3
tSIRF_VOID SIRF_PROTO_AI3_SendEphAckNakMsg(tSIRF_UINT8 reason, tSIRF_UINT8 slcMsgId)
{
   tSIRF_MSG_AI3_SLC_CP_ACK_NACK AI3_EphAckNak;

   AI3_EphAckNak.ack_nack_error = reason;
   AI3_EphAckNak.slc_msg_id = slcMsgId;
   AI3_EphAckNak.spare = 0x0;

   SIRF_PROTO_AI3_Output(SIRF_MSG_AI3_SLC_CP_ACK_NACK,&AI3_EphAckNak,sizeof(AI3_EphAckNak));
}
#endif

/** 
 * 
 *  Description: uncompresses the input buffer and returns the length and
 *               uncompressed buffer
 * 
 * @param input     Compressed data to decompress
 * @param inputLen  Length of input
 * @param output    location to store the decompressed data
 * @param maxOutLen length of output
 * 
 * @return Actual length written to output or 0 on error
 */
tSIRF_UINT32 SIRF_PROTO_AI3_decompressAI3Msg( tSIRF_UINT8 *input,
                                              tSIRF_UINT32 inputLen,
                                              tSIRF_UINT8 *output,
                                              tSIRF_UINT32 maxOutLen )
{
   tSIRF_UINT8  byte_data[2];
   tSIRF_UINT16 data_length, i;
   tSIRF_UINT32 m, n;

   m = 0;
   n = 0;
   while(( m < inputLen) && (n < maxOutLen))
   {
      byte_data[0] = *(input + m);
      byte_data[1] = *(input + m +1);
      m += 2;
      data_length = (byte_data[0] & 0x7F)*256 + byte_data[1];
      if((n + data_length) > maxOutLen)
      {
         return 0; /* Failure return */
      }

      if( 0x80 == (byte_data[0] & 0x80))
      {
         memset((output + n), *(input + m), data_length);
         n += data_length;
         m++;
      }
      else
      {
         for(i = 0; i < data_length; i++)
            *(output + n + i) = *(input + m + i);
         n += data_length;
         m += data_length;
      }
   }

   if(( m < inputLen) && ( n >= maxOutLen))
   {
      return 0; /* Failure return */
   }

   return n;
}

/******************************************************************************
 *  FUNCTION NAME: SIRF_PROTO_AI3_compressAI3Msg
 *
 *  Input:   input buffer, output  buffer, input length
 *
 *  Output:   NONE
 *
 *  Description: compresses the input buffer and returns the length and compressed
 *               buffer
 *
 ******************************************************************************/

tSIRF_UINT16 SIRF_PROTO_AI3_compressAI3Msg(tSIRF_UINT8 *in_buf,  tSIRF_UINT8 *out_buf, tSIRF_UINT16 in_len)
{
   tSIRF_UINT16 run_cnt,
      non_run_cnt,
      in_ind, out_ind,
      cur_ind;
   tSIRF_BOOL  ready_to_copy,match_end;

   in_ind        = 0;
   run_cnt       = 0;
   non_run_cnt   = 0;
   out_ind       = 0;
   cur_ind       = 0;
   ready_to_copy = SIRF_FALSE;
   match_end     = SIRF_FALSE;

   while(cur_ind < in_len - 1)
   {
      if(in_buf[cur_ind] == in_buf[cur_ind +1])
      {
         run_cnt++;
         if( run_cnt < 4)
         {
            non_run_cnt++;
         }
         else
         {
            match_end = SIRF_TRUE;
            if((non_run_cnt >= run_cnt) && (ready_to_copy))
            {
               /* copy non run length */
               out_buf[out_ind++] = 0x00 | ((non_run_cnt - run_cnt + 1) >> 8);
               out_buf[out_ind++] = (tSIRF_UINT8)(non_run_cnt - run_cnt + 1);
               memcpy(&out_buf[out_ind],&in_buf[in_ind],(non_run_cnt - run_cnt + 1));
               in_ind = in_ind + non_run_cnt - run_cnt + 1;
               out_ind = out_ind + non_run_cnt - run_cnt + 1;
               ready_to_copy = SIRF_FALSE;
            }
            non_run_cnt = 0;
         }
      }
      else
      {
         if(match_end)
         {
            run_cnt++;
            match_end = SIRF_FALSE;
         }
         else
         {
            non_run_cnt++;
         }
         if(run_cnt >= 4)
         {
            if((non_run_cnt >= run_cnt) && (ready_to_copy))
            {
               /* copy non run length */
               out_buf[out_ind++] = 0x00 | ((non_run_cnt - run_cnt + 1) >> 8);
               out_buf[out_ind++] = (tSIRF_UINT8)(non_run_cnt - run_cnt + 1);
               memcpy(&out_buf[out_ind],&in_buf[in_ind],(non_run_cnt - run_cnt + 1));
               in_ind = in_ind + non_run_cnt - run_cnt + 1;
               out_ind = out_ind + non_run_cnt - run_cnt + 1;
               ready_to_copy = SIRF_FALSE;
            }
            non_run_cnt = 0;
            /* copy run length */
            out_buf[out_ind++] = 0x80 | ((run_cnt) >> 8);
            out_buf[out_ind++] = (tSIRF_UINT8)(run_cnt);
            out_buf[out_ind++] = in_buf[in_ind];
            in_ind = in_ind + run_cnt;
         }
         run_cnt = 0;
         ready_to_copy = SIRF_TRUE;
      }
      cur_ind++;
   }
   /* copy remaining run length and non run length */
   if(run_cnt >= 4)
   {
      out_buf[out_ind++] = 0x80 | ((run_cnt + 1) >> 8);
      out_buf[out_ind++] = (tSIRF_UINT8)(run_cnt + 1);
      out_buf[out_ind++] = in_buf[in_ind];
   }
   else
   {
      out_buf[out_ind++] = 0x00 | ((non_run_cnt + 1) >> 8);
      out_buf[out_ind++] = (tSIRF_UINT8)(non_run_cnt + 1);
      memcpy(&out_buf[out_ind],&in_buf[in_ind],(non_run_cnt + 1));
      out_ind = out_ind + non_run_cnt + 1;
   }
   return out_ind;
}

#endif /* SIRF_LOC */

/**
 * @}
 */
