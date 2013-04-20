/**
 * @addtogroup platform_src_sirf_util_proto
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *  Copyright (c) 2005-2008 by SiRF Technology, Inc.  All rights reserved. *
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
 * FILENAME:  sirf_proto_parse.c
 *
 * DESCRIPTION: 
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/
#include "sirf_types.h"
#include "sirf_msg.h"

#include "sirf_proto_parse.h"
#include "sirf_proto_common.h"

/***************************************************************************
 *   State Machine definitions for the parsing functions
 ***************************************************************************/
typedef enum Protocol_State_Machine_States_tag {
   stA0,      /* search for A0 */
   stA1,      /* search for A1 */
   stLen1,    /* start packet length bits */
   stLen2,    /* finish packet length bits */
   stLCH,     /* SL Protocols only --> find logical channel */
   stPacket,  /* gather binary data */
   stChkSm1,  /* start checksum bits */
   stChkSm2,  /* finish checksum bits */
   stB0,      /* search for B0 */
   stB3,      /* search for B3 */

   n_stDollar, /* search for "$" */
   n_stP,      /* 'P' */
   n_stS,      /* 'S' */
   n_stR,      /* 'R' */
   n_stF,      /* 'F' */
   n_stCR,     /* carriage return */
   n_stLF      /* line feed */
} Protocol_State_Machine_States;

/***************************************************************************
 *   Definitions
 ***************************************************************************/
#define MAX_BUFFER_SIZE 6000

#define   CR  (0x0D)       /* End of Sentence indicator */
#define   LF  (0x0A)       /* End of sentence indicator */

/***************************************************************************
 *   File Global Variables
 ***************************************************************************/
static t_callback_func f_callback = 0;

static Protocol_State_Machine_States pState = stA0; /* initial state for SSB */
static tSIRF_INT32 PktIndex = 0;  /* current location in the packet */
static tSIRF_INT32 PktLen = 0;    /* packet length */
static tSIRF_INT32 PktChksum = 0; /* packet checksum */
static tSIRF_UINT8 PktBuf[MAX_BUFFER_SIZE]; /* the packet */

/***************************************************************************
 * @brief:      Registration function for the callback routine
 * @param[in]:  callback_func  Function to register
 * @comm:       The registered function gets called when a complete
 *              message (SSB, NMEA, or SLC) is processed by the parsers.
 ***************************************************************************/
tSIRF_VOID SIRF_PROTO_Parse_Register( t_callback_func callback_func )
{
   f_callback = callback_func;
}

#ifdef SIRF_LOC

/***************************************************************************
 * @brief:      The parsing function for SLC messages
 * @param[in]:  Buf  Pointer to the data stream to process
 * @param[in]:  BytesRead  Number of bytes to process
 ***************************************************************************/
tSIRF_VOID SIRF_PROTO_SLParse( tSIRF_UINT8 *Buf, tSIRF_UINT32 BytesRead )
{
   tSIRF_UINT8 Byte = 0; /* holds the next raw data byte */
   tSIRF_UINT32 i = 0;   /* loop counter */

   /* got some data; go through the state machine(s) */
   for (i = 0; i < BytesRead; i++)
   {
      Byte = Buf[i]; 
      switch (pState)
      {
         case stA0:
            if (SIRF_MSG_HEAD0 == Byte)
            {
               pState = stA1;
            }
            else
            {
               pState = stA0;
            }
            break;
            
         case stA1:
            if (SIRF_MSG_HEAD1 == Byte)
            {
               pState = stLen1;
            }
            else
            {
               pState = stA0;
            }
            break;
            
         case stLen1:   /* assume packet length bits 15:08 */
            PktLen = Byte;
            pState = stLen2;
            break;
            
         case stLen2:   /* assume packet lenth bits 07:00 */
            PktLen = (PktLen << 0x08) | Byte;
            pState = stLCH;
            break;
            
         case stLCH:
            if ( (SIRF_LC_AI3 == Byte)   ||   /* Air-Interface, or AI3 Message */
                 (SIRF_LC_F == Byte)     ||   /* System Message */
                  /*(0x80 == Byte)       ||*/ /* PC-CP - see SiRFLoc CP Control ICD */
                  /*(0xCC == Byte)       ||*/ /* Time Transfer (SiRF Internal) */
                 (SIRF_LC_NMEA == Byte)  ||   /* NMEA Message */
                 (SIRF_LC_SSB == Byte)   ||   /* SiRF Binary Message */
                 (SIRF_LC_LPLC == Byte)  ||   /* LPL Cmd Message */
                 (SIRF_LC_DEBUG == Byte)      /* Debug Message */
               )
            {
               pState = stPacket;
               /* note: the checksum is calculated including this byte */
               PktIndex = 0;
               PktBuf [PktIndex] = Byte;
            }
            else
            {
               pState = stA0;
            }
            break;
            
         case stPacket:   /* count bytes until packet length, then advance the state */
            PktIndex++;
            PktBuf [PktIndex] = Byte;
            if ( (PktIndex + 1) < PktLen)
            {
               break;
            }
            pState = stChkSm1;
            break;
            
         case stChkSm1:   /* assume the checksum bits 15:08 and advance the state */
            PktChksum = Byte;
            pState = stChkSm2;
            break;
            
         case stChkSm2:   /* assume the checksum bits 07:00 and validate it */
            {
               tSIRF_UINT16 myChecksum;
               PktChksum = ((PktChksum << 0x08) | Byte) & 0x7FFF;
               myChecksum = SIRF_PROTO_ComputeChksum(PktBuf, PktLen);
               if (myChecksum == PktChksum)
               {
                  pState = stB0;
               }  
               else
               {
                  pState = stA0;
               }
            }
            break;
            
         case stB0:   /* if B0, advance the state, else reset */
            if (SIRF_MSG_TAIL0 == Byte)
            {
               pState = stB3;
            }
            else
            {
               pState = stA0;
            }
            break;
            
         case stB3:   /* if B3, we've got a good message, else reset */
            if (SIRF_MSG_TAIL1 == Byte)
            {
               /* Call the callback if it is available to process the message */
               if ( f_callback ) 
               {
                  f_callback( PktBuf, PktLen, PARSER_SIRFLOC );
               }
            }
            pState = stA0;
            break;
            
         default:
            pState = stA0;
            break;            
         } /* switch (pState) */
   } /* for ( i = 0...*/
}

#else

/***************************************************************************
 * @brief:      Make a decision on which parser (SSB or NMEA) to enter
 * @param[in]:  Byte  Data byte to parse
 * @return:     The next parser state
 * @comm:       This function is only for use with non-SLC messages. That is,
 *              messages formatted using either SSB or NMEA only for GSW.
 ***************************************************************************/
static Protocol_State_Machine_States Parse_Initial_Byte( tSIRF_UINT8 Byte )
{
   /* Default to look for SSB messages. If we don't match the initial byte of 
      either the SSB or the NMEA sequence, then stay at the default state. */
   Protocol_State_Machine_States newState = stA0;
   
   if (SIRF_MSG_HEAD0 == Byte) /* Found the beginning of an SSB message */
   {
      newState = stA1;
   }
   else if ('$' == Byte) /* Switch to NMEA parser */
   {
      PktIndex = 0;
      PktBuf[PktIndex++] = Byte;
      newState = n_stP;
   }
   return newState;
}

/***************************************************************************
 * @brief:      The parsing function for GSW messages (SSB and NMEA)
 * @param[in]:  Buf  Pointer to the data stream to process
 * @param[in]:  BytesRead  Number of bytes to process
 ***************************************************************************/
tSIRF_VOID SIRF_PROTO_Parse( tSIRF_UINT8 *Buf, tSIRF_UINT32 BytesRead )
{
   tSIRF_UINT8 Byte = 0; /* holds the next raw data byte */
   tSIRF_UINT32 i = 0;   /* loop counter */

   for (i = 0; i < BytesRead; i++)
   {
      Byte = Buf[i];

      switch(pState)
      {
         case stA0:   /* if A0, advance the state, else reset */
            pState = Parse_Initial_Byte( Byte );
            break;

         case stA1:   /* if A1, advance the state, else reset */
            if (SIRF_MSG_HEAD1 == Byte)
            {
               pState = stLen1;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case stLen1:   /* assume packet length bits 15:08 */
            PktLen = Byte;
            pState = stLen2;
            break;

         case stLen2:   /* assume packet lenth bits 07:00, but check for maximum size */
            PktLen = (PktLen << 0x08) | Byte;
            PktIndex = -1;
            pState = stPacket;
            break;

         case stPacket:   /* count bytes until packet length */
            PktIndex++;
            PktBuf [PktIndex] = Byte;
            if ( (PktIndex + 1) < PktLen)
            {
               break;
            }
            pState = stChkSm1;
            break;

         case stChkSm1:   /* assume checksum bits 15:08 */
            PktChksum = Byte;
            pState = stChkSm2;
            break;

         case stChkSm2:   /* assume checksum bits 07:00 */
         {
            tSIRF_UINT16 myChecksum;
            PktChksum = ((PktChksum << 0x08) | Byte) & 0x7FFF;
            myChecksum = SIRF_PROTO_ComputeChksum(PktBuf, PktLen);
            if (myChecksum == PktChksum)
            {
               pState = stB0;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;
         }
         case stB0:   /* if B0, advance the state, else reset */
            if (SIRF_MSG_TAIL0 == Byte)
            {
               pState = stB3;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case stB3:   /* if B3, we've got a good message */
            if (SIRF_MSG_TAIL1 == Byte)
            {
               /* Call the callback if it is available to process the message */
               if ( f_callback ) 
               {
                  f_callback( PktBuf, PktLen, PARSER_SSB );
               }   
            }
            pState = stA0;
            break;

         case n_stDollar:
            pState = Parse_Initial_Byte( Byte );
            break;

         case n_stP:   /* looking for the SiRF input signature "$P" */
            if ('P' == Byte)
            {
               pState = n_stS;
               PktBuf[PktIndex++] = Byte;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case n_stS:   /* looking for the SiRF input signature "$PS" */
            if ('S' == Byte)
            {
               pState = n_stR;
               PktBuf[PktIndex++] = Byte;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case n_stR:   /* looking for the SiRF input signature "$PSR" */
            if ('R' == Byte)
            {
               pState = n_stF;
               PktBuf[PktIndex++] = Byte;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case n_stF:   /* looking for the SiRF input signature "$PSRF" */
            if ('F' == Byte)
            {
               pState = n_stCR;
               PktBuf[PktIndex++] = Byte;
            }
            else
            {
               pState = Parse_Initial_Byte( Byte );
            }
            break;

         case n_stCR:   /* looking for the SiRF input signature "$PSRF<cr>" */
            if (CR == Byte)
            {
               pState = n_stLF; 
            }
            PktBuf[PktIndex++] = Byte;
            break;

         case n_stLF:   /* got the full signature, parse the message */
            if (LF == Byte)
            {
               PktBuf[PktIndex] = Byte;
               /* Call the callback if it is available */
               if ( f_callback ) 
               {
                  /* total length starts from 1 and not zero */
                  f_callback( PktBuf, PktIndex + 1, PARSER_NMEA );
               }
            }
            pState = n_stDollar; 
            break;

         default:
            pState = Parse_Initial_Byte( Byte );
            break;
      }
   }
}

#endif /* SIRF_LOC */

/**
 * @}
 */


