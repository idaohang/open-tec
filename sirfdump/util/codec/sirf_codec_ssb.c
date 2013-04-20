/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_codec_ssb.c
 *
 * @brief  Routines to convert data to and from a byte stream
 *         defined by the Sirf Standard Binary API from and
 *         to the Tracker API defined in the Programmers
 *         Reference Manual.
 *
 */




/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_codec_ssb.h"
#include "sirf_proto_common.h"

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */

/***************************************************************************
 * @brief:   Convert a message structure to a byte stream
 * @param[in]:     message id - SSB message id, from the SSB manual
 * @param[in]:     message_structure - data structure keyed to the message id
 * @param[in]:     message_length - used as string length if not a structure
 * @param[out]:    packet - buffer pointer for destination formatted output
 * @param[in out]: packet_length - size of same, computed after processing
 * @return:   SIRF_SUCCESS, SIRF_CODEC_SSB_LENGTH_ERROR,
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH,
 *            SIRF_CODEC_SSB_NULL_POINTER or SIRF_CODEC_SSB_INVALID_MSG_ID
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_SSB_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   /* Check for NULL pointer and exit if detected. */
   if ( ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT8 *) NULL ==  packet) ||
        ((tSIRF_UINT32 *) NULL == packet_length))
   {
      return (SIRF_CODEC_SSB_NULL_POINTER);
   }

   /* Otherwise, process the message */
   {
      tSIRF_UINT32 i, j;
      tSIRF_UINT8 *ptr = packet;
      tSIRF_UINT8 header_len = 0;

      if (SIRF_GET_SUB_ID(message_id))
      {
         header_len++; /* start with sid byte count */
      }
#ifndef SIRF_LOC
      header_len += 1 * sizeof(tSIRF_UINT8); /* mid byte only */
#else
      header_len += 2 * sizeof(tSIRF_UINT8); /* mid + channel bytes */

      /* @todo It is possible for SIRF_LOC to be outputting onto Port B
       * and thus not wanting to input the logical channel  THIS IS BROKEN */
      /* insert the logical channel */
      SIRFBINARY_EXPORT8((tSIRF_UINT8)SIRF_LC_SSB,ptr);
#endif

      /* At a minimum, make sure we have room for the header bytes */
      if ( *packet_length < header_len )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      /* insert the message id */
      SIRFBINARY_EXPORT8((tSIRF_UINT8)message_id, ptr);
      /* this is the second byte of the message id for those that offer one */
      if (SIRF_GET_SUB_ID(message_id))
      {
         SIRFBINARY_EXPORT8((tSIRF_UINT8)(SIRF_GET_SUB_ID(message_id)), ptr);
      }

      switch (message_id)
      {
      case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 */
      {
         tSIRF_MSG_SSB_MEASURED_NAVIGATION * msg = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*) message_structure;

         /**
          * Attention programmers: the code compares the count of data bytes to *packet_length:
          * it generates the count by scanning the data store and adding up all of the
          * UINT8s then all of the UINT16s, then all of the UINT32s.
          */

         /* Check to make sure that the length of the buffer we are writing into is big enough.
            Include the header bytes.  */
         if ( *packet_length < ( (4 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                  4 * sizeof(tSIRF_UINT16) +
                                  4 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_x,    ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_y,    ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_z,    ptr);
            SIRFBINARY_EXPORT8( msg->nav_mode,      ptr);
            SIRFBINARY_EXPORT8( msg->hdop,          ptr);
            SIRFBINARY_EXPORT8( msg->nav_mode2,     ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT8( msg->sv_used_cnt,   ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8(msg->sv_used[i],  ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MEASURED_TRACKER: /* 0x04 */
      {
         tSIRF_MSG_SSB_MEASURED_TRACKER * msg = (tSIRF_MSG_SSB_MEASURED_TRACKER*) message_structure;

         if ( *packet_length < ( (1 + 3 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                 (SIRF_NUM_CHANNELS * SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                 (1 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT16) +
                                  1 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT8( msg->chnl_cnt,      ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8( msg->chnl[i].svid,      ptr);
               SIRFBINARY_EXPORT8( msg->chnl[i].azimuth,   ptr);
               SIRFBINARY_EXPORT8( msg->chnl[i].elevation, ptr);
               SIRFBINARY_EXPORT16(msg->chnl[i].state,     ptr);
               for (j = 0; j < SIRF_NUM_POINTS; j++)
               {
                  SIRFBINARY_EXPORT8(msg->chnl[i].cno[j],  ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 */
      {
         tSIRF_MSG_SSB_RAW_TRACKER * msg = (tSIRF_MSG_SSB_RAW_TRACKER*) message_structure;

         if ( *packet_length < ( (2 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                  7 * sizeof(tSIRF_UINT16) +
                                  6 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->channel,              ptr);
            SIRFBINARY_EXPORT16(msg->svid,                 ptr);
            SIRFBINARY_EXPORT16(msg->state,                ptr);
            SIRFBINARY_EXPORT32(msg->bit_number,           ptr);
            SIRFBINARY_EXPORT16(msg->msec_number,          ptr);
            SIRFBINARY_EXPORT16(msg->chip_number,          ptr);
            SIRFBINARY_EXPORT32(msg->code_phase,           ptr);
            SIRFBINARY_EXPORT32(msg->carrier_doppler,      ptr);
            SIRFBINARY_EXPORT32(msg->measure_timetag,      ptr);
            SIRFBINARY_EXPORT32(msg->delta_carrier_phase,  ptr);
            SIRFBINARY_EXPORT16(msg->search_cnt,           ptr);
            for (i = 0; i < SIRF_NUM_POINTS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->cno[i],  ptr);
            }
            SIRFBINARY_EXPORT8 (msg->power_bad_count,      ptr);
            SIRFBINARY_EXPORT8 (msg->phase_bad_count,      ptr);
            SIRFBINARY_EXPORT16(msg->delta_car_interval,   ptr);
            SIRFBINARY_EXPORT16(msg->correl_interval,      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SW_VERSION:   /* 0x06 */
      {
         tSIRF_MSG_SSB_SW_VERSION * msg = (tSIRF_MSG_SSB_SW_VERSION*) message_structure;

         /* Check to make sure that the length of the buffer we are writing into is big enough.
            Also make sure that the requested copy length does not exceed our capacity. */
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, msg->sw_version, message_length);
            ptr += message_length;
         }
      }
      break;
      case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 */
      {
         tSIRF_MSG_SSB_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_CLOCK_STATUS*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week,       ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,        ptr);
            SIRFBINARY_EXPORT8( msg->sv_used_cnt,    ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,     ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias,       ptr);
            SIRFBINARY_EXPORT32(msg->est_gps_time,   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_50BPS_DATA:          /* 0x08 */
      case SIRF_MSG_SSB_50BPS_DATA_VERIFIED: /* 0x52 */
      {
         tSIRF_MSG_SSB_50BPS_DATA * msg = (tSIRF_MSG_SSB_50BPS_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 10 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->chnl,             ptr);
            SIRFBINARY_EXPORT8 (msg->svid,             ptr);
            for (i = 0; i < 10; i++)
            {
               SIRFBINARY_EXPORT32(msg->word[i],       ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 */
      {
         tSIRF_MSG_SSB_THROUGHPUT * msg = (tSIRF_MSG_SSB_THROUGHPUT*) message_structure;

         if ( *packet_length < ( 4 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->seg_stat_max,  ptr);
            SIRFBINARY_EXPORT16(msg->seg_stat_lat,  ptr);
            SIRFBINARY_EXPORT16(msg->avg_trk_time,  ptr);
            SIRFBINARY_EXPORT16(msg->last_ms,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_ERROR: /* 0x0A */
      {
         tSIRF_MSG_SSB_ERROR * msg = (tSIRF_MSG_SSB_ERROR*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT16) +
                                 SIRF_MSG_SSB_MAX_ERROR_PARAMS * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->err_id,         ptr);
            SIRFBINARY_EXPORT16(msg->param_cnt,      ptr);
            if (msg->param_cnt > SIRF_MSG_SSB_MAX_ERROR_PARAMS)
            {
               msg->param_cnt = SIRF_MSG_SSB_MAX_ERROR_PARAMS;
            }
            for (i = 0; i < SIRF_MSG_SSB_MAX_ERROR_PARAMS; i++)
            {
               SIRFBINARY_EXPORT32(msg->param[i],    ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_ACK: /* 0x0B */
      {
         tSIRF_MSG_SSB_ACK * msg = (tSIRF_MSG_SSB_ACK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->msg_id, ptr);
            SIRFBINARY_EXPORT8(msg->sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NAK: /* 0x0C */
      {
         tSIRF_MSG_SSB_NAK * msg = (tSIRF_MSG_SSB_NAK *) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->msg_id, ptr);
            SIRFBINARY_EXPORT8(msg->sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D */
      {
         tSIRF_MSG_SSB_VISIBILITY_LIST * msg = (tSIRF_MSG_SSB_VISIBILITY_LIST*) message_structure;

         if ( *packet_length < ( (1 + msg->svid_cnt) * sizeof(tSIRF_UINT8) +
                                 (2 * msg->svid_cnt) * sizeof(tSIRF_UINT16) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid_cnt,                 ptr);
            for (i = 0; i < msg->svid_cnt; i++)
            {
               SIRFBINARY_EXPORT8 (msg->visible[i].svid,        ptr);
               SIRFBINARY_EXPORT16(msg->visible[i].azimuth,     ptr);
               SIRFBINARY_EXPORT16(msg->visible[i].elevation,   ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_ALMANAC: /* 0x0E */
      {
         tSIRF_MSG_SSB_ALMANAC * msg = (tSIRF_MSG_SSB_ALMANAC*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                (2 + SIRF_MSG_SSB_ALMANAC_ENTRIES) * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,            ptr);
            SIRFBINARY_EXPORT16(msg->week_and_status, ptr);
            for (i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++)
            {
               SIRFBINARY_EXPORT16(msg->data[i], ptr);
            }
            SIRFBINARY_EXPORT16(msg->almanac_checksum,ptr);

         }
      }
      break;
      case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F */
      {
         tSIRF_MSG_SSB_EPHEMERIS * msg = (tSIRF_MSG_SSB_EPHEMERIS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) +
                                3 * 15 * sizeof(tSIRF_UINT16) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,                ptr);
            for (i = 0; i < 3; i++)
            {
               for (j = 0; j < 15; j++)
               {
                  SIRFBINARY_EXPORT16(msg->subframe[i][j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 */
      {
         tSIRF_MSG_SSB_OK_TO_SEND * msg = (tSIRF_MSG_SSB_OK_TO_SEND*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->input_enabled, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 */
      {
         tSIRF_MSG_SSB_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_RECEIVER_PARAMS*) message_structure;

         if ( *packet_length < ( 20 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->reserved_1,             ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,               ptr);
            SIRFBINARY_EXPORT8 (msg->alt_src,                ptr);
            SIRFBINARY_EXPORT16(msg->alt_input,              ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,          ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_timeout,       ptr);
            SIRFBINARY_EXPORT8 (msg->dr_timeout,             ptr);
            SIRFBINARY_EXPORT8 (msg->trk_smooth,             ptr);
            SIRFBINARY_EXPORT8 (msg->static_nav_mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->enable_3sv_lsq,         ptr);
            SIRFBINARY_EXPORT32(msg->reserved_2,             ptr);
            SIRFBINARY_EXPORT8 (msg->dop_mask_mode,          ptr);
            SIRFBINARY_EXPORT16(msg->nav_elev_mask,          ptr);
            SIRFBINARY_EXPORT8 (msg->nav_pwr_mask,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved_3,             ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_src,               ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_mode,              ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_timeout,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved_4,             ptr);
            SIRFBINARY_EXPORT8 (msg->lp_push_to_fix,         ptr);
            SIRFBINARY_EXPORT32(msg->lp_on_time,             ptr);
            SIRFBINARY_EXPORT32(msg->lp_interval,            ptr);
            SIRFBINARY_EXPORT8 (msg->lp_user_tasks_enabled,  ptr);
            SIRFBINARY_EXPORT32(msg->lp_user_task_interval,  ptr);
            SIRFBINARY_EXPORT8 (msg->lp_pwr_cycling_enabled, ptr);
            SIRFBINARY_EXPORT32(msg->lp_max_acq_time,        ptr);
            SIRFBINARY_EXPORT32(msg->lp_max_off_time,        ptr);
            SIRFBINARY_EXPORT8 (msg->apm_enabled_power_duty_cycle, ptr);
            SIRFBINARY_EXPORT16(msg->number_of_fixes,        ptr);
            SIRFBINARY_EXPORT16(msg->time_between_fixes,      ptr);
            SIRFBINARY_EXPORT8 (msg->horz_vert_error_max,      ptr);
            SIRFBINARY_EXPORT8 (msg->response_time_max,        ptr);
            SIRFBINARY_EXPORT8 (msg->time_accu_time_duty_cycle_priority, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA * msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*) message_structure;

         if ( *packet_length < ( 13 * sizeof(tSIRF_UINT16) +
                                 6 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,               ptr);
            SIRFBINARY_EXPORT16(msg->period,             ptr);
            SIRFBINARY_EXPORT16(msg->bit_synch_time,     ptr);
            SIRFBINARY_EXPORT16(msg->bit_count,          ptr);
            SIRFBINARY_EXPORT16(msg->poor_status_count,  ptr);
            SIRFBINARY_EXPORT16(msg->good_status_count,  ptr);
            SIRFBINARY_EXPORT16(msg->parity_error_count, ptr);
            SIRFBINARY_EXPORT16(msg->lost_vco_count,     ptr);
            SIRFBINARY_EXPORT16(msg->frame_synch_time,   ptr);
            SIRFBINARY_EXPORT16(msg->cno_mean,           ptr);
            SIRFBINARY_EXPORT16(msg->cno_sigma,          ptr);
            SIRFBINARY_EXPORT16(msg->clock_drift,        ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset_mean,  ptr);
            SIRFBINARY_EXPORT16(msg->bad_1khz_bit_count, ptr);
            SIRFBINARY_EXPORT32(msg->abs_i20ms,          ptr);
            SIRFBINARY_EXPORT32(msg->abs_q1ms,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved[0],        ptr);
            SIRFBINARY_EXPORT32(msg->reserved[1],        ptr);
            SIRFBINARY_EXPORT32(msg->reserved[2],        ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B */
      {
         tSIRF_MSG_SSB_DGPS_STATUS * msg = (tSIRF_MSG_SSB_DGPS_STATUS*) message_structure;

         if ( *packet_length < ( (3 + 2 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                  SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT16) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->src,                    ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->cor_age[i],          ptr);
            }
            SIRFBINARY_EXPORT8 (msg->reserved[0],            ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],            ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->corrections[i].prn,  ptr);
               SIRFBINARY_EXPORT16(msg->corrections[i].cor,  ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C */
      {
         tSIRF_MSG_SSB_NL_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_MEAS_DATA*) message_structure;

         if ( *packet_length < ( (5 + 10) * sizeof(tSIRF_UINT8) +
                                  4 * sizeof(tSIRF_UINT16) +
                                  2 * sizeof(tSIRF_UINT32) +
                                  3 * sizeof(tSIRF_DOUBLE) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->Chnl,                 ptr);
            SIRFBINARY_EXPORT32(msg->Timetag,              ptr);
            SIRFBINARY_EXPORT8 (msg->svid,                 ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->gps_sw_time,          ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->pseudorange,          ptr);
            SIRFBINARY_EXPORT32(msg->carrier_freq,         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->carrier_phase,        ptr);
            SIRFBINARY_EXPORT16(msg->time_in_track,        ptr);
            SIRFBINARY_EXPORT8 (msg->sync_flags,           ptr);
            for (i = 0; i <10; i++)
            {
               SIRFBINARY_EXPORT8(msg->cton[i],            ptr);
            }
            SIRFBINARY_EXPORT16(msg->delta_range_interval, ptr);
            SIRFBINARY_EXPORT16(msg->mean_delta_range_time,ptr);
            SIRFBINARY_EXPORT16(msg->extrapolation_time,   ptr);
            SIRFBINARY_EXPORT8 (msg->phase_error_count,    ptr);
            SIRFBINARY_EXPORT8 (msg->low_power_count,      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D */
      {
         tSIRF_MSG_SSB_NL_DGPS_DATA * msg = (tSIRF_MSG_SSB_NL_DGPS_DATA*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,           ptr);
            SIRFBINARY_EXPORT16(msg->iod,            ptr);
            SIRFBINARY_EXPORT8 (msg->source,         ptr);
            SIRFBINARY_EXPORT32(msg->pr_correction,  ptr);
            SIRFBINARY_EXPORT32(msg->prr_correction, ptr);
            SIRFBINARY_EXPORT32(msg->correction_age, ptr);
            SIRFBINARY_EXPORT32(msg->prc_variance,   ptr);
            SIRFBINARY_EXPORT32(msg->prrc_variance,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E */
      {
         tSIRF_MSG_SSB_NL_SV_STATE_DATA * msg = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT32) +
                                (2 + 3 + 3) * sizeof(tSIRF_DOUBLE) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,        ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->time,        ptr);
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT_DOUBLE(msg->pos[i],   ptr);
            }
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT_DOUBLE(msg->vel[i],   ptr);
            }
            SIRFBINARY_EXPORT_DOUBLE(msg->clk,         ptr);
            SIRFBINARY_EXPORT32(msg->clf,         ptr);
            SIRFBINARY_EXPORT8 (msg->eph,         ptr);
            SIRFBINARY_EXPORT32(msg->posvar,      ptr);
            SIRFBINARY_EXPORT32(msg->clkvar,      ptr);
            SIRFBINARY_EXPORT32(msg->iono,        ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F */
      {
         tSIRF_MSG_SSB_NL_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_INIT_DATA*) message_structure;

         if ( *packet_length < ( 13 * sizeof(tSIRF_UINT8) +
                                 13 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 5 * sizeof(tSIRF_DOUBLE) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->enable_con_alt_mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,                   ptr);
            SIRFBINARY_EXPORT8 (msg->alt_source,                 ptr);
            SIRFBINARY_EXPORT32(msg->altitude,                   ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,              ptr);
            SIRFBINARY_EXPORT16(msg->degraded_timeout,           ptr);
            SIRFBINARY_EXPORT16(msg->dr_timeout,                 ptr);
            SIRFBINARY_EXPORT16(msg->coast_timeout,              ptr);
            SIRFBINARY_EXPORT8 (msg->tracksmooth_mode,           ptr);
            SIRFBINARY_EXPORT8 (msg->dop_selection,              ptr);
            SIRFBINARY_EXPORT16(msg->hdop_thresh,                ptr);
            SIRFBINARY_EXPORT16(msg->gdop_thresh,                ptr);
            SIRFBINARY_EXPORT16(msg->pdop_thresh,                ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_selection,             ptr);
            SIRFBINARY_EXPORT16(msg->dgps_timeout,               ptr);
            SIRFBINARY_EXPORT16(msg->elev_nav_mask,              ptr);
            SIRFBINARY_EXPORT16(msg->pow_nav_mask,               ptr);
            SIRFBINARY_EXPORT8 (msg->editing_residual_mode,      ptr);
            SIRFBINARY_EXPORT16(msg->editing_residual_threshold, ptr);
            SIRFBINARY_EXPORT8 (msg->ssd_mode,                   ptr);
            SIRFBINARY_EXPORT16(msg->ssd_threshold,              ptr);
            SIRFBINARY_EXPORT8 (msg->static_nav_mode,            ptr);
            SIRFBINARY_EXPORT16(msg->static_nav_threshold,       ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_x,                ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_y,                ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_z,                ptr);
            SIRFBINARY_EXPORT8 (msg->position_init_source,       ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->gps_time,              ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,                   ptr);
            SIRFBINARY_EXPORT8 (msg->time_init_source,           ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->clk_offset,            ptr);
            SIRFBINARY_EXPORT8 (msg->clk_offset_init_source,     ptr);
         }
      }
      break;
      
      case SIRF_MSG_SSB_NL_AUX_INIT_DATA:  /* 0x40 0x01 */
      {
         tSIRF_MSG_SSB_NL_AUX_INIT_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA *) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8)  +
                                 4 * sizeof(tSIRF_UINT16) +
                                 6 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->time_init_unc, ptr);   
            SIRFBINARY_EXPORT16 (msg->saved_pos_week, ptr);  
            SIRFBINARY_EXPORT32 (msg->saved_pos_tow, ptr);   
            SIRFBINARY_EXPORT16 (msg->saved_pos_ehe, ptr);   
            SIRFBINARY_EXPORT16 (msg->saved_pos_eve, ptr);
            SIRFBINARY_EXPORT8  (msg->sw_version, ptr);
            SIRFBINARY_EXPORT8  (msg->icd_version, ptr);
            SIRFBINARY_EXPORT16 (msg->chip_version, ptr);
            SIRFBINARY_EXPORT32 (msg->acq_clk_speed, ptr);
            SIRFBINARY_EXPORT32 (msg->default_clock_offset, ptr);
            SIRFBINARY_EXPORT32 (msg->tracker_status, ptr);
            SIRFBINARY_EXPORT32 (msg->reserved, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_NL_AUX_MEAS_DATA:  /* 0x40 0x02 */
      {
         tSIRF_MSG_SSB_NL_AUX_MEAS_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA *) message_structure;

         if ( *packet_length < (  5 * sizeof(tSIRF_UINT8)  +
                                  8 * sizeof(tSIRF_INT16)  +
                                  2 * sizeof(tSIRF_UINT16) +
                                  7 * sizeof(tSIRF_INT32)  +
                                  3 * sizeof(tSIRF_UINT32) +
                                  header_len) )

         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8  (msg->sv_prn,              ptr);
            SIRFBINARY_EXPORT8  (msg->status,              ptr);
            SIRFBINARY_EXPORT8  (msg->extended_status,     ptr);
            SIRFBINARY_EXPORT8  (msg->bit_sync_qual,       ptr);
            SIRFBINARY_EXPORT32 (msg->time_tag,            ptr);
            SIRFBINARY_EXPORT32 (msg->code_phase,          ptr);
            SIRFBINARY_EXPORT32 (msg->carrier_phase,       ptr);
            SIRFBINARY_EXPORT32 (msg->carrier_freq,        ptr);
            SIRFBINARY_EXPORT16 (msg->carrier_accel,       ptr);
            SIRFBINARY_EXPORT16 (msg->ms_num,              ptr);
            SIRFBINARY_EXPORT32 (msg->bit_num,             ptr);
            SIRFBINARY_EXPORT32 (msg->code_correction,     ptr);
            SIRFBINARY_EXPORT32 (msg->smooth_code,         ptr);
            SIRFBINARY_EXPORT32 (msg->code_offset,         ptr);
            SIRFBINARY_EXPORT16 (msg->pseudorange_noise,   ptr);
            SIRFBINARY_EXPORT16 (msg->delta_range_qual,    ptr);
            SIRFBINARY_EXPORT16 (msg->phase_lock_qual,     ptr);
            SIRFBINARY_EXPORT16 (msg->ms_uncertainty,      ptr);
            SIRFBINARY_EXPORT16 (msg->sum_abs_I,           ptr);
            SIRFBINARY_EXPORT16 (msg->sum_abs_Q,           ptr);
            SIRFBINARY_EXPORT32 (msg->sv_bit_num,          ptr);
            SIRFBINARY_EXPORT16 (msg->mp_los_det_value,    ptr);
            SIRFBINARY_EXPORT16 (msg->mp_only_det_value,   ptr);
            SIRFBINARY_EXPORT8  (msg->recovery_status,     ptr);
            SIRFBINARY_EXPORT32 (msg->sw_time_uncertainty, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_NL_AUX_AID_DATA:  /* 0x40 0x03 */
      {
         tSIRF_MSG_SSB_NL_AUX_AID_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_AID_DATA *) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_INT32)  +
                                 2 * sizeof(tSIRF_UINT32) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT32(msg->horiz_pos_unc, ptr);
            SIRFBINARY_EXPORT16(msg->alt_unc,       ptr);
            SIRFBINARY_EXPORT32(msg->sw_tow,        ptr);
         }
      }
      break;
      
      case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 */
      {
         tSIRF_MSG_SSB_GEODETIC_NAVIGATION * msg = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*) message_structure;

         if ( *packet_length < ( 8 * sizeof(tSIRF_UINT8) +
                                 13 * sizeof(tSIRF_UINT16) +
                                 14 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->nav_valid,                ptr);
            SIRFBINARY_EXPORT16(msg->nav_mode,                 ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,                 ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                  ptr);
            SIRFBINARY_EXPORT16(msg->utc_year,                 ptr);
            SIRFBINARY_EXPORT8 (msg->utc_month,                ptr);
            SIRFBINARY_EXPORT8 (msg->utc_day,                  ptr);
            SIRFBINARY_EXPORT8 (msg->utc_hour,                 ptr);
            SIRFBINARY_EXPORT8 (msg->utc_min,                  ptr);
            SIRFBINARY_EXPORT16(msg->utc_sec,                  ptr);
            SIRFBINARY_EXPORT32(msg->sv_used,                  ptr);
            SIRFBINARY_EXPORT32(msg->lat,                      ptr);
            SIRFBINARY_EXPORT32(msg->lon,                      ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,               ptr);
            SIRFBINARY_EXPORT32(msg->alt_msl,                  ptr);
            SIRFBINARY_EXPORT8 (msg->datum,                    ptr);
            SIRFBINARY_EXPORT16(msg->sog,                      ptr);
            SIRFBINARY_EXPORT16(msg->hdg,                      ptr);
            SIRFBINARY_EXPORT16(msg->mag_var,                  ptr);
            SIRFBINARY_EXPORT16(msg->climb_rate,               ptr);
            SIRFBINARY_EXPORT16(msg->heading_rate,             ptr);
            SIRFBINARY_EXPORT32(msg->ehpe,                     ptr);
            SIRFBINARY_EXPORT32(msg->evpe,                     ptr);
            SIRFBINARY_EXPORT32(msg->ete,                      ptr);
            SIRFBINARY_EXPORT16(msg->ehve,                     ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias,                 ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias_error,           ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,               ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset_error,         ptr);
            SIRFBINARY_EXPORT32(msg->distance_travelled,       ptr);
            SIRFBINARY_EXPORT16(msg->distance_travelled_error, ptr);
            SIRFBINARY_EXPORT16(msg->heading_error,            ptr);
            SIRFBINARY_EXPORT8 (msg->sv_used_cnt,              ptr);
            SIRFBINARY_EXPORT8 (msg->hdop,                     ptr);
            SIRFBINARY_EXPORT8 (msg->additional_mode_info,     ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D */
      {
         /* programmers note: the "_1HZ" version is just ten of the non- "_1HZ" version,
          * so does it make sense to copy what's there, progressively, or does it
          * make sense to reject the message if it is not precisely the right size? */
         if (sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA) == message_length)
         {
            tSIRF_MSG_SSB_ADC_ODOMETER_DATA * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*) message_structure;

            if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT32(msg->current_time,   ptr);
               SIRFBINARY_EXPORT16(msg->adc2_avg,       ptr);
               SIRFBINARY_EXPORT16(msg->adc3_avg,       ptr);
               SIRFBINARY_EXPORT16(msg->odo_count,      ptr);
               SIRFBINARY_EXPORT8 (msg->gpio_stat,      ptr);
            }
         }
         else if (sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ) == message_length)
         {
            tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*) message_structure;

            if ( *packet_length < ( 1 * 10 * sizeof(tSIRF_UINT8) +
                                    3 * 10 * sizeof(tSIRF_UINT16) +
                                    1 * 10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for (i = 0; i < 10; i++)
               {
                  SIRFBINARY_EXPORT32(msg->dataset[i].current_time,   ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].adc2_avg,       ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].adc3_avg,       ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].odo_count,      ptr);
                  SIRFBINARY_EXPORT8 (msg->dataset[i].gpio_stat,      ptr);
               }
            }
         }
         else
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3 *)message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 16 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,              ptr);
            SIRFBINARY_EXPORT16(msg->period,            ptr);
            SIRFBINARY_EXPORT16(msg->bit_synch_time,    ptr);
            SIRFBINARY_EXPORT16(msg->bit_count,         ptr);
            SIRFBINARY_EXPORT16(msg->poor_status_count, ptr);
            SIRFBINARY_EXPORT16(msg->good_status_count, ptr);
            SIRFBINARY_EXPORT16(msg->parity_error_count,ptr);
            SIRFBINARY_EXPORT16(msg->lost_vco_count,    ptr);
            SIRFBINARY_EXPORT16(msg->frame_synch_time,  ptr);
            SIRFBINARY_EXPORT16(msg->cno_mean,          ptr);
            SIRFBINARY_EXPORT16(msg->cno_sigma,         ptr);
            SIRFBINARY_EXPORT16(msg->clock_drift,       ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset,      ptr);
            SIRFBINARY_EXPORT16(msg->bad_1khz_bit_count,ptr);
            SIRFBINARY_EXPORT32(msg->abs_i20ms,         ptr);
            SIRFBINARY_EXPORT32(msg->abs_q20ms,         ptr);
            SIRFBINARY_EXPORT32(msg->phase_lock,        ptr);
            SIRFBINARY_EXPORT16(msg->rtc_frequency,     ptr);
            SIRFBINARY_EXPORT16(msg->e_to_acq_ratio,    ptr);
            SIRFBINARY_EXPORT8(msg->t_sync_agc_gain,    ptr);
            SIRFBINARY_EXPORT8(msg->tm_5_ready,         ptr);
            SIRFBINARY_EXPORT16(msg->ClkDriftUnc,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x01 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*) message_structure;

         if ( *packet_length < ( 14 * sizeof(tSIRF_UINT8) +
                                  2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->nav,ptr);
            SIRFBINARY_EXPORT16(msg->data,ptr);
            SIRFBINARY_EXPORT8(msg->cal_gb_cal,ptr);
            SIRFBINARY_EXPORT8(msg->gsf_cal_ssf_cal,ptr);
            SIRFBINARY_EXPORT8(msg->nav_across_reset_pos,ptr);
            SIRFBINARY_EXPORT8(msg->hd,ptr);
            SIRFBINARY_EXPORT8(msg->gyr_sub_op_spd_sub_op,ptr);
            SIRFBINARY_EXPORT8(msg->nav_st_int_ran_z_gb_cal_upd,ptr);
            SIRFBINARY_EXPORT8(msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st,ptr);
            SIRFBINARY_EXPORT8(msg->gps_upd_pos,ptr);
            SIRFBINARY_EXPORT8(msg->gps_upd_hd,ptr);
            SIRFBINARY_EXPORT8(msg->gps_pos_gps_vel,ptr);

            /* Where did these fields come from? */
            SIRFBINARY_EXPORT8(msg->dws_hd_rt_s_f_cal_valid,ptr);
            SIRFBINARY_EXPORT8(msg->dws_hd_rt_s_f_cal_upd,ptr);
            SIRFBINARY_EXPORT16(msg->dws_spd_s_f_cal_valid,ptr);
            SIRFBINARY_EXPORT8(msg->dws_spd_s_f_cal_upd,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x02 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_UINT8) +
                                 33 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->spd,ptr);
            SIRFBINARY_EXPORT16(msg->spd_e,ptr);
            SIRFBINARY_EXPORT16(msg->ssf,ptr);
            SIRFBINARY_EXPORT16(msg->ssf_e,ptr);
            SIRFBINARY_EXPORT16(msg->hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->gb,ptr);
            SIRFBINARY_EXPORT16(msg->gbE,ptr);
            SIRFBINARY_EXPORT16(msg->gsf,ptr);
            SIRFBINARY_EXPORT16(msg->gsf_e,ptr);
            SIRFBINARY_EXPORT32(msg->tpe,ptr);
            SIRFBINARY_EXPORT16(msg->the,ptr);
            SIRFBINARY_EXPORT8(msg->nav_ctrl,ptr);
            SIRFBINARY_EXPORT8(msg->reverse,ptr);
            SIRFBINARY_EXPORT16(msg->hd,ptr);
            SIRFBINARY_EXPORT8(msg->sensor_pkg,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd_s_f,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd_s_f_err,ptr);
            SIRFBINARY_EXPORT16(msg->lf_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->lf_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rf_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->rf_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->lr_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->lr_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rr_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->rr_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_spd_delta,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_avg_spd,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_spd_err,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_hd_rt,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_hd_rt_err,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_spd_delta,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_avg_spd,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_spd_err,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_hd_rt,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_hd_rt_err,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x03 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 12 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd_e,ptr);
            SIRFBINARY_EXPORT16(msg->gps_spd,ptr);
            SIRFBINARY_EXPORT16(msg->gps_spd_e,ptr);
            SIRFBINARY_EXPORT32(msg->gps_pos_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_spd,ptr);
            SIRFBINARY_EXPORT16(msg->dr_spd_e,ptr);
            SIRFBINARY_EXPORT32(msg->dr_pos_e,ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[0],ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_VALID: /* 0x05 0x30 */
      {
         tSIRF_MSG_SSB_DR_VALID * msg = (tSIRF_MSG_SSB_DR_VALID*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->valid,                         ptr);
            SIRFBINARY_EXPORT32(msg->reserved,                      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_GYR_FACT_CAL: /* 0x06 30 */
      {
         tSIRF_MSG_SSB_DR_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_GYR_FACT_CAL*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->cal,                           ptr);
            SIRFBINARY_EXPORT8 (msg->reserved,                      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x07 30 */
      {
         tSIRF_MSG_SSB_DR_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) +
                                2 * sizeof(tSIRF_UINT16) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->base_ssf,ptr);
            SIRFBINARY_EXPORT16(msg->base_gb,ptr);
            SIRFBINARY_EXPORT16(msg->base_gsf,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x08 0x30 */
      {
         tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*) message_structure;

         if ( *packet_length < (  2 * sizeof(tSIRF_UINT8) +
                                 (1 + 2 * SIRF_MSG_SSB_DR_DATASET_LENGTH) * sizeof(tSIRF_UINT16) +
                                 (1 + 1 * SIRF_MSG_SSB_DR_DATASET_LENGTH) * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->meas_type,ptr);
            SIRFBINARY_EXPORT8(msg->valid_cnt,ptr);
            SIRFBINARY_EXPORT16(msg->bkup_flgs,ptr);
            for (i = 0; i < SIRF_MSG_SSB_DR_DATASET_LENGTH; i++)
            {
               SIRFBINARY_EXPORT32(msg->blk[i].tag,                 ptr);
               SIRFBINARY_EXPORT16(msg->blk[i].spd,                 ptr);
               SIRFBINARY_EXPORT16(msg->blk[i].hd_rte,              ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MMF_STATUS: /*  0x5030 */
      {
         tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->status,   ptr);
            SIRFBINARY_EXPORT16(msg->sys_hdg,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_lat,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_lon,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_alt,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_tow,  ptr);
            SIRFBINARY_EXPORT16(msg->mmf_hdg,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_lat,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_lon,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_alt,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_tow,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 */
      {
         tSIRF_MSG_SSB_SBAS_PARAM * msg = (tSIRF_MSG_SSB_SBAS_PARAM*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->prn,      ptr);
            SIRFBINARY_EXPORT8 (msg->mode,     ptr);
            SIRFBINARY_EXPORT8 (msg->timeout,  ptr);
            SIRFBINARY_EXPORT8 (msg->flg_bits, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x01 0x33 */
      {
         tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x04 0x33 */
      {
         tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*) message_structure;

         if ( *packet_length < ( 6 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 3 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->tracker_time_tag, ptr);
            SIRFBINARY_EXPORT16(msg->gps_week, ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_day, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_hour, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_min, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_sec, ptr);
            SIRFBINARY_EXPORT16(msg->tracker_rtc_clkctr, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_ok, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_rollover_complete, ptr);
            SIRFBINARY_EXPORT32(msg->tracker_rtc_rollover_seconds, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
      {
         tSIRF_MSG_SSB_SIRFNAV_START* msg = (tSIRF_MSG_SSB_SIRFNAV_START*) message_structure;
         
         if ( *packet_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->start_mode    ,ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset  ,ptr);
            SIRFBINARY_EXPORT32(msg->port_num      ,ptr);
            SIRFBINARY_EXPORT32(msg->baud_rate     ,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (51, 8) */
      {
         tSIRF_MSG_SSB_SIRFNAV_STOP* msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*) message_structure;
         
         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->stop_mode  ,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_STARTUP_INFO: /* 0x01 0x36 */
      {
         tSIRF_MSG_SSB_STARTUP_INFO * msg = (tSIRF_MSG_SSB_STARTUP_INFO*) message_structure;

         if ( *packet_length < ( 7 * sizeof(tSIRF_UINT8) +
                                 6 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,                   ptr);
            SIRFBINARY_EXPORT8 (msg->status,                 ptr);
            SIRFBINARY_EXPORT8 (msg->dr_status,              ptr);
            SIRFBINARY_EXPORT32(msg->gps_off_time,           ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,               ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                ptr);
            SIRFBINARY_EXPORT16(msg->utc_year,               ptr);
            SIRFBINARY_EXPORT8 (msg->utc_month,              ptr);
            SIRFBINARY_EXPORT8 (msg->utc_day,                ptr);
            SIRFBINARY_EXPORT8 (msg->utc_hour,               ptr);
            SIRFBINARY_EXPORT8 (msg->utc_min,                ptr);
            SIRFBINARY_EXPORT16(msg->utc_sec,                ptr);
            SIRFBINARY_EXPORT32(msg->lat,                    ptr);
            SIRFBINARY_EXPORT32(msg->lon,                    ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,             ptr);
            SIRFBINARY_EXPORT16(msg->heading,                ptr);
            SIRFBINARY_EXPORT32(msg->valid_ephemeris,        ptr);
            SIRFBINARY_EXPORT32(msg->collected_almanac,      ptr);
            SIRFBINARY_EXPORT16(msg->collected_almanac_week, ptr);
            SIRFBINARY_EXPORT16(msg->factory_almanac_week,   ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,             ptr);
            SIRFBINARY_EXPORT32(msg->reserved,               ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_GPS_TIME_INFO:  /* 0x01 0x38 */
      {
         tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO *) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->time_valid_flag,         ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,               ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                ptr);
            SIRFBINARY_EXPORT32(msg->ephemeris_request_mask, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_INTEGRITY:  /* 0x02 0x38 */
      {
         tSIRF_MSG_SSB_EE_INTEGRITY *msg = (tSIRF_MSG_SSB_EE_INTEGRITY *) message_structure;

         if ( *packet_length < (3 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->sat_pos_validity_mask, ptr);
            SIRFBINARY_EXPORT32(msg->sat_clk_validity_mask, ptr);
            SIRFBINARY_EXPORT32(msg->sat_health_mask,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_STATE:      /* 0x03 0x38 */
      {
         tSIRF_MSG_SSB_EE_STATE *msg =
            (tSIRF_MSG_SSB_EE_STATE *) message_structure;

         if ( *packet_length < (6 * SIRF_MAX_SVID_CNT * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for( i = 0; i < SIRF_MAX_SVID_CNT; i++)
            {
               SIRFBINARY_EXPORT8(msg[i].svid,            ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_source_flag, ptr);
               SIRFBINARY_EXPORT8(msg[i].week_no,         ptr);
               SIRFBINARY_EXPORT8(msg[i].toe,             ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_integrity,   ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_age,         ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_EE_CLK_BIAS_ADJ:  /* 0x04 0x38 */
      {
         tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *msg =
            (tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *) message_structure;

         if ( *packet_length < ( 3 * SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for(i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8(msg[i].svid,            ptr);
               SIRFBINARY_EXPORT8(msg[i].toe,             ptr);
               SIRFBINARY_EXPORT8(msg[i].clk_bias_adjust, ptr);
            }
         }
      }
      break;
      /* added for GSW_CLM */
      case SIRF_MSG_SSB_EE_EPHEMERIS_AGE:     /* 0x11 0x38 */
      {
         tSIRF_MSG_EE_AGE *msg = (tSIRF_MSG_EE_AGE*) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week, ptr);   /* GPS week number in weeks */
            SIRFBINARY_EXPORT16(msg->gps_tow,  ptr);   /* GPS time of week in seconds */
            SIRFBINARY_EXPORT8 (msg->indicator, ptr);  /* 0 means age is valid */
            SIRFBINARY_EXPORT8 (msg->age, ptr);
            SIRFBINARY_EXPORT8 (msg->age_in_days, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_ACK:       /* 0xFF 0x38 */
      {
         tSIRF_MSG_SSB_EE_ACK *msg = (tSIRF_MSG_SSB_EE_ACK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->acq_id, ptr);
            SIRFBINARY_EXPORT8(msg->acq_sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA_7:     /* 0x07 0x3F */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_7 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_7*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->test_status, ptr);       /* TM7 status */
            SIRFBINARY_EXPORT32(msg->spur1_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur1_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur2_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur2_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur3_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur3_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur4_frequency, ptr);   /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur4_sig_to_noise, ptr);/* spur power */
         }
      }
      break;
      case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 */
      {
         tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*) message_structure;

         if ( *packet_length < ( 5 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->gps_tow, ptr);
            SIRFBINARY_EXPORT16(msg->gdop,    ptr);
            SIRFBINARY_EXPORT16(msg->pdop,    ptr);
            SIRFBINARY_EXPORT16(msg->hdop,    ptr);
            SIRFBINARY_EXPORT16(msg->vdop,    ptr);
            SIRFBINARY_EXPORT16(msg->tdop,    ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_INITIALIZE: /* 0x80 */
      {
         tSIRF_MSG_SSB_INITIALIZE * msg = (tSIRF_MSG_SSB_INITIALIZE*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,    ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT8 (msg->chnl_cnt,      ptr);
            SIRFBINARY_EXPORT8 (msg->restart_flags, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_NMEA_MODE: /* 0x81 */
      {
         tSIRF_MSG_SSB_SET_NMEA_MODE * msg = (tSIRF_MSG_SSB_SET_NMEA_MODE*) message_structure;

         if ( *packet_length < ( 2 * 10 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->mode,                ptr);
            for (i = 0; i < SIRF_MSG_SSB_MAX_NUM_NMEA_MSG; i++)
            {
               SIRFBINARY_EXPORT8(msg->nmea_cfg[i].rate,  ptr);
               SIRFBINARY_EXPORT8(msg->nmea_cfg[i].cksum, ptr);
            }
            SIRFBINARY_EXPORT16(msg->baud_rate,           ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_ALMANAC: /* 0x82 */
      {
         tSIRF_MSG_SSB_SET_ALMANAC * msg = (tSIRF_MSG_SSB_SET_ALMANAC*) message_structure;

         if ( *packet_length < ( SIRF_MAX_SVID_CNT * SIRF_MSG_SSB_ALMANAC_DATA_ENTRY * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for (i = 0; i < SIRF_MAX_SVID_CNT; i++)
            {
               for (j = 0; j < SIRF_MSG_SSB_ALMANAC_DATA_ENTRY; j++)
               {
                  SIRFBINARY_EXPORT16(msg->almanac[i].data[j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_SW_VERSION: /* 0x84 */
      {
         tSIRF_MSG_SSB_POLL_SW_VERSION * msg = (tSIRF_MSG_SSB_POLL_SW_VERSION*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_DGPS_SOURCE: /* 0x85 */
      {
         tSIRF_MSG_SSB_SET_DGPS_SOURCE * msg = (tSIRF_MSG_SSB_SET_DGPS_SOURCE*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->src,      ptr);
            SIRFBINARY_EXPORT32(msg->freq,     ptr);
            SIRFBINARY_EXPORT8 (msg->bit_rate, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_NAV_MODE: /* 0x88 */
      {
         tSIRF_MSG_SSB_SET_NAV_MODE* msg = (tSIRF_MSG_SSB_SET_NAV_MODE*) message_structure;

         if ( *packet_length < ( 11 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->enable_3d,          ptr);
            SIRFBINARY_EXPORT8 (msg->enable_con_alt,     ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,      ptr);
            SIRFBINARY_EXPORT8 (msg->pad,                ptr);
            SIRFBINARY_EXPORT8 (msg->dr_enable,          ptr);
            SIRFBINARY_EXPORT16(msg->alt_input,          ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,           ptr);
            SIRFBINARY_EXPORT8 (msg->alt_src,            ptr);
            SIRFBINARY_EXPORT8 (msg->coast_timeout,      ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_timeout,   ptr);
            SIRFBINARY_EXPORT8 (msg->dr_timeout,         ptr);
            SIRFBINARY_EXPORT8 (msg->trk_smooth,         ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_DOP_MODE: /* 0x89 */
      {
         tSIRF_MSG_SSB_SET_DOP_MODE * msg = (tSIRF_MSG_SSB_SET_DOP_MODE*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->mode,    ptr);
            SIRFBINARY_EXPORT8(msg->gdop_th, ptr);
            SIRFBINARY_EXPORT8(msg->pdop_th, ptr);
            SIRFBINARY_EXPORT8(msg->hdop_th, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SET_DGPS_MODE: /* 0x8A */
      {
         tSIRF_MSG_SSB_SET_DGPS_MODE *msg = (tSIRF_MSG_SSB_SET_DGPS_MODE*)message_structure;
         if (*packet_length < (2*sizeof(tSIRF_UINT8) + header_len))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->mode,    ptr);
            SIRFBINARY_EXPORT8(msg->timeout, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SET_ELEV_MASK: /* 0x8B */
      {
         tSIRF_MSG_SSB_SET_ELEV_MASK * msg = (tSIRF_MSG_SSB_SET_ELEV_MASK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->trk, ptr);
            SIRFBINARY_EXPORT16(msg->nav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_POWER_MASK: /* 0x8C */
      {
         tSIRF_MSG_SSB_SET_POWER_MASK * msg = (tSIRF_MSG_SSB_SET_POWER_MASK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->trk, ptr);
            SIRFBINARY_EXPORT8(msg->nav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_STAT_NAV: /* 0x8F */
      {
         tSIRF_MSG_SSB_SET_STAT_NAV* msg = (tSIRF_MSG_SSB_SET_STAT_NAV*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->staticnav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_CLOCK_STATUS: /* 0x90 */
      {
         tSIRF_MSG_SSB_POLL_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_POLL_CLOCK_STATUS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_ALMANAC: /* 0x92 */
      {
         tSIRF_MSG_SSB_POLL_ALMANAC * msg = (tSIRF_MSG_SSB_POLL_ALMANAC*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_EPHEMERIS: /* 0x93 */
      {
         tSIRF_MSG_SSB_POLL_EPHEMERIS * msg = (tSIRF_MSG_SSB_POLL_EPHEMERIS*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,     ptr);
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_EPHEMERIS: /* 0x95 */
      {
         tSIRF_MSG_SSB_SET_EPHEMERIS * msg = (tSIRF_MSG_SSB_SET_EPHEMERIS*) message_structure;

         if ( *packet_length < (3 * 15 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for (i = 0; i < 3; i++)
            {
               for (j = 0; j < 15; j++)
               {
                  SIRFBINARY_EXPORT16(msg->subframe[i][j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SET_OP_MODE: /* 0x96 */
      {
         tSIRF_MSG_SSB_SET_OP_MODE * msg = (tSIRF_MSG_SSB_SET_OP_MODE*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->mode,   ptr);
            SIRFBINARY_EXPORT16(msg->svid,   ptr);
            SIRFBINARY_EXPORT16(msg->period, ptr);
            SIRFBINARY_EXPORT8(msg->tm5Cmd, ptr );
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_RECEIVER_PARAMS: /* 0x98 */
      {
         tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS: /* 0x02A1 */
      {
         tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS *msg = (tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS*)message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16 ( msg->TaskID,           ptr );
            SIRFBINARY_EXPORT32 ( msg->PeriodOn,         ptr );
            SIRFBINARY_EXPORT32 ( msg->PeriodOff,        ptr );
            SIRFBINARY_EXPORT32 ( msg->LoadDuration,     ptr );
            SIRFBINARY_EXPORT32 ( msg->FullLoadDuration, ptr );
            SIRFBINARY_EXPORT32 ( msg->OffsetTime,       ptr );
         }
      }
      break;

      case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05 0xA1 */
      {
         tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->start_mode  , ptr);
            SIRFBINARY_EXPORT32 (msg->clock_offset, ptr);
            SIRFBINARY_EXPORT32 (msg->port_num    , ptr);
            SIRFBINARY_EXPORT32 (msg->baud_rate   , ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06 0xA1 */
      {
         tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->stop_mode  , ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 */
      {
         /* Nothing to do here, message has no data */
      }
      break;
      case SIRF_MSG_SSB_SET_MSG_RATE: /* 0xA6 */
      {
         tSIRF_MSG_SSB_SET_MSG_RATE * msg = (tSIRF_MSG_SSB_SET_MSG_RATE*) message_structure;

         if ( *packet_length < (7 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,         ptr);
            SIRFBINARY_EXPORT8 (msg->msg_id,       ptr);
            SIRFBINARY_EXPORT8 (msg->rate,         ptr);
            SIRFBINARY_EXPORT8 (msg->sub_id,       ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[0],  ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],  ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[2],  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_SBAS_PRN: /* 0xAA */
      {
         tSIRF_MSG_SSB_SET_SBAS_PRN * msg = (tSIRF_MSG_SSB_SET_SBAS_PRN*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->prn,         ptr);
            SIRFBINARY_EXPORT32(msg->reserved[0], ptr);
            SIRFBINARY_EXPORT32(msg->reserved[1], ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_NAV_INIT: /* 0x01 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_NAV_INIT * msg = (tSIRF_MSG_SSB_DR_SET_NAV_INIT*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->lat,            ptr);
            SIRFBINARY_EXPORT32(msg->lon,            ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,     ptr);
            SIRFBINARY_EXPORT16(msg->heading,        ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,     ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,        ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,       ptr);
            SIRFBINARY_EXPORT8 (msg->chnl_cnt,       ptr);
            SIRFBINARY_EXPORT8 (msg->restart_flags,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_NAV_MODE: /* 0x02 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_NAV_MODE * msg = (tSIRF_MSG_SSB_DR_SET_NAV_MODE*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,ptr);
            SIRFBINARY_EXPORT8( msg->reserved, ptr );
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL: /* 0x03 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->cal,        ptr);
            SIRFBINARY_EXPORT8 (msg->reserved,   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_SENS_PARAM: /* 0x04 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SET_SENS_PARAM*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->base_ssf,ptr);
            SIRFBINARY_EXPORT16(msg->base_gb,ptr);
            SIRFBINARY_EXPORT16(msg->base_gsf,ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_DR_POLL_VALID:        /* 0x05 0xAC */
      case SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL: /* 0x06 0xAC */
      case SIRF_MSG_SSB_DR_POLL_SENS_PARAM:   /* 0x07 0xAC */
         break;

      case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*) message_structure;

         if ( *packet_length < ( (2 + 2 * msg->num_of_data_sets) * sizeof(tSIRF_UINT8) +
                                 (1 + 5 * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                 msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->sensor_data_type, ptr);
            SIRFBINARY_EXPORT8 (msg->num_of_data_sets, ptr);
            SIRFBINARY_EXPORT16(msg->reverse_bitmap,   ptr);

            for (i = 0; i < (msg->num_of_data_sets); i++)
            {
               SIRFBINARY_EXPORT8 (msg->data[i].valid_sensor_indication,  ptr);
               SIRFBINARY_EXPORT32(msg->data[i].data_set_time_tag,        ptr);
               SIRFBINARY_EXPORT16(msg->data[i].odometer_speed,           ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data1,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data2,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data3,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data4,                    ptr);
               SIRFBINARY_EXPORT8 (msg->data[i].reserved,                 ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0A 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->mode, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0B 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->mode, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_MMF_DATA: /* 0x50 0xAC */
      {
         tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*) message_structure;

         if ( *packet_length < (  2 * sizeof(tSIRF_UINT8) +
                                 (3 * 3) * sizeof(tSIRF_UINT16) +
                                 (1 + 5 * 3) * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->gps_tow_reference, ptr);
            SIRFBINARY_EXPORT8 (msg->num_of_data_sets,  ptr);
            SIRFBINARY_EXPORT8 (msg->control,           ptr);
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT32(msg->mmf_data[i].lat,              ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].lon,              ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].horiz_pos_uncert, ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].alt_ellips,       ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].vert_pos_uncert,  ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].heading,          ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].heading_uncert,   ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].reserved,         ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51 0xAC */
      {
         tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->mode,                   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRF_INTERNAL:   /* 0xE1 */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;

      case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:   /* 0x01 0xE8 */
      case SIRF_MSG_SSB_EE_POLL_STATE:    /* 0x02 0xE8 */
      case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFE 0xE8 */
      case SIRF_MSG_SSB_EE_DEBUG:         /* 0xFF 0xE8 */
         break;

      case SIRF_MSG_SSB_TEXT:         /* 0xFF */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;
      case SIRF_MSG_SSB_TRKR_DBG:         /* 0x44 */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;

      default:
         /* 0x61 to 0x7F and 0xE1 */
         if (( message_id >= SIRF_MSG_SSB_PASSTHRU_OUTPUT_BEGIN && message_id <= SIRF_MSG_SSB_PASSTHRU_OUTPUT_END)
              || (SIRF_MSG_SSB_SIRF_INTERNAL_OUT == message_id))
         { /* pass-thru from tracker:*/
            if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               memcpy(ptr, message_structure, message_length);
               ptr += message_length;
            }
         }
         else
         {
            tRet = SIRF_CODEC_SSB_INVALID_MSG_ID;
         }
         break;

      } /* switch */

      if (SIRF_SUCCESS == tRet)
      {
         *packet_length = (tSIRF_UINT32)(ptr - packet);
         if ( *packet_length > SIRF_MSG_SSB_MAX_MESSAGE_LEN )
         {
            tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
         }
      }
   } /* end of process message */


   if ( tRet != SIRF_SUCCESS )
   {
      *packet_length = 0;
   }

   return tRet;

} /* SIRF_CODEC_SSB_Encode() */



/***************************************************************************
 * @brief:   Convert a byte stream to a formatted SSB structure
 * @param[in]:  payload - contains the byte stream, in protocol format
 * @param[in]   payload_length - the size of same, including overhead
 * @param[out]  message_id - SSB message id, derived from the input
 * @param[out]  message_structure - data structure keyed to the message id
 * @param[out]  message_length - pointer to the length of the message
 * @return:  SIRF_SUCCESS, SIRF_CODEC_SSB_NULL_POINTER, or
 *           SIRF_CODEC_SSB_LENGTH_ERROR
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_SSB_Decode( tSIRF_UINT8 *payload,
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID   *message_structure,
                                    tSIRF_UINT32 *message_length ) {
   return SIRF_CODEC_SSB_Decode_Ex(payload, payload_length, 0, message_id,
	 message_structure, message_length);
}

/***************************************************************************
 * @brief:   Convert a byte stream to a formatted SSB structure
 * @param[in]:  payload - contains the byte stream, in protocol format
 * @param[in]   payload_length - the size of same, including overhead
 * @param[in]   payload_length - the size of same, including overhead
 * @param[in]   sirf_flags - SIRF_CODEC_FLAGS_GSW230_BYTE_ORDER for GSW2.3 - 2.99
 * @param[out]  message_id - SSB message id, derived from the input
 * @param[out]  message_structure - data structure keyed to the message id
 * @param[out]  message_length - pointer to the length of the message
 * @return:  SIRF_SUCCESS, SIRF_CODEC_SSB_NULL_POINTER, or
 *           SIRF_CODEC_SSB_LENGTH_ERROR
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_SSB_Decode_Ex( tSIRF_UINT8 *payload,
                                    tSIRF_UINT32 payload_length,
				    tSIRF_UINT32 sirf_flags,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID   *message_structure,
                                    tSIRF_UINT32 *message_length
				    )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   if ( ((tSIRF_UINT8 *) NULL !=  payload) &&
        ((tSIRF_UINT32 *) NULL != message_id) &&
        ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_UINT32 *) NULL != message_length) )
   {
      tSIRF_UINT8 *ptr = payload;
      tSIRF_UINT32 i, j;
      tSIRF_UINT8 mid, sid = 0;
      tSIRF_UINT8 header_len = 1 * sizeof(tSIRF_UINT8); /* At least the mid byte */

      *message_id = 0;

      /* At a minimum, make sure we have room for the first header byte */
      if ( payload_length < header_len )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      mid = SIRFBINARY_IMPORT_UINT8(ptr);
      *message_id     = SIRF_MAKE_MSG_ID(SIRF_LC_SSB,mid,sid);
      *message_length = 0;

      /*   add the sub-id to the message id */
      if (
         SIRF_MSG_SSB_SIRFNAV_COMMAND      == *message_id ||
         SIRF_MSG_SSB_ADVANCED_NAV_INIT    == *message_id ||
         SIRF_MSG_SSB_SW_CONTROL           == *message_id ||
         SIRF_MSG_SSB_EE_INPUT             == *message_id ||
         SIRF_MSG_SSB_DR_OUTPUT            == *message_id ||
         SIRF_MSG_SSB_SIRFNAV_NOTIFICATION == *message_id ||
         SIRF_MSG_SSB_EVENT                == *message_id ||
         SIRF_MSG_SSB_EE                   == *message_id ||
         SIRF_MSG_SSB_NL_AUX_DATA          == *message_id ||
         SIRF_MSG_SSB_TRACKER_DATA         == *message_id
         )
      {
         header_len += 1 * sizeof(tSIRF_UINT8);
         if ( payload_length < header_len )
         {
            return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         sid = SIRFBINARY_IMPORT_UINT8(ptr);
      }

      *message_id = SIRF_MAKE_MSG_ID(SIRF_LC_SSB,mid,sid);
      /* be aware that this is now a 16-bit value */

      switch (*message_id)
      {
         case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 */
         {   /* coercive cast the output to the format we want */
            tSIRF_MSG_SSB_MEASURED_NAVIGATION * msg = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*) message_structure;

            if ( payload_length < ( (4 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_UINT16) +
                                     4 * sizeof(tSIRF_UINT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_y         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_z         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_vel_x     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ecef_vel_y     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ecef_vel_z     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->nav_mode       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->nav_mode2      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->gps_week       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sv_used_cnt    = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->sv_used[i]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MEASURED_TRACKER: /* 0x04 */
         {
            tSIRF_MSG_SSB_MEASURED_TRACKER * msg = (tSIRF_MSG_SSB_MEASURED_TRACKER*) message_structure;

            if ( payload_length < ( (1 + 3 * SIRF_NUM_CHANNELS + SIRF_NUM_CHANNELS * SIRF_NUM_POINTS) *
                                                                                     sizeof(tSIRF_UINT8) +
                                    (1 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT16) +
                                     1 * sizeof(tSIRF_UINT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_week             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->chnl_cnt             = SIRFBINARY_IMPORT_UINT8( ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->chnl[i].svid      = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].azimuth   = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].elevation = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].state     = SIRFBINARY_IMPORT_UINT16(ptr);
                  for(j = 0; j < SIRF_NUM_POINTS; j++)
                  {
                     msg->chnl[i].cno[j] = SIRFBINARY_IMPORT_UINT8( ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 */
         {
            tSIRF_MSG_SSB_RAW_TRACKER * msg = (tSIRF_MSG_SSB_RAW_TRACKER*) message_structure;

            if ( payload_length < ( (2 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                     7 * sizeof(tSIRF_INT16) +
                                     6 * sizeof(tSIRF_INT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->channel              = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->svid                 = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->state                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->bit_number           = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->msec_number          = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->chip_number          = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->code_phase           = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->carrier_doppler      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->measure_timetag      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->delta_carrier_phase  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->search_cnt           = SIRFBINARY_IMPORT_SINT16(ptr);
               for (i = 0; i < SIRF_NUM_POINTS; i++)
               {
                  msg->cno[i]            = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->power_bad_count      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->phase_bad_count      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->delta_car_interval   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->correl_interval      = SIRFBINARY_IMPORT_SINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SW_VERSION: /* 0x06 */
         {
            *message_length = payload_length - 1;
            memcpy(message_structure, ptr, payload_length - 1);
            *((char*) message_structure + payload_length - 1) = '\0';
            ptr += payload_length - 1;
         }
         break;
         case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 */
         {
            tSIRF_MSG_SSB_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_CLOCK_STATUS*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    4 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sv_used_cnt   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->clk_offset    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clk_bias      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->est_gps_time  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_50BPS_DATA:          /* 0x08 */
         case SIRF_MSG_SSB_50BPS_DATA_VERIFIED: /* 0x52 */
         {
            tSIRF_MSG_SSB_50BPS_DATA * msg = (tSIRF_MSG_SSB_50BPS_DATA*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->chnl       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->svid       = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i <10; i++)
               {
                  msg->word[i] = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 */
         {
            tSIRF_MSG_SSB_THROUGHPUT * msg = (tSIRF_MSG_SSB_THROUGHPUT*) message_structure;

            if ( payload_length < (  4 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->seg_stat_max = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->seg_stat_lat = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->avg_trk_time = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->last_ms      = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_ERROR: /* 0x0A */
         {
            tSIRF_MSG_SSB_ERROR * msg = (tSIRF_MSG_SSB_ERROR*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    SIRF_MSG_SSB_MAX_ERROR_PARAMS * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->err_id     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->param_cnt  = SIRFBINARY_IMPORT_UINT16(ptr);
               if (msg->param_cnt > SIRF_MSG_SSB_MAX_ERROR_PARAMS)
               {
                  msg->param_cnt = SIRF_MSG_SSB_MAX_ERROR_PARAMS;
               }
               for (i = 0; i < SIRF_MSG_SSB_MAX_ERROR_PARAMS; i++)
               {
                  msg->param[i] = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_ACK: /* 0x0B */
         {
            tSIRF_MSG_SSB_ACK * msg = (tSIRF_MSG_SSB_ACK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->msg_id = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sub_id = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NAK: /* 0x0C */
         {
            tSIRF_MSG_SSB_NAK * msg = (tSIRF_MSG_SSB_NAK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->msg_id = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sub_id = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D */
         {
            tSIRF_MSG_SSB_VISIBILITY_LIST * msg = (tSIRF_MSG_SSB_VISIBILITY_LIST*) message_structure;

            if ( payload_length < ( (1 + msg->svid_cnt) * sizeof(tSIRF_UINT8) +
                                    (2 * msg->svid_cnt) * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid_cnt                = SIRFBINARY_IMPORT_UINT8 (ptr);
               if (msg->svid_cnt > SIRF_MAX_SVID_CNT)
               {
                  msg->svid_cnt = SIRF_MAX_SVID_CNT;
               }
               for (i = 0; i < msg->svid_cnt; i++)
               {
                  msg->visible[i].svid      = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->visible[i].azimuth   = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->visible[i].elevation = SIRFBINARY_IMPORT_SINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_ALMANAC: /* 0x0E */
         {
            tSIRF_MSG_SSB_ALMANAC * msg = (tSIRF_MSG_SSB_ALMANAC*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) +
                                  (2 + SIRF_MSG_SSB_ALMANAC_ENTRIES) * sizeof(tSIRF_UINT16) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid            = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->week_and_status = SIRFBINARY_IMPORT_UINT16 (ptr);
               for (i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++)
               {
                  msg->data[i] = SIRFBINARY_IMPORT_UINT16(ptr);
               }
               msg->almanac_checksum = SIRFBINARY_IMPORT_UINT16 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F */
         {
            tSIRF_MSG_SSB_EPHEMERIS * msg = (tSIRF_MSG_SSB_EPHEMERIS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) +
                                   3 * 15 * sizeof(tSIRF_UINT16) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < 3; i++)
               {
                  for (j = 0; j < 15; j++)
                  {
                     msg->subframe[i][j] = SIRFBINARY_IMPORT_UINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 */
         {
            tSIRF_MSG_SSB_OK_TO_SEND * msg = (tSIRF_MSG_SSB_OK_TO_SEND*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->input_enabled = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 */
         {
            tSIRF_MSG_SSB_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_RECEIVER_PARAMS*) message_structure;

            if ( payload_length < (19 * sizeof(tSIRF_UINT8) +
                                   4 * sizeof(tSIRF_UINT16) +
                                   9 * sizeof(tSIRF_UINT32) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved_1             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->alt_mode               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_src                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_input              = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->degraded_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_timeout             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->trk_smooth             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->static_nav_mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->enable_3sv_lsq         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_2             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dop_mask_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->nav_elev_mask          = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_pwr_mask           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_3             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dgps_src               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_mode              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_timeout           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_4             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_push_to_fix         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_on_time             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_interval            = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_user_tasks_enabled  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_user_task_interval  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_pwr_cycling_enabled = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_max_acq_time        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_max_off_time        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->apm_enabled_power_duty_cycle      =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->number_of_fixes                   =SIRFBINARY_IMPORT_UINT16(ptr);
               msg->time_between_fixes                =SIRFBINARY_IMPORT_UINT16(ptr);
               msg->horz_vert_error_max               =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->response_time_max                 =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->time_accu_time_duty_cycle_priority=SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA * msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*) message_structure;

            if ( payload_length < (13 * sizeof(tSIRF_UINT16) +
                                   6 * sizeof(tSIRF_UINT32) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid =                SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period =              SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_synch_time =      SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_count =           SIRFBINARY_IMPORT_UINT16(ptr);
               msg->poor_status_count =   SIRFBINARY_IMPORT_UINT16(ptr);
               msg->good_status_count =   SIRFBINARY_IMPORT_UINT16(ptr);
               msg->parity_error_count =  SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lost_vco_count =      SIRFBINARY_IMPORT_UINT16(ptr);
               msg->frame_synch_time =    SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cno_mean =            SIRFBINARY_IMPORT_SINT16(ptr);
               msg->cno_sigma =           SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_drift =         SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_offset_mean =   SIRFBINARY_IMPORT_SINT32(ptr);
               msg->bad_1khz_bit_count =  SIRFBINARY_IMPORT_SINT16(ptr);
               msg->abs_i20ms =           SIRFBINARY_IMPORT_SINT32(ptr);
               msg->abs_q1ms =            SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[0] =         SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[1] =         SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[2] =         SIRFBINARY_IMPORT_SINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B */
         {
            tSIRF_MSG_SSB_DGPS_STATUS * msg = (tSIRF_MSG_SSB_DGPS_STATUS*) message_structure;

            if ( payload_length < ( (3 + 2 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                     SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->src = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->cor_age[i] = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->reserved[0] = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved[1] = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->corrections[i].prn = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->corrections[i].cor = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C */
         {
            tSIRF_MSG_SSB_NL_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_MEAS_DATA*) message_structure;

            if ( payload_length < ( (5 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_UINT16) +
                                     1 * sizeof(tSIRF_UINT32) +
                                     1 * sizeof(tSIRF_FLOAT) +
                                     3 * sizeof(tSIRF_DOUBLE) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->Chnl                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->Timetag               = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->svid                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE_EX(msg->gps_sw_time,            ptr, sirf_flags);
               SIRFBINARY_IMPORT_DOUBLE_EX(msg->pseudorange,            ptr, sirf_flags);
               SIRFBINARY_IMPORT_FLOAT (msg->carrier_freq,           ptr);
               SIRFBINARY_IMPORT_DOUBLE_EX(msg->carrier_phase,          ptr, sirf_flags);
               msg->time_in_track         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sync_flags            = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_POINTS; i++)
               {
                  msg->cton[i]            = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->delta_range_interval  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mean_delta_range_time = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->extrapolation_time    = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->phase_error_count     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->low_power_count       = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D */
         {
            tSIRF_MSG_SSB_NL_DGPS_DATA * msg = (tSIRF_MSG_SSB_NL_DGPS_DATA*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_FLOAT) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->iod            = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->source         = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->pr_correction,   ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prr_correction,  ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->correction_age,  ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prc_variance,    ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prrc_variance,   ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E */
         {
            tSIRF_MSG_SSB_NL_SV_STATE_DATA * msg = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*) message_structure;

            if ( payload_length < (  2 * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_FLOAT) +
                                    (2 + 3 + 3) * sizeof(tSIRF_DOUBLE) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid       = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_DOUBLE_EX (msg->time,       ptr, sirf_flags);
               for (i = 0; i < 3; i++)
               {
                  SIRFBINARY_IMPORT_DOUBLE_EX(msg->pos[i],  ptr, sirf_flags);
               }
               for (i = 0; i < 3; i++)
               {
                  SIRFBINARY_IMPORT_DOUBLE_EX(msg->vel[i],  ptr, sirf_flags);
               }
               SIRFBINARY_IMPORT_DOUBLE_EX(msg->clk,        ptr, sirf_flags);
               SIRFBINARY_IMPORT_FLOAT (msg->clf,        ptr);
               msg->eph        = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->posvar,     ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->clkvar,     ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->iono,       ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F */
         {
            tSIRF_MSG_SSB_NL_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_INIT_DATA*) message_structure;

            if ( payload_length < ( 13 * sizeof(tSIRF_UINT8) +
                                    13 * sizeof(tSIRF_INT16) +
                                    1 * sizeof(tSIRF_FLOAT) +
                                    5 * sizeof(tSIRF_DOUBLE) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->enable_con_alt_mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_mode                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_source                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->altitude,                    ptr);
               msg->degraded_mode              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->dr_timeout                 = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->coast_timeout              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->tracksmooth_mode           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dop_selection              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->gdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->pdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->dgps_selection             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_timeout               = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->elev_nav_mask              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->pow_nav_mask               = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->editing_residual_mode      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->editing_residual_threshold = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ssd_mode                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->ssd_threshold              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->static_nav_mode            = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->static_nav_threshold       = SIRFBINARY_IMPORT_SINT16(ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_x,                      ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_y,                      ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_z,                      ptr);
               msg->position_init_source       = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->gps_time,                    ptr);
               msg->gps_week                   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->time_init_source           = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->clk_offset,                  ptr);
               msg->clk_offset_init_source     = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_INIT_DATA: /* 0x40 0x01 */
         {
            tSIRF_MSG_SSB_NL_AUX_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8)  +
                                    4 * sizeof(tSIRF_UINT16) +
                                    6 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->time_init_unc        = SIRFBINARY_IMPORT_UINT32 (ptr);   
               msg->saved_pos_week       = SIRFBINARY_IMPORT_UINT16 (ptr);  
               msg->saved_pos_tow        = SIRFBINARY_IMPORT_UINT32 (ptr);   
               msg->saved_pos_ehe        = SIRFBINARY_IMPORT_UINT16 (ptr);   
               msg->saved_pos_eve        = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sw_version           = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->icd_version          = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->chip_version         = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->acq_clk_speed        = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->default_clock_offset = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->tracker_status       = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->reserved             = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_MEAS_DATA: /* 0x40 0x02 */
         {
            tSIRF_MSG_SSB_NL_AUX_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA*) message_structure;

            if ( payload_length < (  5 * sizeof(tSIRF_UINT8)  +
                                     8 * sizeof(tSIRF_INT16)  +
                                     2 * sizeof(tSIRF_UINT16) +
                                     7 * sizeof(tSIRF_INT32)  +
                                     3 * sizeof(tSIRF_UINT32) +
                                     header_len) )

            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->sv_prn              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->status              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->extended_status     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->bit_sync_qual       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->time_tag            = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->code_phase          = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->carrier_phase       = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->carrier_freq        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->carrier_accel       = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->ms_num              = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->bit_num             = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->code_correction     = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->smooth_code         = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->code_offset         = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->pseudorange_noise   = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->delta_range_qual    = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->phase_lock_qual     = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->ms_uncertainty      = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->sum_abs_I           = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sum_abs_Q           = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sv_bit_num          = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->mp_los_det_value    = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->mp_only_det_value   = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->recovery_status     = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->sw_time_uncertainty = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_AID_DATA: /* 0x40 0x03 */
         {
            tSIRF_MSG_SSB_NL_AUX_AID_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_AID_DATA*) message_structure;

            if ( payload_length < ( 3 * sizeof(tSIRF_INT32)  +
                                    2 * sizeof(tSIRF_UINT32) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->ecef_y        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->ecef_z        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->horiz_pos_unc = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->alt_unc       = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sw_tow        = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 */
         {
            tSIRF_MSG_SSB_GEODETIC_NAVIGATION* msg = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*) message_structure;

            if ( payload_length < ( 8 * sizeof(tSIRF_UINT8) +
                                    13 * sizeof(tSIRF_UINT16) +
                                    14 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->nav_valid                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_mode                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_week                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->utc_year                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->utc_month                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_day                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_hour                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_min                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_sec                   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sv_used                   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lat                       = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon                       = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips                = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_msl                   = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->datum                     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sog                       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hdg                       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mag_var                   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->climb_rate                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->heading_rate              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ehpe                      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->evpe                      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ete                       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ehve                      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_bias                  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_bias_error            = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clk_offset                = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_offset_error          = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->distance_travelled        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->distance_travelled_error  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->heading_error             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sv_used_cnt               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop                      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->additional_mode_info      = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D */
         {
            tSIRF_UINT8 len = header_len + 1 * sizeof(tSIRF_UINT8) + 3 * sizeof(tSIRF_UINT16) + 1 * sizeof(tSIRF_UINT32);
            tSIRF_UINT8 len_1hz = header_len +
                                  10 * (1 * sizeof(tSIRF_UINT8) + 3 * sizeof(tSIRF_UINT16) + 1 * sizeof(tSIRF_UINT32));

            if (len == payload_length)
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*) message_structure;

               *message_length = sizeof(*msg);
               msg->current_time = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->adc2_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->adc3_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->odo_count    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gpio_stat    = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
            else if (len_1hz == payload_length)
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*) message_structure;

               *message_length = sizeof(*msg);
               for (i = 0; i < 10; i++)
               {
                  msg->dataset[i].current_time = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->dataset[i].adc2_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->dataset[i].adc3_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->dataset[i].odo_count    = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->dataset[i].gpio_stat    = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
            else
            {
               tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
            }
         }
         break;
         case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3 *)message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    16 * sizeof(tSIRF_UINT16) +
                                    4 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period                = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_synch_time        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_count             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->poor_status_count     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->good_status_count     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->parity_error_count    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lost_vco_count        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->frame_synch_time      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cno_mean              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->cno_sigma             = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_drift           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_offset          = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->bad_1khz_bit_count    = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->abs_i20ms             = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->abs_q20ms             = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->phase_lock            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->rtc_frequency         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->e_to_acq_ratio        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->t_sync_agc_gain       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->tm_5_ready            = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->ClkDriftUnc           = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x01 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*) message_structure;

            if ( payload_length < ( 14 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->nav = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->data = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cal_gb_cal = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gsf_cal_ssf_cal = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_across_reset_pos = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gyr_sub_op_spd_sub_op = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_st_int_ran_z_gb_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_upd_pos = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_upd_hd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_pos_gps_vel = SIRFBINARY_IMPORT_UINT8(ptr);

               /* Where did these fields come from? */
               msg->dws_hd_rt_s_f_cal_valid = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dws_hd_rt_s_f_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dws_spd_s_f_cal_valid = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dws_spd_s_f_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
            }

         }
         break;
         case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x02 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*) message_structure;

            if ( payload_length < ( 3 * sizeof(tSIRF_UINT8) +
                                    33 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ssf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ssf_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gb = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gbE = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gsf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gsf_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tpe = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->the = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_ctrl = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reverse = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hd = SIRFBINARY_IMPORT_UINT16(ptr);

               /* where did these fields come from */
               msg->sensor_pkg = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->odo_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->odo_spd_s_f = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->odo_spd_s_f_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lf_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lf_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rf_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rf_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lr_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lr_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rr_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rr_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_spd_delta = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_avg_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_spd_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_hd_rt = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_hd_rt_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_spd_delta = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_avg_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_spd_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_hd_rt = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_hd_rt_err = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x03 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    12 * sizeof(tSIRF_UINT16) +
                                    2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_pos_e = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dr_hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_pos_e = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved[0]   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved[1]   = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_VALID: /* 0x05 0x30 */
         {
            tSIRF_MSG_SSB_DR_VALID * msg = (tSIRF_MSG_SSB_DR_VALID*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->valid    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_GYR_FACT_CAL: /* 0x06 0x30 */
         {
            tSIRF_MSG_SSB_DR_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_GYR_FACT_CAL*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->cal       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved  = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x07 0x30 */
         {
            tSIRF_MSG_SSB_DR_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->base_ssf = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->base_gb = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->base_gsf = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x08 0x30 */
         {
            tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    (1 + 2 * 10) * sizeof(tSIRF_UINT16) +
                                    10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->meas_type = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->valid_cnt = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->bkup_flgs = SIRFBINARY_IMPORT_UINT16(ptr);
               for (i = 0; i < 10; i++)
               {
                  msg->blk[i].tag    = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->blk[i].spd    = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->blk[i].hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MMF_STATUS: /* 0x50 0x30 */
         {
            tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    9 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->status   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sys_hdg  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sys_lat  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_lon  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_alt  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_tow  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->mmf_hdg  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mmf_lat  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_lon  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_alt  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_tow  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 */
         {
            tSIRF_MSG_SSB_SBAS_PARAM * msg = (tSIRF_MSG_SSB_SBAS_PARAM*) message_structure;

            if ( payload_length < (12 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->prn       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->mode      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->timeout   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->flg_bits  = SIRFBINARY_IMPORT_UINT8 (ptr);
	       ptr += 8;
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x01 0x33 */
         {
            tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved   = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x04 0x33 */
         {
            tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*) message_structure;

            if ( payload_length < ( 6 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    3 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->tracker_time_tag              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week                      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->tracker_rtc_day               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_hour              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_min               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_sec               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_clkctr            = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tracker_rtc_ok                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_rollover_complete = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_rollover_seconds  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
         {
            tSIRF_MSG_SSB_SIRFNAV_START* msg = (tSIRF_MSG_SSB_SIRFNAV_START*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->start_mode   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clock_offset = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->port_num     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->baud_rate    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (51, 8) */
         {
            tSIRF_MSG_SSB_SIRFNAV_STOP* msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length   = sizeof(*msg);
               msg->stop_mode = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_STARTUP_INFO: /* 0x0136 */
         {
            tSIRF_MSG_SSB_STARTUP_INFO * msg = (tSIRF_MSG_SSB_STARTUP_INFO*) message_structure;

            if ( payload_length < ( 7 * sizeof(tSIRF_UINT8) +
                                    6 * sizeof(tSIRF_UINT16) +
                                    9 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode                     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->status                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_status                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->gps_off_time             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->utc_year                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->utc_month                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_day                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_hour                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_min                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_sec                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lat                      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon                      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips               = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->heading                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->valid_ephemeris          = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->collected_almanac        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->collected_almanac_week   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->factory_almanac_week     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_offset               = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved                 = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_EE_GPS_TIME_INFO: /* 0x0138 */
         case SIRF_MSG_SSB_EE_INTEGRITY:     /* 0x0238 */
         case SIRF_MSG_SSB_EE_STATE:         /* 0x0338 */
         case SIRF_MSG_SSB_EE_CLK_BIAS_ADJ:  /* 0x0438 */
         case SIRF_MSG_SSB_EE_ACK:           /* 0xFF38 */
         break;

         case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 */
         {
            tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*) message_structure;

            if ( payload_length < ( 5 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_tow = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->vdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tdop    = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_INITIALIZE: /* 0x80 */
         {
            tSIRF_MSG_SSB_INITIALIZE * msg = (tSIRF_MSG_SSB_INITIALIZE*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_y        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_z        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_offset    = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->gps_tow       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->chnl_cnt      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->restart_flags = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_NMEA_MODE: /* 0x81 */
         {
            tSIRF_MSG_SSB_SET_NMEA_MODE * msg = (tSIRF_MSG_SSB_SET_NMEA_MODE*) message_structure;

            if ( payload_length < ( (1 + 2 * 10) * sizeof(tSIRF_UINT8) +
                                     1 * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->mode                 = SIRFBINARY_IMPORT_UINT8  (ptr);
               for (i = 0; i <SIRF_MSG_SSB_MAX_NUM_NMEA_MSG; i++)
               {
                  msg->nmea_cfg[i].rate  = SIRFBINARY_IMPORT_UINT8  (ptr);
                  msg->nmea_cfg[i].cksum = SIRFBINARY_IMPORT_UINT8  (ptr);
               }
               msg->baud_rate            = SIRFBINARY_IMPORT_UINT16 (ptr);
               *message_length = sizeof(*msg);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_ALMANAC: /* 0x82 */
         {
            tSIRF_MSG_SSB_SET_ALMANAC * msg = (tSIRF_MSG_SSB_SET_ALMANAC*) message_structure;

            if ( payload_length < ( SIRF_MAX_SVID_CNT * SIRF_MSG_SSB_ALMANAC_DATA_ENTRY * sizeof(tSIRF_INT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               for (i = 0; i < SIRF_MAX_SVID_CNT; i++)
               {
                  for (j = 0; j < SIRF_MSG_SSB_ALMANAC_DATA_ENTRY; j++)
                  {
                     msg->almanac[i].data[j] = SIRFBINARY_IMPORT_SINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_SW_VERSION: /* 0x84 */
         {
            tSIRF_MSG_SSB_POLL_SW_VERSION * msg = (tSIRF_MSG_SSB_POLL_SW_VERSION*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_DGPS_SOURCE: /* 0x85 */
         {
            tSIRF_MSG_SSB_SET_DGPS_SOURCE * msg = (tSIRF_MSG_SSB_SET_DGPS_SOURCE*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->src      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->freq     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->bit_rate = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_NAV_MODE: /* 0x88 */
         {
            tSIRF_MSG_SSB_SET_NAV_MODE* msg = (tSIRF_MSG_SSB_SET_NAV_MODE*) message_structure;

            if ( payload_length < ( 11 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_INT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->enable_3d         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->enable_con_alt    = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_mode     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->pad               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_enable         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_input         = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->alt_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_src           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->coast_timeout     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_timeout        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->trk_smooth        = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_DOP_MODE: /* 0x89 */
         {
            tSIRF_MSG_SSB_SET_DOP_MODE* msg = (tSIRF_MSG_SSB_SET_DOP_MODE*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->pdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_DGPS_MODE: /* 0x8A */
         {
            tSIRF_MSG_SSB_SET_DGPS_MODE *msg = (tSIRF_MSG_SSB_SET_DGPS_MODE*)message_structure;
            if (payload_length < (2*sizeof(tSIRF_UINT8) + header_len))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->timeout = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_ELEV_MASK: /* 0x8B */
         {
            tSIRF_MSG_SSB_SET_ELEV_MASK* msg = (tSIRF_MSG_SSB_SET_ELEV_MASK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_INT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->trk     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->nav     = SIRFBINARY_IMPORT_SINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_POWER_MASK: /* 0x8C */
         {
            tSIRF_MSG_SSB_SET_POWER_MASK* msg = (tSIRF_MSG_SSB_SET_POWER_MASK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->trk     = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav     = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_STAT_NAV: /* 0x8F */
         {
            tSIRF_MSG_SSB_SET_STAT_NAV* msg = (tSIRF_MSG_SSB_SET_STAT_NAV*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->staticnav = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_CLOCK_STATUS: /* 0x90 */
         {
            tSIRF_MSG_SSB_POLL_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_POLL_CLOCK_STATUS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_ALMANAC: /* 0x92 */
         {
            tSIRF_MSG_SSB_POLL_ALMANAC * msg = (tSIRF_MSG_SSB_POLL_ALMANAC*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_EPHEMERIS: /* 0x93 */
         {
            tSIRF_MSG_SSB_POLL_EPHEMERIS * msg = (tSIRF_MSG_SSB_POLL_EPHEMERIS*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid     = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_EPHEMERIS: /* 0x95 */
         {
            tSIRF_MSG_SSB_SET_EPHEMERIS * msg = (tSIRF_MSG_SSB_SET_EPHEMERIS*) message_structure;

            if ( payload_length < (3 * 15 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               for (i = 0; i < 3; i++)
               {
                  for (j = 0; j < 15; j++)
                  {
                     msg->subframe[i][j] = SIRFBINARY_IMPORT_UINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SET_OP_MODE: /* 0x96 */
         {
            tSIRF_MSG_SSB_SET_OP_MODE * msg = (tSIRF_MSG_SSB_SET_OP_MODE*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->svid   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tm5Cmd = SIRFBINARY_IMPORT_UINT8( ptr );
            }
         }
         break;
         case SIRF_MSG_SSB_SET_LOW_POWER: /* 0x97 */
         {
            tSIRF_MSG_SSB_SET_LOW_POWER* msg = (tSIRF_MSG_SSB_SET_LOW_POWER*)  message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length  = sizeof(*msg);
               msg->push_to_fix = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->duty_cycle  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->on_time     = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_RECEIVER_PARAMS: /* 0x98 */
         {
            tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DEMO_SET_RESTART_MODE: /* 0x01A1 */
         {
            tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE* msg = (tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->control = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS: /* 0x02A1 */
         {
            tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS* msg = (tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->TaskID           = SIRFBINARY_IMPORT_UINT16( ptr );
               msg->PeriodOn         = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->PeriodOff        = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->LoadDuration     = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->FullLoadDuration = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->OffsetTime       = SIRFBINARY_IMPORT_UINT32( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_STOP_TEST_APP: /* 0x03A1 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DEMO_STOP_TEST_APP) );
            *message_length = sizeof(tSIRF_MSG_SSB_DEMO_STOP_TEST_APP);
         }
         break;

         case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05A1 */
         {
            tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE* msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->start_mode   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clock_offset = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->port_num     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->baud_rate    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06A1 */
         {
            tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE* msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length   = sizeof(*msg);
               msg->stop_mode = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_SIRFNAV_STORE_NOW) );
            *message_length = sizeof(tSIRF_MSG_SSB_SIRFNAV_STORE_NOW);
         }
         break;

         case SIRF_MSG_SSB_SET_MSG_RATE: /* 0xA6 */
         {
            tSIRF_MSG_SSB_SET_MSG_RATE * msg = (tSIRF_MSG_SSB_SET_MSG_RATE*) message_structure;

            if ( payload_length < (7 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode        = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->msg_id      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->rate        = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->sub_id      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[2] = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_LOW_POWER_PARAMS:  /* 0xA7 */
         {
            tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS* msg = (tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS*)  message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    3 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length  = sizeof(*msg);
               msg->MaxOffTime             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->MaxAcqTime             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->push_to_fix_period     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->adaptive_tp            = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_SBAS_PRN: /* 0xAA */
         {
            tSIRF_MSG_SSB_SET_SBAS_PRN * msg = (tSIRF_MSG_SSB_SET_SBAS_PRN*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->prn         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved[0] = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved[1] = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

        case SIRF_MSG_SSB_ADVANCED_NAV_INIT: /**  0x00AC  */
        {
            tSIRF_MSG_SSB_ADVANCED_NAV_INIT * msg = (tSIRF_MSG_SSB_ADVANCED_NAV_INIT*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    8 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               *(tSIRF_UINT32*)&msg->control = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->timeSkew   = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->timeUncert = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->pos[0]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos[1]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos[2]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->horzUncert = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->vertUncert = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clkSkew    = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clkUncert  = SIRFBINARY_IMPORT_UINT32(ptr);
               ptr += 6; /** message has 6 reserved bytes at end */
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_NAV_INIT:  /* 0x01 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_NAV_INIT * msg = (tSIRF_MSG_SSB_DR_SET_NAV_INIT*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->lat            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->heading        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_offset     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->gps_tow        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->chnl_cnt       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->restart_flags  = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_NAV_MODE: /* 0x02 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_NAV_MODE * msg = (tSIRF_MSG_SSB_DR_SET_NAV_MODE*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved   = SIRFBINARY_IMPORT_UINT8( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL: /* 0x03 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->cal         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved    = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_SENS_PARAM: /* 0x04 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SET_SENS_PARAM*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->base_ssf = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->base_gb  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->base_gsf = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_SENS_PARAM:   /* 0x07 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_SENS_PARAM) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_SENS_PARAM);
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL: /* 0x06 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL);
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_VALID:        /* 0x05 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_VALID) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_VALID);
         }
         break;

         case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*) message_structure;

            *message_length = sizeof(*msg);
            msg->sensor_data_type  = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->num_of_data_sets  = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->reverse_bitmap    = SIRFBINARY_IMPORT_UINT16(ptr);

            if ( payload_length < ( (2 + 2 * msg->num_of_data_sets) * sizeof(tSIRF_UINT8) +
                                    (1 + 5 * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                    msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for(i = 0; i <(msg->num_of_data_sets); i++)
               {
                  msg->data[i].valid_sensor_indication   = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->data[i].data_set_time_tag         = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->data[i].odometer_speed            = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->data[i].data1                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data2                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data3                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data4                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].reserved                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
         }
         break;

         case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0A 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length              = sizeof(*msg);
               msg->mode                    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0B 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length              = sizeof(*msg);
               msg->mode                    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_MMF_DATA: /* 0x50 0xAC */
         {
            tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*) message_structure;

            if ( payload_length < (  2 * sizeof(tSIRF_UINT8) +
                                    (3 * 3) * sizeof(tSIRF_UINT16) +
                                    (1 + 3 * 4) * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_tow_reference = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->num_of_data_sets  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->control           = SIRFBINARY_IMPORT_UINT8 (ptr);
               for(i = 0; i <3; i++)
               {
                  msg->mmf_data[i].lat              = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].lon              = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].horiz_pos_uncert = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->mmf_data[i].alt_ellips       = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].vert_pos_uncert  = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->mmf_data[i].heading          = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->mmf_data[i].heading_uncert   = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->mmf_data[i].reserved         = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51 0xAC */
         {
            tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->mode              = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_TRK_HW_CONFIG:   /* 0xCE */
         {
            tSIRF_MSG_SSB_TRK_HW_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_CONFIG*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->ExtLNAEnable      = SIRFBINARY_IMPORT_UINT8(ptr);  /* External LNA Enable 0=Ignore, 1=Internal, 2=External */
               /* Advancing the data pointer to make the size reconcile correctly, since this data won't be */
               /* utilized. DO NOT allow SSB to set the Reserved fields, since this is responsibility of    */
               /* the Host. Customers are NOT allowed to modify these Reserved fields.                      */
               ptr++;
               msg->Reserved = 0;
               for (i = 0; i < SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH; i++)
               {
                    ptr++;
                    msg->Reserved1[i] = 0;
               }
            }
         }
         break;

         case SIRF_MSG_SSB_SW_COMMANDED_OFF: /* 0x10, 0xCD */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_SW_COMMANDED_OFF) );
            *message_length = sizeof (tSIRF_MSG_SSB_SW_COMMANDED_OFF);
         }
         break;

         case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
         {
            tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->RFTestPoint       = SIRFBINARY_IMPORT_UINT8(ptr);   /* RF Test Point  0=Ignore, 1=Enable, 2=Disable */
               msg->INTCpuPause       = SIRFBINARY_IMPORT_UINT8(ptr);   /* INT_CPUPause       0=Ignore, 1=Enable, 2=Disable */
            }
         }
         break;

         case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:  /* 0x01 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

            if ( payload_length < ( (8 + 7 * SV_PER_PACKET) * sizeof(tSIRF_UINT8) +
                                    (1 + 11 * SV_PER_PACKET) * sizeof(tSIRF_UINT16) +
                                    (8 * SV_PER_PACKET) * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->week = SIRFBINARY_IMPORT_UINT16(ptr);
               for( i = 0; i < SV_PER_PACKET; i++)
               {
                  msg->extended_ephemeris[i].PRN = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].ephemerisValidityFlag = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].URA = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].IODE = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].Crs  = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].deltaN = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].M0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Cuc = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].eccen = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->extended_ephemeris[i].Cus = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].sqrtA = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->extended_ephemeris[i].toe = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->extended_ephemeris[i].Cic = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].omega0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Cis = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].i0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Crc = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].w = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].omegaDot = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].iDot = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].toc = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->extended_ephemeris[i].Tgd = SIRFBINARY_IMPORT_SINT8(ptr);
                  msg->extended_ephemeris[i].af2 = SIRFBINARY_IMPORT_SINT8(ptr);
                  msg->extended_ephemeris[i].af1 = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].af0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].age = SIRFBINARY_IMPORT_UINT8(ptr);
               }
               msg->extended_iono.alpha[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[2] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[3] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[2] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[3] = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_EE_POLL_STATE: /* 0x02 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->svid_mask = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         /* added for GSW_CLM */
         case SIRF_MSG_SSB_EE_QUERY_AGE:     /* 0x11 0xE8 */
         {
         }
         break;
         /* end of GSW_CLM */

         case SIRF_MSG_SSB_EE_DEBUG:
         {
            tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG *msg = (tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->debug_flags = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFE 0xE8 */
         {
            tSIRF_MSG_SSB_EE_DISABLE_EE_SECS *msg = (tSIRF_MSG_SSB_EE_DISABLE_EE_SECS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->num_secs   = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_TRKR_DBG:   /* 0x44 */
         case SIRF_MSG_SSB_TEXT:       /* 0xFF */
         {
            *message_length = payload_length - 1;
            memcpy(message_structure, ptr, payload_length - 1);
            *((char*) message_structure + payload_length - 1) = '\0';
            ptr += payload_length - 1;
         }
         break;

         default:
         {
            if (( *message_id >= SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN   /* 0xB4 */
                 && *message_id <= SIRF_MSG_SSB_PASSTHRU_INPUT_END   /* 0xC7 */
                 && payload_length - 1 <= SIRF_MSG_SSB_MAX_MESSAGE_LEN
                 && payload_length > 0) || (SIRF_MSG_SSB_SIRF_INTERNAL == *message_id)) /* 0xE4 */
            { /* pass-thru */
               *message_length = payload_length - 1;
               memcpy(message_structure, ptr, payload_length - 1);
               ptr += payload_length - 1;
            }
            else
            { /* this message id is not imported */
               tRet = SIRF_CODEC_SSB_INVALID_MSG_ID;
            }
         }
         break;
      }

      /* check if length does not match */
      if ( (tRet == SIRF_SUCCESS) && (ptr != (payload + payload_length)))
      {
         *message_id = *message_length = 0;
         tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
      }
   }
   else
   {
      tRet = SIRF_CODEC_SSB_NULL_POINTER;
   }

   return tRet;

} /* SIRF_CODEC_SSB_Decode() */

/**
 * @}
 */
