/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2007-2009 by SiRF Technology, Inc. All rights reserved.*
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
 * MODULE:  CODEC
 *
 * FILENAME:  sirf_codec_ascii.c
 *
 * DESCRIPTION: 
 *
 ***************************************************************************/

/***************************************************************************
 * Include Fields
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_msg_ssb.h"
#include "sirf_codec.h"
#include "sirf_pal_com.h"     /* Required for PAL COM Error codes */
#include "sirf_codec_ascii.h"

#ifdef __cplusplus
extern "C" {
#endif
size_t strlcat(
#if defined(TOOLCHAIN_VC8) && ( defined(OS_WINCE) || defined(OS_WIN32) || defined (OS_WIN32_MFC) )
         char        * __restrict dst,
         const char  * __restrict src,
#else
         char        *dst,
         const char  *src,
#endif
         size_t       size);
#ifdef __cplusplus
}
#endif

/***************************************************************************
 *   Macro Definitions
 ***************************************************************************/

/***************************************************************************
 * @brief:   Encode the error string for the specified error
 * @param:   pBuf - The buffer to write the data into
 * @param:   size - Maximum size of the buffer
 * @param:   pError - Pointer to the error structure
 * @return:  None
 ***************************************************************************/

static tSIRF_VOID PErrorFormat(tSIRF_CHAR *pBuf, tSIRF_UINT32 size, tSIRF_MSG_SSB_ERROR *pError)
{
   switch( pError->err_id )
   {
      case SIRF_MSG_SSB_ERRID_MI_BUFFERALLOCFAILURE:
         snprintf( pBuf, size, "UART buffer allocation error: %lu", (unsigned long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_UPDATETIMEFAILURE:
         /* "One second task not complete on entry #%ld (%ld tracker ms errors)";*/
         snprintf( pBuf, size, "OSOR #%ld (%ld ms)", (long)pError->param[0], (long)pError->param[1] ); 
         break;
      case SIRF_MSG_SSB_ERRID_MI_VCOCLOCKLOST:
         snprintf( pBuf, size, "VCO lock lost in %lu msec", (unsigned long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_MEMORYTESTFAILED:
         snprintf( pBuf, size, "Memory test failed at 0x%lx", (unsigned long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_FALSEACQRECEIVERRESET:
         snprintf( pBuf, size, "Assuming false acquisition and reseting receiver at %ld", (long)pError->param[0] ); 
         break;
      case SIRF_MSG_SSB_ERRID_KFC_KILLCHANNEL:
         snprintf( pBuf, size, "Kalman Filter @%ld: Kill chan %02ld, range res=%ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_VELOCITY:
         snprintf( pBuf, size, "SRAM backup not done: Velocity sum exceeds maximum." );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_NUMSV:
         snprintf( pBuf, size, "SRAM backup not done: Too few SVs used in a solution." );
         break;
      case SIRF_MSG_SSB_ERRID_KFS_BADALTITUDE:
         snprintf( pBuf, size, "KFDBG @%ld: Bad altitude = %ld", (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_KRS_BADSOLUTION:
         snprintf( pBuf, size, "Krause solution poor:\n POS = %ld %ld %ld\n PR = %ld %ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2], (long)pError->param[3], (long)pError->param[4] );
         break;
      case SIRF_MSG_SSB_ERRID_RMC_GETTINGPOSITION:
         snprintf( pBuf, size, "RxM error in getting SV %02ld's position", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TIMEEXCEEDED:
         snprintf( pBuf, size, "RxM: Pseudo range in seconds exceeds limit: %ld", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TDOPOVERFLOW:
         snprintf( pBuf, size, "RxM error: tdop=%ld overflow", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_VALIDDURATIONEXCEEDED:
         snprintf( pBuf, size, "RxM: Ephemeris for SV %02ld exceeds valid time duration by %ld sec",
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_SRAMCKSUM:
         snprintf( pBuf, size, "SRAM control flags contain bad checksum" );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_RTCTIMEINVALID:
         snprintf( pBuf, size, "RTC time or SRAM backup invalid, TOW:%ld, WNUM:%ld",
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_BADPOSTION:
         snprintf( pBuf, size, "SRAM: Bad position during startup, x:%ld y:%ld z:%ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_CS_SVPARITY:
         snprintf( pBuf, size, "Bad parity from SV %02ld, subframe %ld", (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_PAL_COM_ERROR:
         snprintf( pBuf, size, "Error in serial communication with tracker; type:%ld, param:%ld", 
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_PAL_COM_DATA_NOT_AVAILABLE:
         snprintf( pBuf, size, "Error: tracker data not available; state:%ld, time:%ld", 
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      default:
         snprintf( pBuf, size, "Error in unknown format: err_id=%lx params: %lx %lx %lx %lx", 
                   (unsigned long)pError->param[0],
		   (unsigned long)pError->param[1],
		   (unsigned long)pError->param[2],
		   (unsigned long)pError->param[3],
		   (unsigned long)pError->param[4] );
         break;
   }
}

/***************************************************************************
 * @brief:   Convert the specified SSB message structure into a text stream
 * @param[in]:     message_id - id of message to translate
 * @param[in]:     message_structure - data structure for that message
 * @param[in]:     message_length - pseudo-length of the data structure
 * @param[out]:    lpszText - output buffer to hold the translation
 * @param[in out]: lpszText_length - Input- size of lpszText buffer
                                     Output- resultant length of the output string
 * @return:   SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_PARAMETER, or
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH
 ***************************************************************************/
tSIRF_RESULT SIRF_CODEC_ASCII_Encode( tSIRF_UINT32 message_id,
                                      tSIRF_VOID *message_structure,
                                      tSIRF_UINT32 message_length,
                                      tSIRF_CHAR *lpszText,
                                      tSIRF_UINT32 *lpszText_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   if ( ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_CHAR *) NULL != lpszText) &&
        ((tSIRF_UINT32 *) NULL != lpszText_length) )
   {
      tSIRF_UINT32 i, j;
      tSIRF_CHAR szFoo[150];
      tSIRF_CHAR szBuf[2 * SIRF_MSG_SSB_MAX_MESSAGE_LEN];

      switch ( message_id )
      {
         case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 (2) */
         {
            tSIRF_MSG_SSB_MEASURED_NAVIGATION *NavData = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*)message_structure;
            snprintf((char*)szBuf, sizeof(szBuf), "2,%ld,%ld,%ld,%.3f,%.3f,%.3f,%u,%.1f,%u,%u,%lu,%u",
                     (long)NavData->ecef_x,
                     (long)NavData->ecef_y,
                     (long)NavData->ecef_z,
                     NavData->ecef_vel_x * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_y * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_z * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->nav_mode,
                     NavData->hdop * SIRF_MSG_SSB_DOP_LSB,
                     NavData->nav_mode2,
                     NavData->gps_week,
                     (unsigned long)NavData->gps_tow,
                     NavData->sv_used_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (int)NavData->sv_used[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_MEASURED_TRACKER:  /* 0x04 (4) */
         {
            tSIRF_MSG_SSB_MEASURED_TRACKER *TrkData = (tSIRF_MSG_SSB_MEASURED_TRACKER*)message_structure;
            snprintf( (char*)szBuf, sizeof(szBuf), "4,%d,%lu,%u",
                     TrkData->gps_week,
                     (unsigned long)TrkData->gps_tow,
                     TrkData->chnl_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( (char*)szFoo,  sizeof(szFoo),",%u,%u,%u,%u",
                        TrkData->chnl[i].svid,
                        (int) (TrkData->chnl[i].azimuth * SIRF_MSG_SSB_AZIMUTH_LSB),
                        (int) (TrkData->chnl[i].elevation * SIRF_MSG_SSB_ELEVATION_LSB),
                        TrkData->chnl[i].state );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
               for( j = 0; j < SIRF_NUM_POINTS; j++ )
               {
                  snprintf( szFoo,  sizeof(szFoo), ",%u", TrkData->chnl[i].cno[j] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 (5) */
         {
            tSIRF_MSG_SSB_RAW_TRACKER *RawTrk = (tSIRF_MSG_SSB_RAW_TRACKER*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "5,%ld,%d,%02x,%ld,%d,%d,%ld,%ld,%ld,%ld,%d",
                     (long)RawTrk->channel,
                     RawTrk->svid,
                     RawTrk->state,
                     (long)RawTrk->bit_number,
                     RawTrk->msec_number,
                     RawTrk->chip_number,
                     (long)RawTrk->code_phase,
                     (long)RawTrk->carrier_doppler,
                     (long)RawTrk->measure_timetag,
                     (long)RawTrk->delta_carrier_phase,
                     RawTrk->search_cnt );
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", RawTrk->cno[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%d",
                     RawTrk->power_bad_count,
                     RawTrk->phase_bad_count,
                     RawTrk->delta_car_interval,
                     RawTrk->correl_interval );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_SW_VERSION: /* 0x06 (6) */
         {
            tSIRF_MSG_SSB_SW_VERSION *SwVersion = (tSIRF_MSG_SSB_SW_VERSION*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "SW Version: %s", SwVersion->sw_version );
            break;
         }

         case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 (7) */
         {
            tSIRF_MSG_SSB_CLOCK_STATUS *ClkStatus = (tSIRF_MSG_SSB_CLOCK_STATUS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "Week:%u  TOW:%lu  EstGPSTime:%lu ms  SVCnt:%u  "
                               "Clock Drift:%lu Hz  Clock Bias:%lu ns",
                     ClkStatus->gps_week,
                     (unsigned long)ClkStatus->gps_tow,
                     (unsigned long)ClkStatus->est_gps_time,
                     ClkStatus->sv_used_cnt,
                     (unsigned long)ClkStatus->clk_offset,
                     (unsigned long)ClkStatus->clk_bias );
            break;
         }

         case SIRF_MSG_SSB_50BPS_DATA:          /* 0x08  (8) */
         case SIRF_MSG_SSB_50BPS_DATA_VERIFIED: /* 0x52 (82) */
         {
            tSIRF_MSG_SSB_50BPS_DATA *data50bps = (tSIRF_MSG_SSB_50BPS_DATA*)message_structure;
            
            if ( message_id == SIRF_MSG_SSB_50BPS_DATA_VERIFIED )
               snprintf( szBuf, sizeof(szBuf), "82,%02u %02u ",
                               data50bps->chnl, data50bps->svid );
            else
               snprintf( szBuf, sizeof(szBuf), ": %02u %02u ",
                               data50bps->chnl, data50bps->svid );

            for( i = 0; i < 10; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%08lx ", (unsigned long)data50bps->word[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 (9) */
         {
            tSIRF_MSG_SSB_THROUGHPUT *Throughput = (tSIRF_MSG_SSB_THROUGHPUT*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "ThrPut(186=1ms): Latency:%03u SegStatMax:%03u "
                               "AveTrkTime:%03u Nav Complete:%04u ms",
                     Throughput->seg_stat_lat,
                     Throughput->seg_stat_max,
                     Throughput->avg_trk_time,
                     Throughput->last_ms );
            break;
         }

         case SIRF_MSG_SSB_ERROR: /* 0x0A (10) */
         {
            tSIRF_MSG_SSB_ERROR *pError = (tSIRF_MSG_SSB_ERROR *)message_structure;
            PErrorFormat( (tSIRF_CHAR *)szBuf, sizeof(szBuf), pError );
            break;
         }

         case SIRF_MSG_SSB_ACK: /* 0x0B (11) */
         {
            snprintf( szBuf, sizeof(szBuf), "Ack: 0x%02X 0x%02X", ((tSIRF_MSG_SSB_ACK*)message_structure)->msg_id,
                     ((tSIRF_MSG_SSB_ACK*)message_structure)->sub_id );
            break;
         }

         case SIRF_MSG_SSB_NAK: /* 0x0C (12) */
         {
            snprintf( szBuf, sizeof(szBuf), "Nak: 0x%02X 0x%02X", ((tSIRF_MSG_SSB_NAK*)message_structure)->msg_id,
                     ((tSIRF_MSG_SSB_NAK*)message_structure)->sub_id );
            break;
         }

         case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D (13) */
         {
            tSIRF_MSG_SSB_VISIBILITY_LIST *VisList = (tSIRF_MSG_SSB_VISIBILITY_LIST*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "Visible List\n" );
            for( i = 0; i < VisList->svid_cnt; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "SVID:%02u  Az:%03d  El:%03d\n",
                               VisList->visible[i].svid,
                               VisList->visible[i].azimuth,
                               VisList->visible[i].elevation );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_ALMANAC: /* 0x0E (14) */
         {
            tSIRF_MSG_SSB_ALMANAC * Alm = (tSIRF_MSG_SSB_ALMANAC*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "ALMANAC SVID:%02u  ", Alm->svid );
            for( i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%d, ", Alm->data[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F (15) */
         {
            tSIRF_MSG_SSB_EPHEMERIS * Eph = (tSIRF_MSG_SSB_EPHEMERIS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "EPHEMERIS SVID:%02u  ", Eph->svid );
            for( i = 0; i < 3; i++ )
            {
               for ( j = 0; j < 15; j++ )
               {
                  snprintf( szFoo, sizeof(szFoo), "%u, ", Eph->subframe[i][j] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 (18) */
         {
            snprintf( szBuf, sizeof(szBuf), "OkToSend: %s",
               ((tSIRF_MSG_SSB_OK_TO_SEND*)message_structure)->input_enabled ? "1" : "0" );
            break;
         }

         case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 (19) */
         {
            tSIRF_MSG_SSB_RECEIVER_PARAMS * RcvrParam = (tSIRF_MSG_SSB_RECEIVER_PARAMS*)message_structure;
            const char* strAltMode []={"auto","always","never use"};
            const char* strAltSrc  []={"last KF alt","fixed","dynamic"};
            const char* strDegMode []={"DThenT","TThenD","DirOnly","TimeOnly","Disabled"};
            const char* strDGPSMode[]={"auto","exclusive","never use","mixed"};
            const char* strDOPMode []={"auto","PDOP","HDOP","GDOP","disabled"};
            const char* strEnabled []={"disabled","enabled"};
            const char* strDGPSSrc []={"None","WAAS","Serial","Internal Beacon","Software"};
            float lp_duty_cycle;
            char  lp_str[300] = "";
            char  tmpStr[100] = "";

            if( (!RcvrParam->lp_on_time) || (!RcvrParam->lp_interval) )
            {
               lp_duty_cycle = 0.0;
            }
            else
            {
               lp_duty_cycle = (float) (100 * RcvrParam->lp_on_time)
                            / (float) (RcvrParam->lp_interval);
            }
            if( RcvrParam->lp_push_to_fix )
            {
               strlcat( lp_str, "Push-To-Fix enabled\n", sizeof(lp_str) );
            }
            else if( !RcvrParam->lp_pwr_cycling_enabled )
            {
               strlcat( lp_str, "Continuous power enabled\n", sizeof(lp_str) );
            }
            else
            {
               snprintf( tmpStr, sizeof(tmpStr), "TricklePower enabled with %ld ms on, %f%% duty cycle\n",
                        (long)RcvrParam->lp_on_time, lp_duty_cycle );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            if( RcvrParam->lp_user_tasks_enabled )
            {
               snprintf( tmpStr, sizeof(tmpStr), "User tasks enabled, period = %ld\n",
                        (long)RcvrParam->lp_user_task_interval );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            else
            {
               strlcat( lp_str, "User tasks disabled\n", sizeof(lp_str) );
            }
            snprintf( tmpStr, sizeof(tmpStr), "MaxAcqTime = %lu ms; MaxOffTime = %lu ms\n",
                     (unsigned long)RcvrParam->lp_max_acq_time, (unsigned long)RcvrParam->lp_max_off_time );
            strlcat( lp_str, tmpStr, sizeof(lp_str) );
            snprintf( szBuf, sizeof(szBuf), "Polling Navigation Parameters\n"
                     "AltMode: %s\nAltSource: %s\nAltitude: %d\n"
                     "DegradedMode: %s\nDegradedTimeout: %u s\nDRTimeout: %u s\n"
                     "TrkSmoothMode: %s\nStaticNav: %s\n3SV LSQ: %s\n"
                     "DOPMaskMode: %s\nElevMask:%.1f deg\nPwrMask: %u dBHz\n"
                     "DGPSSrc: %s\nDGPSMode: %s\nDGPSTimeout: %u s\n"
                     "%s",
                     strAltMode[RcvrParam->alt_mode],
                     strAltSrc[RcvrParam->alt_src],
                     RcvrParam->alt_input,
                     strDegMode[RcvrParam->degraded_mode],
                     RcvrParam->degraded_timeout,
                     RcvrParam->dr_timeout,
                     strEnabled[RcvrParam->trk_smooth],
                     strEnabled[RcvrParam->static_nav_mode],
                     strEnabled[RcvrParam->enable_3sv_lsq],
                     strDOPMode[RcvrParam->dop_mask_mode],
                     RcvrParam->nav_elev_mask / 10.0F,
                     RcvrParam->nav_pwr_mask,
                     strDGPSSrc[RcvrParam->dgps_src],
                     strDGPSMode[RcvrParam->dgps_mode],
                     RcvrParam->dgps_timeout,
                     lp_str );
            break;
         }
   
         case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 (20) */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*)message_structure;
   
            if ( message_length != sizeof(*msg) )
            {
               return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
   
            snprintf( szBuf, sizeof(szBuf), "20,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%ld,%d,%ld,%ld,%ld,%ld,%ld",
                                 msg->svid,
                                 msg->period,
                                 msg->bit_synch_time,
                                 msg->bit_count,
                                 msg->poor_status_count,
                                 msg->good_status_count,
                                 msg->parity_error_count,
                                 msg->lost_vco_count,
                                 msg->frame_synch_time,
                                 msg->cno_mean,
                                 msg->cno_sigma,
                                 msg->clock_drift,
                                 (long) msg->clock_offset_mean,
                                 msg->bad_1khz_bit_count,
                                 (long) msg->abs_i20ms,
                                 (long) msg->abs_q1ms,
                                 (long) msg->reserved[0],
                                 (long) msg->reserved[1],
                                 (long)msg->reserved[2] );
         }
         break;
   
         case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B (27) */
         {
            tSIRF_MSG_SSB_DGPS_STATUS *msg = (tSIRF_MSG_SSB_DGPS_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "27,%u", msg->src );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", msg->cor_age[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            snprintf( szFoo, sizeof(szFoo), ",%u,%u", msg->reserved[0], msg->reserved[1] );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u,%d", msg->corrections[i].prn, msg->corrections[i].cor );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            break;
         }

         case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C (28) */
         {
            tSIRF_MSG_SSB_NL_MEAS_DATA *NlMeas = (tSIRF_MSG_SSB_NL_MEAS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "28,%u,%lu,%u,%.10e,%.10e,%.10e,%.10e,%u,%u",
                     NlMeas->Chnl,
                     (unsigned long)NlMeas->Timetag,
                     NlMeas->svid,
                     NlMeas->gps_sw_time,
                     NlMeas->pseudorange,
                     NlMeas->carrier_freq,
                     NlMeas->carrier_phase,
                     NlMeas->time_in_track,
                     NlMeas->sync_flags);
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", NlMeas->cton[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%u,%u",
                     NlMeas->delta_range_interval,
                     NlMeas->mean_delta_range_time,
                     NlMeas->extrapolation_time,
                     NlMeas->phase_error_count,
                     NlMeas->low_power_count );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D (29) */
         {
            tSIRF_MSG_SSB_NL_DGPS_DATA *NlDgps = (tSIRF_MSG_SSB_NL_DGPS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "29,%d,%d,%u,%.10e,%.10e,%.10e,%.10e,%.10e",
                     NlDgps->svid,
                     NlDgps->iod,
                     NlDgps->source,
                     NlDgps->pr_correction,
                     NlDgps->prr_correction,
                     NlDgps->correction_age,
                     NlDgps->prc_variance,
                     NlDgps->prrc_variance );
            break;
         }

         case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E (30) */
         {
            tSIRF_MSG_SSB_NL_SV_STATE_DATA *NlSvState = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "30,%u,%.11e",
                     NlSvState->svid,
                     NlSvState->time );
            for( i = 0; i < 3; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%.10e", NlSvState->pos[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            for( i = 0; i < 3; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%.10e", NlSvState->vel[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%.10e,%u,%.10e,%.10e,%.10e",
                     NlSvState->clk,
                     NlSvState->clf,
                     NlSvState->eph,
                     NlSvState->posvar,
                     NlSvState->clkvar,
                     NlSvState->iono );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F (31) */
         {
            tSIRF_MSG_SSB_NL_INIT_DATA * NlInit = (tSIRF_MSG_SSB_NL_INIT_DATA*)message_structure;

            /* From NL_ControlBlock */
            snprintf( szBuf, sizeof(szBuf), "31,%u,%u,%u,%.10e,%u,%d,%d,%d,%u,%u,%d,%d,%d,%u,%d,%d"
                                 ",%d,%u,%d,%u,%d,%u,%d",
                     NlInit->enable_con_alt_mode, NlInit->alt_mode, NlInit->alt_source,
                     NlInit->altitude, NlInit->degraded_mode, NlInit->degraded_timeout,
                     NlInit->dr_timeout, NlInit->coast_timeout, NlInit->tracksmooth_mode,
                     NlInit->dop_selection, NlInit->hdop_thresh, NlInit->gdop_thresh,
                     NlInit->pdop_thresh, NlInit->dgps_selection, NlInit->dgps_timeout,
                     NlInit->elev_nav_mask, NlInit->pow_nav_mask, NlInit->editing_residual_mode,
                     NlInit->editing_residual_threshold, NlInit->ssd_mode, NlInit->ssd_threshold,
                     NlInit->static_nav_mode, NlInit->static_nav_threshold );

            /* From NL_PositionInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%.10e,%.10e,%u",
                     NlInit->ecef_x, NlInit->ecef_y, NlInit->ecef_z, NlInit->position_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* From NL_TimeInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%d,%u",
                     NlInit->gps_time, NlInit->gps_week, NlInit->time_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* from NL_ClockDriftInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%u",
                     NlInit->clk_offset, NlInit->clk_offset_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_INIT_DATA: /* 0x40 0x01 */
         {
            tSIRF_MSG_SSB_NL_AUX_INIT_DATA * NlAuxInit = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,1,%lu,%u,%lu,%u,%u,%u,%u,%u,%lu,%lu,%lu,%lu",
                      (unsigned long)NlAuxInit->time_init_unc,NlAuxInit->saved_pos_week,(unsigned long)NlAuxInit->saved_pos_tow,
                      NlAuxInit->saved_pos_ehe,NlAuxInit->saved_pos_eve,NlAuxInit->sw_version,NlAuxInit->icd_version,
                      NlAuxInit->chip_version,(unsigned long)NlAuxInit->acq_clk_speed,(unsigned long)NlAuxInit->default_clock_offset,
                      (unsigned long)NlAuxInit->tracker_status,(unsigned long)NlAuxInit->reserved);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_MEAS_DATA: /* 0x40 0x02 */
         {
            tSIRF_MSG_SSB_NL_AUX_MEAS_DATA * NlAuxMeas = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA*)message_structure;  
            snprintf( szBuf, sizeof(szBuf), "64,2,%u,%u,%u,%u,%lu,%lu,%ld,%ld,%d,%d,%ld,%ld,%ld,%ld,"
                            "%d,%d,%d,%d,%u,%u,%ld,%d,%d,%u,%lu",                                  
                      NlAuxMeas->sv_prn, NlAuxMeas->status, NlAuxMeas->extended_status,NlAuxMeas->bit_sync_qual,
                      (unsigned long)NlAuxMeas->time_tag,(unsigned long)NlAuxMeas->code_phase,(long)NlAuxMeas->carrier_phase,(long)NlAuxMeas->carrier_freq,
                      NlAuxMeas->carrier_accel,NlAuxMeas->ms_num,(long)NlAuxMeas->bit_num,(long)NlAuxMeas->code_correction,
                      (long)NlAuxMeas->smooth_code,(long)NlAuxMeas->code_offset,NlAuxMeas->pseudorange_noise,NlAuxMeas->delta_range_qual,
                      NlAuxMeas->phase_lock_qual,NlAuxMeas->ms_uncertainty,NlAuxMeas->sum_abs_I,NlAuxMeas->sum_abs_Q,
                      (long)NlAuxMeas->sv_bit_num,NlAuxMeas->mp_los_det_value,NlAuxMeas->mp_only_det_value,
                      NlAuxMeas->recovery_status,(unsigned long)NlAuxMeas->sw_time_uncertainty);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_AID_DATA: /* 0x40 0x03 */
         {
            tSIRF_MSG_SSB_NL_AUX_AID_DATA * NlAuxAid = (tSIRF_MSG_SSB_NL_AUX_AID_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,3,%ld,%ld,%ld,%lu,%u,%lu",
                            (long)NlAuxAid->ecef_x,(long)NlAuxAid->ecef_y,(long)NlAuxAid->ecef_z,(unsigned long)NlAuxAid->horiz_pos_unc,
                            NlAuxAid->alt_unc,(unsigned long)NlAuxAid->sw_tow);
            break;
         }

         case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 (41) */
         {
            tSIRF_MSG_SSB_GEODETIC_NAVIGATION *GeodNavState = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "41,%u,%u,%u,%lu,%u,%u,%u,%u,%u,%u,%lu,%ld,%ld,%ld,%ld,%u,%u,%u,"
                               "%d,%d,%d,%lu,%lu,%lu,%u,%ld,%lu,%ld,%lu,%lu,%u,%u,%u,%u,%u",
                     GeodNavState->nav_valid, GeodNavState->nav_mode,
                     GeodNavState->gps_week, (unsigned long)GeodNavState->gps_tow, GeodNavState->utc_year,
                     GeodNavState->utc_month, GeodNavState->utc_day, GeodNavState->utc_hour,
                     GeodNavState->utc_min, GeodNavState->utc_sec, (unsigned long)GeodNavState->sv_used,
                     (long)GeodNavState->lat, (long)GeodNavState->lon, (long)GeodNavState->alt_ellips,
                     (long)GeodNavState->alt_msl, GeodNavState->datum, GeodNavState->sog,
                     GeodNavState->hdg, GeodNavState->mag_var, GeodNavState->climb_rate,
                     GeodNavState->heading_rate, (unsigned long)GeodNavState->ehpe, (unsigned long)GeodNavState->evpe,
                     (unsigned long)GeodNavState->ete,GeodNavState->ehve, (long)GeodNavState->clk_bias,
                     (unsigned long)GeodNavState->clk_bias_error, (long)GeodNavState->clk_offset,
                     (unsigned long)GeodNavState->clk_offset_error, (unsigned long)GeodNavState->distance_travelled,
                     GeodNavState->distance_travelled_error, GeodNavState->heading_error, GeodNavState->sv_used_cnt,
                     GeodNavState->hdop, GeodNavState->additional_mode_info );
            break;
         }

         case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D (45) */
         {
            if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA *ADCData10Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45,%lu,%d,%d,%u,%u",
                  (unsigned long)ADCData10Hz->current_time,
                  ADCData10Hz->adc2_avg,
                  ADCData10Hz->adc3_avg,
                  ADCData10Hz->odo_count,
                  ADCData10Hz->gpio_stat);
            }
            else if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ *ADCData1Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45" );
               for ( i=0; i<10; i++ )
               {
                  snprintf(szFoo, sizeof(szFoo), ",%lu,%d,%d,%u,%u",
                     (unsigned long)ADCData1Hz->dataset[i].current_time,
                     ADCData1Hz->dataset[i].adc2_avg,
                     ADCData1Hz->dataset[i].adc3_avg,
                     ADCData1Hz->dataset[i].odo_count,
                     ADCData1Hz->dataset[i].gpio_stat);
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E (46) */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3*)message_structure;

            if ( message_length != sizeof(*msg) )
            {
               return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }

            snprintf( szBuf, sizeof(szBuf), "46,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%ld,%d,%ld,%ld,%ld,%u,%u,%u,%u,%u",
                                 msg->svid,
                                 msg->period,
                                 msg->bit_synch_time,
                                 msg->bit_count,
                                 msg->poor_status_count,
                                 msg->good_status_count,
                                 msg->parity_error_count,
                                 msg->lost_vco_count,
                                 msg->frame_synch_time,
                                 msg->cno_mean,
                                 msg->cno_sigma,
                                 msg->clock_drift,
                                 (long)msg->clock_offset,
                                 msg->bad_1khz_bit_count,
                                 (long)msg->abs_i20ms,
                                 (long)msg->abs_q20ms,
                                 (long)msg->phase_lock,
                                 msg->rtc_frequency,
                                 msg->e_to_acq_ratio,
                                 msg->t_sync_agc_gain,
                                 msg->tm_5_ready,
                                 msg->ClkDriftUnc );
         }
         break;

         case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x0130 (48, 1) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,1,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",

                     msg->nav,
                     msg->data,
                     msg->cal_gb_cal,
                     msg->gsf_cal_ssf_cal,
                     msg->nav_across_reset_pos,
                     msg->hd,
                     msg->gyr_sub_op_spd_sub_op,
                     msg->nav_st_int_ran_z_gb_cal_upd,
                     msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st,
                     msg->gps_upd_pos,
                     msg->gps_upd_hd,
                     msg->gps_pos_gps_vel,
                     msg->dws_hd_rt_s_f_cal_valid,
                     msg->dws_hd_rt_s_f_cal_upd,
                     msg->dws_spd_s_f_cal_valid,
                     msg->dws_spd_s_f_cal_upd );
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x0230 (48, 2) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,2,%u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%lu,%u,%u,%u,%u, %u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%u,%d,%u,%d,%u,%u,%d,%u",
                     msg->spd,
                     msg->spd_e,
                     msg->ssf,
                     msg->ssf_e,
                     msg->hd_rte,
                     msg->hd_rte_e,
                     msg->gb,
                     msg->gbE,
                     msg->gsf,
                     msg->gsf_e,
                     (unsigned long)msg->tpe,
                     msg->the,
                     msg->nav_ctrl,
                     msg->reverse,
                     msg->hd,
                     msg->sensor_pkg,
                     msg->odo_spd,
                     msg->odo_spd_s_f,
                     msg->odo_spd_s_f_err,
                     msg->lf_wheel_spd_sf,
                     msg->lf_wheel_spd_sf_err,
                     msg->rf_wheel_spd_sf,
                     msg->rf_wheel_spd_sf_err,
                     msg->lr_wheel_spd_sf,
                     msg->lr_wheel_spd_sf_err,
                     msg->rr_wheel_spd_sf,
                     msg->rr_wheel_spd_sf_err,
                     msg->rear_axle_spd_delta,
                     msg->rear_axle_avg_spd,
                     msg->rear_axle_spd_err,
                     msg->rear_axle_hd_rt,
                     msg->rear_axle_hd_rt_err,
                     msg->front_axle_spd_delta,
                     msg->front_axle_avg_spd,
                     msg->front_axle_spd_err,
                     msg->front_axle_hd_rt,
                     msg->front_axle_hd_rt_err);
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x0330 (48, 3) */
         {
            tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,3,%d,%u,%u,%u,%u,%u,%lu,%d,%u,%u,%u,%u,%u,%lu,%u,%u",
                     msg->gps_hd_rte,
                     msg->gps_hd_rte_e,
                     msg->gps_hd,
                     msg->gps_hd_e,
                     msg->gps_spd,
                     msg->gps_spd_e,
                     (unsigned long)msg->gps_pos_e,
                     msg->dr_hd_rte,
                     msg->dr_hd_rte_e,
                     msg->dr_hd,
                     msg->dr_hd_e,
                     msg->dr_spd,
                     msg->dr_spd_e,
                     (unsigned long)msg->dr_pos_e,
                     msg->reserved[0],
                     msg->reserved[1]);
            break;
         }

         case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x00730 (48, 7) */
         {
            tSIRF_MSG_SSB_DR_SENS_PARAM *msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,7,%u,%u,%u,",
                     msg->base_ssf,
                     msg->base_gb,
                     msg->base_gsf );
            break;
         }

         case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x0830 (48, 8) */
         {
            tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,8,%u,%u,%u,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d",
              msg->meas_type,  msg->valid_cnt,  msg->bkup_flgs,
              (unsigned long)msg->blk[0].tag, msg->blk[0].spd, msg->blk[0].hd_rte,
              (unsigned long)msg->blk[1].tag, msg->blk[1].spd, msg->blk[1].hd_rte,
              (unsigned long)msg->blk[2].tag, msg->blk[2].spd, msg->blk[2].hd_rte,
              (unsigned long)msg->blk[3].tag, msg->blk[3].spd, msg->blk[3].hd_rte,
              (unsigned long)msg->blk[4].tag, msg->blk[4].spd, msg->blk[4].hd_rte,
              (unsigned long)msg->blk[5].tag, msg->blk[5].spd, msg->blk[5].hd_rte,
              (unsigned long)msg->blk[6].tag, msg->blk[6].spd, msg->blk[6].hd_rte,
              (unsigned long)msg->blk[7].tag, msg->blk[7].spd, msg->blk[7].hd_rte,
              (unsigned long)msg->blk[8].tag, msg->blk[8].spd, msg->blk[8].hd_rte,
              (unsigned long)msg->blk[9].tag, msg->blk[9].spd, msg->blk[9].hd_rte );
            break;
         }

         case SIRF_MSG_SSB_MMF_STATUS: /* 0x5030 (48, 80) */
         {
            tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "48,80,%lu,%u,%ld,%ld,%ld,%lu,%u,%ld,%ld,%ld,%lu",
                  (unsigned long)msg->status,
                  msg->sys_hdg,
                  (long)msg->sys_lat,
                  (long)msg->sys_lon,
                  (long)msg->sys_alt,
                  (unsigned long)msg->sys_tow,
                  msg->mmf_hdg,
                  (long)msg->mmf_lat,
                  (long)msg->mmf_lon,
                  (long)msg->mmf_alt,
                  (unsigned long)msg->mmf_tow );
            break;
         }

         case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 (50) */
         {
            tSIRF_MSG_SSB_SBAS_PARAM *msg = (tSIRF_MSG_SSB_SBAS_PARAM*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "50,%u,%u,%u,%u",
                  msg->prn,
                  msg->mode,
                  msg->timeout,
                  msg->flg_bits );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x0133 (51, 1) */
         {
            tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,1,%u",
                  msg->reserved);
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x0433 (51, 4) */
         {
            tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,4,%lu,%u,%lu,%u,%u,%u,%u,%u,%u,%u,%lu",
                  (unsigned long)msg->tracker_time_tag,
                  msg->gps_week,
                  (unsigned long)msg->gps_tow,
                  msg->tracker_rtc_day,
                  msg->tracker_rtc_hour,
                  msg->tracker_rtc_min,
                  msg->tracker_rtc_sec,
                  msg->tracker_rtc_clkctr,
                  msg->tracker_rtc_ok,
                  msg->tracker_rtc_rollover_complete,
                  (unsigned long)msg->tracker_rtc_rollover_seconds );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
         {
            tSIRF_MSG_SSB_SIRFNAV_START *msg = (tSIRF_MSG_SSB_SIRFNAV_START*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "51,7,%lu,%lu,%lu,%lu",
                  (unsigned long)msg->start_mode,
                  (unsigned long)msg->clock_offset,
                  (unsigned long)msg->port_num,
                  (unsigned long)msg->baud_rate );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (161, 8) */
         {
            tSIRF_MSG_SSB_SIRFNAV_STOP *msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,8,%lu",
                  (unsigned long)msg->stop_mode );
            break;
         }

         case SIRF_MSG_SSB_STARTUP_INFO: /* 0x0136 (54, 1) */
         {
            tSIRF_MSG_SSB_STARTUP_INFO *msg = (tSIRF_MSG_SSB_STARTUP_INFO*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "54,1,%u,%u,%u,"
                               "%lu,%u,%lu,%u,%u,%u,%u,%u,%u,"
                               "%ld,%ld,%ld,%u,"
                               "%lu,%lu,%u,%u,%ld,%lu",
              msg->mode,  msg->status,  msg->dr_status,
              (unsigned long)msg->gps_off_time, msg->gps_week, (unsigned long)msg->gps_tow, msg->utc_year, msg->utc_month, msg->utc_day,   msg->utc_hour, msg->utc_min, msg->utc_sec,
              (long)msg->lat, (long)msg->lon, (long)msg->alt_ellips, msg->heading,
              (unsigned long)msg->valid_ephemeris, (unsigned long)msg->collected_almanac, msg->collected_almanac_week, msg->factory_almanac_week,   (long)msg->clk_offset, (unsigned long)msg->reserved );
            break;
         }

         case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 (66) */
         {
            tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "66,%lu,%u,%u,%u,%u,%u",
              (unsigned long)msg->gps_tow,
              msg->gdop, msg->pdop, msg->hdop, msg->vdop, msg->tdop );
            break;
         }

         case SIRF_MSG_SSB_TRKR_DBG: /* 0x44 (68) */
         {
            tSIRF_CHAR *msg = ((tSIRF_CHAR*)message_structure);

            snprintf( szBuf, sizeof(szBuf), "68,%u,%*s", (tSIRF_UINT8)(*msg), (int)(message_length-1), msg+1 );
            break;
         }

         case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05A1 (161, 5) */
         {
            tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "161,5,%lu,%lu,%lu,%lu",
                  (unsigned long)msg->start_mode,
                  (unsigned long)msg->clock_offset,
                  (unsigned long)msg->port_num,
                  (unsigned long)msg->baud_rate );
            break;
         }

         case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06A1 (161, 6) */
         {
            tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "161,6,%lu",
                  (unsigned long)msg->stop_mode );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 (161, 7) */
         {
            snprintf( szBuf, sizeof(szBuf), "161,7" );
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09AC (172, 9) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,9,%u,%u,%u",
                  msg->sensor_data_type,
                  msg->num_of_data_sets,
                  msg->reverse_bitmap );
            for ( i = 0; i < 11; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%u,%lu,%u,%d,%d,%d,%d,%u",
                     msg->data[i].valid_sensor_indication,
                     (unsigned long)msg->data[i].data_set_time_tag,
                     msg->data[i].odometer_speed,
                     msg->data[i].data1,
                     msg->data[i].data2,
                     msg->data[i].data3,
                     msg->data[i].data4,
                     msg->data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0AAC (172, 10) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "172,10,%lu",
                     (unsigned long)msg->mode );
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0BAC (172, 11) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,11,%lu",
                     (unsigned long)msg->mode );
            break;
         }

         case SIRF_MSG_SSB_MMF_DATA: /* 0x50AC (172, 80) */
         {
            tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,80,%lu,%u,%u",
                  (unsigned long)msg->gps_tow_reference,
                  msg->num_of_data_sets,
                  msg->control );
            for ( i = 0; i < 3; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%ld,%ld,%lu,%ld,%lu,%u,%u,%u",
                     (long)msg->mmf_data[i].lat,
                     (long)msg->mmf_data[i].lon,
                     (unsigned long)msg->mmf_data[i].horiz_pos_uncert,
                     (long)msg->mmf_data[i].alt_ellips,
                     (unsigned long)msg->mmf_data[i].vert_pos_uncert,
                     msg->mmf_data[i].heading,
                     msg->mmf_data[i].heading_uncert,
                     msg->mmf_data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            } /* for */
            break;
         }

         case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51AC (172, 81) */
         {
            tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,81,%lu",
                     (unsigned long)msg->mode );
            break;
         }

         case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
         {
            tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "207,%d,%d",
                     msg->RFTestPoint,
                     msg->INTCpuPause );
         }
         break;

         case SIRF_MSG_SSB_TEXT: /* 0xFF (255) */
         {
            tSIRF_CHAR *anStoredDataPacket = ((tSIRF_NAV_TEXT*)message_structure)->msg_text;
            tSIRF_UINT32 cbStoredDataPacket = (tSIRF_UINT32)strlen( anStoredDataPacket );
            snprintf( szBuf, sizeof(szBuf), "%*s", (int)cbStoredDataPacket, anStoredDataPacket );
            /**
             * message may start with \r\n and end with \r\n
             * remove these if existing, replace with space ' '
             */
            for( i = 0; i < cbStoredDataPacket; i++ )
            {
               if( ('\r' == szBuf[i]) || ('\n' == szBuf[i]) )
               {
                  szBuf[i] = ' ';
               }
            }
            szBuf[cbStoredDataPacket] = '\0';
            break;
         }
         case SIRF_MSG_SSB_SIRF_INTERNAL_OUT:
         default: /* Other message: dump in hex */
         {
            snprintf( szBuf, sizeof(szBuf), "%ld,%02X", (unsigned long)SIRF_GET_MID(message_id), SIRF_GET_SUB_ID(message_id));
            for ( i = 0; i < message_length; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%02X", ((tSIRF_UINT8*)message_structure)[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }
      }

      /* Check to make sure that the length of the buffer we are writing into is big enough.
         Account for the line terminator bytes. */
      if ( *lpszText_length < (strlen(szBuf) - 2) )
      {
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
      else
      {
         /* Copy the data to our output buffer */
         strncpy( (char *)lpszText, (const char *)szBuf, *lpszText_length );

         /* Add line terminator */
         strlcat(lpszText, "\x0D\x0A", *lpszText_length);

         /* Calculate string length */
         *lpszText_length = (tSIRF_UINT32)strlen(lpszText);
      }

   } /* if check on input parameters */
   else
   {
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }

   return tRet;

} /* SIRF_CODEC_ASCII_Encode() */


/*=============================================================================

  -----------------------------------------------------------------------------*/
#if 0
tSIRF_RESULT SIRF_CODEC_ASCII_Decode( tSIRF_CHAR   *linestr,
                                      tSIRF_UINT32 *message_id,
                                      tSIRF_VOID   *message_structure,
                                      tSIRF_UINT32 *message_length )
{
   tSIRF_CHAR   *ptr = linestr;
   tSIRF_CHAR   *c;
   tSIRF_UINT32  items_read;

   *message_length = 0;

   /* Decode message id */
   c = strchr( ptr, ',' );
   if ( c )
   {
      while ( ptr != c )
      {
         if ( isdigit( *ptr ) )
         {
            ptr++;
         }
         else
         {
            return SIRF_FAILURE; /* Characters before comma are not number */
         }
      }
   }
   else
   {
      return SIRF_FAILURE; /* Comma has not been found */
   }

   items_read = sscanf( linestr, "%15d,", message_id );
   if ( items_read != 1 )
   {
      return SIRF_FAILURE;
   }

   /* Decode message */
   switch( *message_id )
   {
      case SIRF_GET_MID( SIRF_MSG_SSB_SIRFNAV_NOTIFICATION ):
      {
         tSIRF_UINT32 sub_id = 0;

         items_read = sscanf( linestr, "%15d,%15u", message_id, &sub_id );
         if ( items_read != 2 )
         {
            return SIRF_FAILURE;
         }

         *message_id |= sub_id << 8;

         switch ( sub_id )
         {
            case SIRF_GET_SUB_ID(SIRF_MSG_SSB_SIRFNAV_START):
            {
               tSIRF_MSG_SSB_SIRFNAV_START *msg = (tSIRF_MSG_SSB_SIRFNAV_START*)message_structure;
               *message_length = sizeof(tSIRF_MSG_SSB_SIRFNAV_START);

               memset( msg, 0, sizeof(tSIRF_MSG_SSB_SIRFNAV_START) );

               items_read = sscanf( (const char *)linestr, "51,7,%15lu,%15lu,%15lu,%15lu",
                                     &msg->start_mode, &msg->clock_offset, &msg->port_num, &msg->baud_rate);
               if ( items_read != 4 )
               {
                  return SIRF_FAILURE;
               }
               break;
            }

            default:
               return SIRF_FAILURE;
         } /* switch ( sub_id ) */
         break;
      }

      case SIRF_GET_MID(SIRF_MSG_SSB_DEMO_START_GPS_ENGINE):
      {
         tSIRF_UINT32 sub_id = 0;

         items_read = sscanf( linestr, "%15d,%15u", message_id, &sub_id );
         if ( items_read != 2 )
         {
            return SIRF_FAILURE;
         }

         *message_id |= sub_id << 8;

         switch ( sub_id )
         {
            case SIRF_GET_SUB_ID(SIRF_MSG_SSB_DEMO_START_GPS_ENGINE):
            {
               tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*)message_structure;
               *message_length = sizeof(tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE);

               memset( msg, 0, sizeof(tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE) );

               items_read = sscanf( (const char *)linestr, "161,5,%15lu,%15lu,%15lu,%15lu",
                                    &msg->start_mode, &msg->clock_offset, &msg->port_num, &msg->baud_rate);
               if ( items_read != 4 )
               {
                  return SIRF_FAILURE;
               }
               break;
            }

            default:
               return SIRF_FAILURE;
         } /* switch ( sub_id ) */
         break;
      }
   } /* switch ( message_id ) */

   *message_id |= SIRF_LC_SSB << 16;

   return SIRF_SUCCESS;

} /* SIRF_CODEC_ASCII_Decode() */
#endif

/**
 * @}
 */

