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
 * FILENAME:  sirf_codec_nmea.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the NMEA protocol from and to the Tracker API 
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/

#ifdef SIRF_CODEC_NMEA

#define MAX_NMEA_CHANNELS 50

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "sirf_types.h"
#include "sirf_codec.h"
#include "sirf_codec_nmea.h"
//#include "sirfnav_ui_io.h"

//!! #include "string_sif.h"

/* To avoid compile warnings */
extern size_t strlcpy(char *dst, const char *src, size_t size);
extern size_t strlcat(char *dst, const char *src, size_t size);

/* This include should be removed in the future when the NMEA code is brought
   out of the SN3 library. For now, it is needed for MAX_NMEA_CHANNELS */
//!! #include "sdk/gpstype.h"

/***************************************************************************
 * Global Variables
 ***************************************************************************/

tSIRF_CHAR *g_nmea_null_char;
#define ABS(a)       (((a) < 0) ? (-(a)) : (a))
float const Power10[] ={ 1,1e1f,1e2f,1e3f,1e4f,1e5f,1e6f,1e7f,1e8f,1e9f,1e10f,1e11f,1e12f };

#define SIRF_MSG_SSB_MODE_MASK                  ( 0x07 )
#define SIRF_MSG_SSB_MODE_DGPS_USED             ( 0x80 )

/***************************************************************************
 * Description: Convert a floating point value to a string
 * Parameters:  input number, format: left.right, output string variable
 * Returns:     nothing
 ***************************************************************************/
static tSIRF_VOID Float2AsciiPad( float FValue, tSIRF_INT16 left, tSIRF_INT16 right, tSIRF_CHAR *pOutString )
{
   if ((tSIRF_CHAR *) NULL != pOutString)
   {
      long IValue;
      long NValue;
      tSIRF_CHAR theString[20];
      tSIRF_CHAR *pEndString = &theString[19];
      tSIRF_CHAR *pDot = pEndString - right;
      tSIRF_CHAR *pLeftEnd = pDot - left;
      tSIRF_CHAR LsChar;

      IValue = (long) ( FValue * Power10[right + 1] );
      if ( IValue < 0 )
      {
         *pOutString++ = '-';
         IValue = -IValue;
         pLeftEnd++;
      }

      *pDot = '.';
      IValue = IValue / 10;  /* truncate instead of roundoff */

      for ( ; pEndString >= pLeftEnd; pEndString-- )
      {
         if( pEndString != pDot )
         {
            if( IValue )
            {
               NValue = IValue / 10;
               LsChar = (tSIRF_CHAR) (IValue - 10 * NValue);
               *pEndString = '0' + LsChar;
               IValue = NValue;
            }
            else
            {
               *pEndString = '0';
            }
         }
      } 

      for ( pEndString++; pEndString < &theString[20]; )
      {
         *pOutString++ = *pEndString++;
      }
      *pOutString = 0;
   }
} /* Float2AsciiPad */

/*=============================================================================
 *
 * Inputs:  pBuf  pointer to message string, null terminated
 *          msgid ID of message, to determine if ck sum is T/F
 *                use to index UI_ msg table
 *
 * Outputs: None
 *
 * Desc:    if checksum is enabled for that particular message, calculate
 *          the checksum and strcat() the checksum to the message
 *
 *          It is the caller function responsibility to ensure enough buffer
 *          space for the '*CK'  astrisk checksum delimiter and 2 character
 *          checksum
 *
 *---------------------------------------------------------------------------*/
static tSIRF_VOID AddCheckSum( tSIRF_CHAR *pBuf)
{
    tSIRF_UINT8 ckSum = 0;

   while( *pBuf != '\0' )
   {
      ckSum ^= *pBuf++;
   }
   g_nmea_null_char = pBuf + sprintf( pBuf, "*%02lX\x0D\x0A", (unsigned long)ckSum );  /* +"*CK\x0d\x0a" */

} /* AddCheckSum() */

/******************************************************************************
 *
 * Description:
 *      The GGA-Global Positioning System Fix Data describes Time, position
 *      and fix related data for a GPS receiver.
 *
 ******************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GGA(tSIRF_MSG_SSB_GEODETIC_NAVIGATION *data, tSIRF_CHAR *buf )
{
   tSIRF_RESULT tRet = SIRF_FAILURE;
   if (( (tSIRF_MSG_SSB_GEODETIC_NAVIGATION *) NULL != data) && (tSIRF_CHAR *) NULL != buf)
   {
      tSIRF_DOUBLE latInDegree = 0;
      tSIRF_INT16 latDD = 0;
      tSIRF_DOUBLE latMMmm = 0;
      tSIRF_DOUBLE lonInDegree = 0;
      tSIRF_INT16 lonDDD = 0;
      tSIRF_DOUBLE lonMMmm = 0;
      tSIRF_INT32 NavMode = 0;
      tSIRF_CHAR latMMmmbuf[20] = "";
      tSIRF_CHAR lonMMmmbuf[20] = "";

      if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
      {
         NavMode = 0;
      }
      else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
      {
         NavMode = 6;
      }
      else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
      {
         NavMode = 2;
      }
      else
      {
         NavMode = 1;
      }

      latInDegree = data->lat * 1e-7;
      lonInDegree = data->lon * 1e-7;

      /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
      latDD   = (tSIRF_INT16)latInDegree;
      latMMmm = fabs( (latInDegree - latDD) * 60.0 );

      /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
      lonDDD  = (tSIRF_INT16)lonInDegree;
      lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

      /** 
       Note: function receives a msg #41 as input, which contains two height formats   
       ellipsoid height, and sea leval height.  The geoid separation that is required   
       for this message can be back-calculated as follows: geoid = (ellipse - msl)
      */    

      Float2AsciiPad( (float) latMMmm, 2, 4, latMMmmbuf );
      Float2AsciiPad( (float) lonMMmm, 2, 4, lonMMmmbuf );

      sprintf( buf, "$GPGGA,%02d%02d%06.3f,%02d%s,%c,%03d%s,%c,%ld,%02ld,%.1f,%.1f,M,%.1f,M,,",
      data->utc_hour, data->utc_min, data->utc_sec*1e-3,
      ABS( latDD ),                    /* tSIRF_INT32 deg   */
      latMMmmbuf,                      /* tSIRF_INT32 min   */
      latInDegree >= 0.0 ? 'N' : 'S',  /* direction */
      ABS( lonDDD ),                   /* tSIRF_INT32 deg   */
      lonMMmmbuf,                      /* tSIRF_INT32 min   */
      lonInDegree < 0.0 ? 'W' : 'E',   /* direction */
      (long)NavMode,
      (long)data->sv_used_cnt,
      (double)data->hdop * SIRF_MSG_SSB_DOP_LSB,
      data->alt_msl*1e-2,
      (data->alt_ellips - data->alt_msl)*1e-2 );

      AddCheckSum( buf+1 );

      tRet = SIRF_SUCCESS;
   }
   return tRet;
} /* SIRF_CODEC_NMEA_Encode_GGA */

/**************************************************************************
*                                                                         *
* OutputRMC                                                               *
*                                                                         *
* Description:                                                            *
*      RMC-Recommended minimum specific GPS/Transit data                   *
*     and fix related data for a GPS receiver.                            *
*                                                                         *
****************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_RMC( tSIRF_MSG_SSB_GEODETIC_NAVIGATION *data, tSIRF_CHAR *buf )
{
   tSIRF_DOUBLE latInDegree=0;
   tSIRF_INT16 latDD=0;
   tSIRF_DOUBLE latMMmm=0;

   tSIRF_DOUBLE lonInDegree=0;
   tSIRF_INT16 lonDDD=0;
   tSIRF_DOUBLE lonMMmm=0;

   tSIRF_CHAR   NavMode;
   tSIRF_CHAR   NavStatus;
   tSIRF_CHAR   latMMmmbuf[20]="";
   tSIRF_CHAR   lonMMmmbuf[20]="";

   tSIRF_DOUBLE speed_knots;

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
   {
      NavMode   = 'N';
      NavStatus = 'V';
   }
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
   {
      NavMode   = 'E';
      NavStatus = 'A';
   }
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
   {
      NavMode   = 'D';
      NavStatus = 'A';
   }
   else
   {
      NavMode   = 'A';
      NavStatus = 'A';
   }

   latInDegree = data->lat*1e-7;
   lonInDegree = data->lon*1e-7;

   /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
   latDD   = (tSIRF_INT16)latInDegree;
   latMMmm = fabs( (latInDegree - latDD) * 60.0 );

   /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
   lonDDD  = (tSIRF_INT16)lonInDegree;
   lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

   Float2AsciiPad( (float) latMMmm, 2, 4, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 4, lonMMmmbuf );

   speed_knots = data->sog*1e-2 * 3600.0 / (0.3048 * 6076.0);     /* convert to knots */

   Float2AsciiPad( (float) latMMmm, 2, 4, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 4, lonMMmmbuf );

   sprintf( buf, "$GPRMC,%02d%02d%06.3f,%c,%02d%s,%c,%03d%s,%c,%.1f,%.1f,%02d%02d%02d,,,%c",
            data->utc_hour, data->utc_min, data->utc_sec*1e-3,
            NavStatus,
            ABS( latDD ), latMMmmbuf, latInDegree >= 0.0 ? 'N' : 'S',
            ABS( lonDDD ), lonMMmmbuf, lonInDegree < 0.0 ? 'W' : 'E',
            speed_knots,
            data->hdg*1e-2,
            data->utc_day, data->utc_month, data->utc_year%100,
            NavMode );

   AddCheckSum( buf+1 );

   return SIRF_SUCCESS;
} /* SIRF_CODEC_NMEA_Encode_RMC */

/******************************************************************************
 *
 *   OutputGLL
 *
 * Description:
 *     Geographic Position-Latidude/Longitude
 *
 ******************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GLL( tSIRF_MSG_SSB_GEODETIC_NAVIGATION *data, tSIRF_CHAR *buf )
{
   tSIRF_DOUBLE latInDegree=0;
   tSIRF_INT16 latDD=0;
   tSIRF_DOUBLE latMMmm=0;

   tSIRF_DOUBLE lonInDegree=0;
   tSIRF_INT16 lonDDD=0;
   tSIRF_DOUBLE lonMMmm=0;

   tSIRF_CHAR   NavMode;
   tSIRF_CHAR   NavStatus;

   tSIRF_CHAR   latMMmmbuf[20]="";
   tSIRF_CHAR   lonMMmmbuf[20]="";

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
   {
      NavMode   = 'N';
      NavStatus = 'V';
   }
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
   {
      NavMode   = 'E';
      NavStatus = 'A';
   }
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
   {
      NavMode   = 'D';
      NavStatus = 'A';
   }
   else
   {
      NavMode   = 'A';
      NavStatus = 'A';
   }

   latInDegree = data->lat*1e-7;
   lonInDegree = data->lon*1e-7;

   /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
   latDD   = (tSIRF_INT16)latInDegree;
   latMMmm = fabs( (latInDegree - latDD) * 60.0 );

   /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
   lonDDD  = (tSIRF_INT16)lonInDegree;
   lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

   Float2AsciiPad( (float) latMMmm, 2, 4, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 4, lonMMmmbuf );

   sprintf( buf, "$GPGLL,%02d%s,%c,%03d%s,%c,%02d%02d%06.3f,%c,%c",
            ABS( latDD ),                    /* int deg*/
            latMMmmbuf,                      /* int min*/
            latInDegree >= 0.0 ? 'N' : 'S',
            ABS( lonDDD ),                   /* int deg*/
            lonMMmmbuf,                      /* int min*/
            lonInDegree  < 0.0 ? 'W' : 'E',
            data->utc_hour, data->utc_min, data->utc_sec*1e-3,
            NavStatus,
            NavMode );

   AddCheckSum( buf+1 );

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_GLL */

/******************************************************************************
 *
 *   OutputGSA
 *
 * Description:
 *     GPS receiver operating mode, satellites used in the navigation
 *     solution reported by the $--GGA sentence, and DOP values.
 *
 ******************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSA( tSIRF_MSG_SSB_GEODETIC_NAVIGATION *data, tSIRF_CHAR *buf )
{
   tSIRF_INT32 i;
   tSIRF_INT32 NavMode;

   tSIRF_CHAR   str[128]="";

   switch ( data->nav_mode & SIRF_MSG_SSB_MODE_MASK )
   {
      case 4:
      case 6:
               NavMode = 3;  /* 3-d fix */
               break;
      case 0:
               NavMode = 1;  /* no nav */
               break;
      default:
               NavMode = 2;  /* 2-d fix */
               break;
   }

   /* put active chans svid */
   for ( i = 0; i < 32; i++ )
   {
      if (data->sv_used & (1<<i))
      {
         tSIRF_CHAR str1[128];
         snprintf(str1, sizeof(str1), "%02d,", i+1);
         strlcat(str, str1, sizeof(str));
      }
   } /* for */

   /* fill the other chan fields with ,,,, */
   for( i = 0; i < MAX_NMEA_CHANNELS - data->sv_used_cnt; i++ )
   {
      strlcat(str, ",", sizeof(str));
   }
   /* format is ,pdop,hdop,vdop,   only have hdop :->  ,,hdop,  */
   sprintf(buf, "$GPGSA,A,%1d,%s,,%.1f,",
            NavMode,
            str,
            (double)data->hdop * SIRF_MSG_SSB_DOP_LSB );

   AddCheckSum( buf+1 );

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_GSA */

/**************************************************************************
*                                                                         *
* OutputVTG                                                               *
*                                                                         *
* Description:                                                            *
*     The VTG - Course Over Ground and Ground Speed                       *
*               The actual course and speed relative to the ground.       *
*                                                                         *
****************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode_VTG( tSIRF_MSG_SSB_GEODETIC_NAVIGATION *data, tSIRF_CHAR *buf )
{
   tSIRF_DOUBLE speed_knots=0;
   tSIRF_DOUBLE speed_kmh=0;

   tSIRF_CHAR   NavMode;

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
      NavMode = 'N';
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
      NavMode = 'E';
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
      NavMode = 'D';
   else
      NavMode = 'A';

   speed_knots = data->sog*1e-2 * 3600.0 / (0.3048 * 6076.0);     /* convert to knots */
   speed_kmh = data->sog*1e-2 * 3600.0 / 1000.0;                  /* convert to kmh */

   sprintf (buf, "$GPVTG,%.1f,T,,M,%.1f,N,%.1f,K,%c",
                  data->hdg*1e-2,
                  speed_knots,
                  speed_kmh,
                  NavMode);

   AddCheckSum( buf+1 );

   return SIRF_SUCCESS;
}  /* SIRF_CODEC_NMEA_Encode_VTG */

/*
***************************************************************************
*                                                                         *
* OutputGSV                                                               *
*                                                                         *
* Description:                                                            *
*                                                                         *
*     The GSV - GPS Satellites in view                                    *
*     Number of satellites in view, PRN numbers, elevation, azimuth,      *
*     and SNR value.  Four satellites max. per transmission, additional   *
*     satellite data sent in second or third message. Total # of messages *
*     being transmitted and the # of the message being transmitted is     *
*     indicated in the first two fields.                                  *
*                                                                         *
****************************************************************************/

tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSV( tSIRF_MSG_SSB_MEASURED_TRACKER *data, tSIRF_CHAR *buf )
{
   tSIRF_INT32 nMsgCnt, svidcnt;
   tSIRF_INT32 msgno, chnlno, smplno, fieldno;
   tSIRF_DOUBLE avgcno=0.0;
   tSIRF_CHAR * ptr;
   tSIRF_CHAR   str[64];

   /* Find how many messages do we need to generate, since each message */
   /* can only contain 4 satellites information*/
   svidcnt = 0;
   for (chnlno = 0; chnlno < data->chnl_cnt; chnlno++ )
      if ( data->chnl[chnlno].svid != 0 )
         svidcnt++;

   nMsgCnt = (svidcnt + 3) / 4;

   ptr = buf;
   chnlno = 0;

   for (msgno=1; msgno <= nMsgCnt; msgno++)
   {
      sprintf (ptr, "$GPGSV,%ld,%ld,%02ld",
               (long)nMsgCnt, /* total # of msgs */
               (long)msgno,       /* Msg #           */
               (long)svidcnt );

      fieldno = 0;

      while ( fieldno<4 && chnlno<MAX_NMEA_CHANNELS )
      {
         if ( chnlno < MAX_NMEA_CHANNELS )
         {
            avgcno = 0.0;
            for ( smplno=0; smplno<10; smplno++)
               avgcno += data->chnl[chnlno].cno[smplno];

            avgcno /= 10.0;

            if ( data->chnl[chnlno].svid != 0 )
            {
               snprintf(str, sizeof(str), ",%02d,%02d,%03d,%02d",
                  (data->chnl[chnlno].svid >= 120 ? data->chnl[chnlno].svid - 87 : data->chnl[chnlno].svid ),
                  (short) (data->chnl[chnlno].elevation * 0.5),
                  (short) (data->chnl[chnlno].azimuth * 1.5),
                  (short) avgcno );
               strcat( ptr, str );
               fieldno++;
            }

            chnlno++;
         }
      } /* while */

      AddCheckSum( ptr+1 );
      ptr += strlen(ptr);

   } /* for */

   return SIRF_SUCCESS;
}  /* SIRF_CODEC_NMEA_Encode_GSV */

/******************************************************************************/
/*                                                                            */
/* FUNCTION: SIRF_CODEC_NMEA_Import                                                  */
/*                                                                            */
/* DESCRIPTION: Reads NMEA messages and stores the data in an internal        */
/*              data structure which is also used for SiRF Binary messages    */
/*                                                                            */
/* PARAMETERS:                                                                */
/*  Name                    Mode                 Use                          */
/*   payload                 in             NMEA message                      */
/*   payload_length          in             Length of NMEA message            */
/*   message_id              out            Id of the corresponding           */
/*                                          SiRFBinary Message                */
/*   message_structure       out            Structure to store message content*/
/*   message_length          out            Length of the NMEA message        */
/*                                                                            */
/* RETURNS: Integer code to denote success or failure                         */
/******************************************************************************/                                                           
#ifdef GSW_CLM
tSIRF_RESULT SIRF_CODEC_NMEA_Import( tSIRF_UINT8* payload, tSIRF_INT32 payload_length,
                                    tSIRF_UINT32 *message_id, void *message_structure, tSIRF_UINT32 *message_length)
{
   tSIRF_UINT32 nmea_message_id;

   /* Get message ID */
   if( sscanf((const tSIRF_CHAR *)payload, "$PSRF%lu", &nmea_message_id) != 1)
   {
      *message_id = SIRF_MSG_NMEA_PASS_THRU;
      return SIRF_SUCCESS;
   }

   /* Translate Message IDs to GPS Message IDs defined in gps_messages.h */

   switch ( nmea_message_id )
   {
   case NMEA_EE_REQUEST_EPHEMERIS:
      *message_id = SIRF_MSG_SSB_EE_GPS_TIME_INFO;
      break;

   case NMEA_EE_INTEGRITY_WARNING:  
      *message_id = SIRF_MSG_SSB_EE_INTEGRITY;
      break;

   case NMEA_EE_MSG_ACK:
      *message_id = SIRF_MSG_SSB_EE_ACK;
      break;

   case NMEA_EE_CLK_BIAS_ADJUSTMENT:
      *message_id = SIRF_MSG_SSB_EE_CLK_BIAS_ADJ;
      break;

   case NMEA_EE_SUBFRAME_NAV_BITS_OUTPUT:
      *message_id = SIRF_MSG_SSB_50BPS_DATA;
      break;

   default: 
      *message_id = SIRF_MSG_NMEA_PASS_THRU;
      return SIRF_SUCCESS;         
   }

   *message_length = 0;

   switch ( *message_id )
   {
   case SIRF_MSG_SSB_EE_GPS_TIME_INFO:
      {
         tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO*)message_structure;
         tSIRF_INT32             time_valid_flag = 0;
         tSIRF_INT32             gps_week       = 0;
         *message_length         = sizeof(*msg);

         if ( sscanf((const tSIRF_CHAR *)payload, "$PSRF151,%ld,%ld,%lu,0x%08lX",
            &time_valid_flag,
            &gps_week,
            &msg->gps_tow,
            &msg->ephemeris_request_mask) == 4 )
         {
            msg->time_valid_flag = (tSIRF_UINT8)time_valid_flag;
            msg->gps_week = (tSIRF_UINT16)gps_week;
         }
         else
            return SIRF_FAILURE;
      }
      break;

   case SIRF_MSG_SSB_EE_INTEGRITY:
      {
         tSIRF_MSG_SSB_EE_INTEGRITY *msg = (tSIRF_MSG_SSB_EE_INTEGRITY*)message_structure;
         *message_length             = sizeof(*msg);

         if(sscanf((const tSIRF_CHAR *)payload, "$PSRF152,0x%08lX,0x%08lX,0x%08lX",
            &msg->sat_pos_validity_mask,
            &msg->sat_clk_validity_mask,
            &msg->sat_health_mask)!=3 )
            return SIRF_FAILURE;
      }
      break;

   case SIRF_MSG_SSB_EE_CLK_BIAS_ADJ:
      {
         tSIRF_UINT32 svid[3];
         tSIRF_UINT16 toe[3];
         tSIRF_INT32 clk_bias_adj[3];
         tSIRF_UINT16 sequence_number  = 0;
         tSIRF_UINT16 index            = 0;
         tSIRF_UINT16 i                = 0;

         tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *msg = (tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ*)message_structure;
         *message_length               = 3 * sizeof(*msg);

         if ( sscanf( (const tSIRF_CHAR *)payload, "$PSRF155,4,%hu,%u,%hu,%ld,%u,%hu,%ld,%u,%hu,%ld",
            &sequence_number,
            &svid[0],&toe[0],&clk_bias_adj[0],
            &svid[1],&toe[1],&clk_bias_adj[1],
            &svid[2],&toe[2],&clk_bias_adj[2] ) == 10 )
         {
            index = ( sequence_number - 1 )* 3 ;
            for ( i = index; i < index+3; i++ )
            {
               msg[i].svid         = (tSIRF_UINT8)svid[i - index];
               msg[i].toe          = toe[i-index];/* *(2e-4); */
               msg[i].clk_bias_adjust = clk_bias_adj[i-index];/**(2e+31);*/
            }                   
         }
         else
            return SIRF_FAILURE;             
      }
      break;

   case SIRF_MSG_SSB_EE_ACK:
      {
         tSIRF_MSG_SSB_EE_ACK   *msg    = (tSIRF_MSG_SSB_EE_ACK*)message_structure;
         int            nmeaid;
         *message_length      = sizeof(*msg);

         if ( sscanf((const tSIRF_CHAR *)payload, "$PSRF154,%d", &nmeaid) != 1 )
            return SIRF_FAILURE;

         switch ( nmeaid )
         {
         case NMEA_EE_PROVIDE_EPHEMERIS:
            { 
               msg->acq_id = SIRF_GET_MID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH);
               msg->acq_sub_id = SIRF_GET_SUB_ID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH); /* SIRFBinary ACK Sub-ID */
            }
            break;

         case NMEA_EE_PROVIDE_IONO_PARAMS:
            {
               msg->acq_id = SIRF_GET_MID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH);
               msg->acq_sub_id = SIRF_GET_SUB_ID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH);
            }
            break;

#ifndef CLM_STANDALONE
         case NMEA_EE_DEBUG_MSG:
            {
               msg->acq_id = SIRF_GET_MID(SIRF_MSG_SSB_EE_DEBUG);
               msg->acq_sub_id = SIRF_GET_SUB_ID(SIRF_MSG_SSB_EE_DEBUG); /* SIRFBinary ACK Sub-ID */
            }
            break;
#endif

         default:
            return SIRF_FAILURE;
         }/* End of nested switch */
      }
      break;

   case SIRF_MSG_SSB_50BPS_DATA:       /* NEW NMEA MSG ######*/
      {
         tSIRF_UINT32 chnl, svid;
         tSIRF_MSG_SSB_50BPS_DATA  *msg    = (tSIRF_MSG_SSB_50BPS_DATA*)message_structure;
         *message_length                   = sizeof(*msg);

         if ( sscanf((const tSIRF_CHAR *)payload, 
            "$PSRF140,%d,%d,%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x", 
            &chnl, &svid, &msg->word[0], &msg->word[1], &msg->word[2],
            &msg->word[3], &msg->word[4], &msg->word[5], &msg->word[6], &msg->word[7],
            &msg->word[8],&msg->word[9]) != 12 )
         {
            return SIRF_FAILURE;
         } 
         else
         {
            msg->chnl = (tSIRF_UINT8)chnl;
            msg->svid = (tSIRF_UINT8)svid;
         }
      }
      break;

   default:
      break;

   } /* switch */

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Import()*/


/******************************************************************************/
/*                                                                            */
/* FUNCTION: SIRF_CODEC_NMEA_Export                                                  */
/*                                                                            */
/* DESCRIPTION: Composes NMEA Messages from the data in an internal           */
/*              data structure                                                */
/*                                                                            */
/* PARAMETERS:                                                                */
/*    Name                  Mode                 Use                          */
/*   message_id              in            SiRF Binary message ID             */
/*   message_structure       in            Structure to store message content */
/*   message_length          in            Length of the message in the       */
/*                                         structure                          */
/*   packet                  out           Composed NMEA  Message             */
/*   packet_length           out           Length of the NMEA message         */
/*                                                                            */
/* RETURNS: Integer code to denote success or failure                         */
/******************************************************************************/   

tSIRF_RESULT SIRF_CODEC_NMEA_Export(
                                    tSIRF_UINT32 message_id, void *message_structure, tSIRF_UINT32 message_length,
                                    tSIRF_UINT8* packet, tSIRF_UINT32 *packet_length, tSIRF_UINT8 numSV_nmea )
{
   tSIRF_UINT32 nmea_message_id;

   /* Translate Message IDs to NMEA Message IDs defined in gps_proto_nmea.h */

   switch (message_id)
   {
   case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:
      {
         if ( numSV_nmea == 0 )
            nmea_message_id = NMEA_EE_PROVIDE_IONO_PARAMS;
         else
            nmea_message_id = NMEA_EE_PROVIDE_EPHEMERIS;
      }
      break;

   case SIRF_MSG_SSB_EE_ACK:
      nmea_message_id = NMEA_EE_DEBUG_MSG;
      break;

   case SIRF_MSG_SSB_SET_MSG_RATE:                 /* NEW NMEA MSG #####*/
      nmea_message_id = NMEA_EE_SET_MSG_RATE;
      break;   

   default: 
      nmea_message_id = SIRF_MSG_NMEA_PASS_THRU;
   }

   /* Message data:*/
   switch ( nmea_message_id ) 
   {
   case NMEA_EE_PROVIDE_IONO_PARAMS:
      {

         tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH * msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*)message_structure;

         if ( message_length!=sizeof(*msg) )
            return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;

         sprintf( (tSIRF_CHAR *)packet, "$PSRF108,%d,%d,%d,%d,%d,%d,%d,%d",
            (int)msg->extended_iono.alpha[0],
            (int)msg->extended_iono.alpha[1],
            (int)msg->extended_iono.alpha[2],
            (int)msg->extended_iono.alpha[3],
            (int)msg->extended_iono.beta[0],
            (int)msg->extended_iono.beta[1],
            (int)msg->extended_iono.beta[2],
            (int)msg->extended_iono.beta[3]);             

      }
      break;

   case NMEA_EE_PROVIDE_EPHEMERIS:
      {

         tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH * msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*)message_structure;

         if ( message_length != sizeof(*msg) )
            return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;

         if ( numSV_nmea == 1 )
         {
            /* Ephemeris data for only one satellite */

            sprintf( (tSIRF_CHAR *)packet, "$PSRF107,%d,%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
               "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d,"
               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
               (int)msg->week,(int)msg->extended_ephemeris[0].PRN,(int)msg->extended_ephemeris[0].ephemerisValidityFlag,(tSIRF_UINT32)msg->extended_ephemeris[0].URA,
               (tSIRF_UINT32)msg->extended_ephemeris[0].URA,(int)msg->extended_ephemeris[0].age,(int)msg->extended_ephemeris[0].Crs, (int)msg->extended_ephemeris[0].deltaN, 
               (int)msg->extended_ephemeris[0].M0,  (int)msg->extended_ephemeris[0].Cuc, (unsigned long)msg->extended_ephemeris[0].eccen, (int)msg->extended_ephemeris[0].Cus,(unsigned long)msg->extended_ephemeris[0].sqrtA,
               (tSIRF_UINT32)msg->extended_ephemeris[0].toe, (int)msg->extended_ephemeris[0].Cic, (int)msg->extended_ephemeris[0].omega0, (int)msg->extended_ephemeris[0].Cis,(int)msg->extended_ephemeris[0].i0,
               (int)msg->extended_ephemeris[0].Crc, (int)msg->extended_ephemeris[0].w, (int)msg->extended_ephemeris[0].omegaDot,(int)msg->extended_ephemeris[0].iDot, (tSIRF_UINT32)msg->extended_ephemeris[0].toc,(int)msg->extended_ephemeris[0].Tgd,
               (tSIRF_UINT32)msg->extended_ephemeris[0].af2, (int)msg->extended_ephemeris[0].af1, (int)msg->extended_ephemeris[0].af0 );

         }
         else if ( numSV_nmea == 2 )
         {

            sprintf( (tSIRF_CHAR *)packet, "$PSRF107,%d,%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
               "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d,"
               "%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
               "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d",
               (int)msg->week,(int)msg->extended_ephemeris[0].PRN,(int)msg->extended_ephemeris[0].ephemerisValidityFlag,(tSIRF_UINT32)msg->extended_ephemeris[0].URA,
               (tSIRF_UINT32)msg->extended_ephemeris[0].IODE,(int)msg->extended_ephemeris[0].age,(int)msg->extended_ephemeris[0].Crs, (int)msg->extended_ephemeris[0].deltaN, 
               (int)msg->extended_ephemeris[0].M0,  (int)msg->extended_ephemeris[0].Cuc, (unsigned long)msg->extended_ephemeris[0].eccen, (int)msg->extended_ephemeris[0].Cus,(unsigned long)msg->extended_ephemeris[0].sqrtA,
               (tSIRF_UINT32)msg->extended_ephemeris[0].toe, (int)msg->extended_ephemeris[0].Cic, (int)msg->extended_ephemeris[0].omega0, (int)msg->extended_ephemeris[0].Cis,(int)msg->extended_ephemeris[0].i0,
               (int)msg->extended_ephemeris[0].Crc, (int)msg->extended_ephemeris[0].w, (int)msg->extended_ephemeris[0].omegaDot,(int)msg->extended_ephemeris[0].iDot, (tSIRF_UINT32)msg->extended_ephemeris[0].toc,(int)msg->extended_ephemeris[0].Tgd,
               (tSIRF_UINT32)msg->extended_ephemeris[0].af2, (int)msg->extended_ephemeris[0].af1, (int)msg->extended_ephemeris[0].af0,
               (int)msg->extended_ephemeris[1].PRN,(int)msg->extended_ephemeris[1].ephemerisValidityFlag,(tSIRF_UINT32)msg->extended_ephemeris[1].URA,
               (tSIRF_UINT32)msg->extended_ephemeris[1].URA,(int)msg->extended_ephemeris[1].age,(int)msg->extended_ephemeris[1].Crs, (int)msg->extended_ephemeris[1].deltaN, 
               (int)msg->extended_ephemeris[1].M0,  (int)msg->extended_ephemeris[1].Cuc, (unsigned long)msg->extended_ephemeris[1].eccen, (int)msg->extended_ephemeris[1].Cus,(unsigned long)msg->extended_ephemeris[1].sqrtA,
               (tSIRF_UINT32)msg->extended_ephemeris[1].toe, (int)msg->extended_ephemeris[1].Cic, (int)msg->extended_ephemeris[1].omega0, (int)msg->extended_ephemeris[1].Cis,(int)msg->extended_ephemeris[1].i0,
               (int)msg->extended_ephemeris[1].Crc, (int)msg->extended_ephemeris[1].w, (int)msg->extended_ephemeris[1].omegaDot,(int)msg->extended_ephemeris[1].iDot, (tSIRF_UINT32)msg->extended_ephemeris[1].toc,(int)msg->extended_ephemeris[1].Tgd,
               (tSIRF_UINT32)msg->extended_ephemeris[1].af2, (int)msg->extended_ephemeris[1].af1, (int)msg->extended_ephemeris[1].af0 );


         }
         else
            return (SIRF_SUCCESS);

      }
      break;

#ifdef CLM_STANDALONE
   case NMEA_EE_DEBUG_MSG:
      {
         tSEA_DEBUG_MSG * msg = (tSEA_DEBUG_MSG *)message_structure;
         sprintf( (tSIRF_CHAR *)packet, "$PSRF110,0x%08lx",msg->debug_flags );
      }
      break; 
#endif
   case SIRF_MSG_NMEA_PASS_THRU:
      {
         strcpy( (char *)packet, (const char *)message_structure );
      }
      break;

   case NMEA_EE_SET_MSG_RATE:             /* NEW NMEA MSG #####*/
      {
         if ( message_length != sizeof(tSIRF_MSG_SSB_SET_MSG_RATE) )
            return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;

         sprintf( (tSIRF_CHAR *)packet, "$PSRF112,140,6,0" ); 
         /* PRINTF( "CLM : NMEA send out $PSRF112,140,6,0\n" );*/
         /* OLD : sprintf( (tSIRF_CHAR *)packet, "$PSRF112,%d,%d,%d",*/
         /* OLD :  (int)msg->msg_id, (int)msg->rate, (int)msg->mode);*/
      }
      break;

   default:
      return SIRF_CODEC_ERROR_INVALID_MSG_ID;

   } /*switch*/

   /* write tail: */

   if ( nmea_message_id != SIRF_MSG_NMEA_PASS_THRU )
      AddCheckSum(( tSIRF_CHAR*)packet + 1 );

   /* return length:*/
   *packet_length = (tSIRF_INT32)strlen( (const tSIRF_CHAR *)packet );

   return SIRF_SUCCESS;

} /*SIRF_CODEC_NMEA_Export()*/

#endif /* GSW_CLM */

#endif /* SIRF_CODEC_NMEA */

/**
 * @}
 */

