/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc. All rights reserved.*
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
 * MODULE:
 *
 * FILENAME: sirf_msg_ssb.h
 *
 * DESCRIPTION: Contains the GPS Protocol Definitions
 *
 ***************************************************************************/

#ifndef __SIRF_MSG_SSB__H__
#define __SIRF_MSG_SSB__H__

#include "sirf_types.h"

#define SIRF_MSG_NMEA_PASS_THRU \
    (SIRF_MAKE_MSG_ID(SIRF_LC_NMEA,0,0))

/*******************************************************************************
 *   This file defines the messages in the following list:
 *      1. SSB Message IDs
 *   followed by:
 *      1. SSB Message Structures
 ******************************************************************************/

/*******************************************************************************
   Constants Used in Structure Definitions
*******************************************************************************/

#define SIRF_NUM_CHANNELS       ( 12 )
#define SIRF_NUM_POINTS         ( 10 )


/*******************************************************************************
   SSB Message IDs - Output
*******************************************************************************/

/* Messages from GPS (output):                               LC           MID   SID */
#define SIRF_MSG_SSB_MEASURED_NAVIGATION    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x02,    0)
#define SIRF_MSG_SSB_MEASURED_TRACKER       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x04,    0)
#define SIRF_MSG_SSB_RAW_TRACKER            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x05,    0)
#define SIRF_MSG_SSB_SW_VERSION             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x06,    0)
#define SIRF_MSG_SSB_CLOCK_STATUS           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x07,    0)
#define SIRF_MSG_SSB_50BPS_DATA             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x08,    0)
#define SIRF_MSG_SSB_THROUGHPUT             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x09,    0)
#define SIRF_MSG_SSB_ERROR                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0A,    0)
#define SIRF_MSG_SSB_ACK                    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0B,    0)
#define SIRF_MSG_SSB_NAK                    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0C,    0)
#define SIRF_MSG_SSB_VISIBILITY_LIST        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0D,    0)
#define SIRF_MSG_SSB_ALMANAC                SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0E,    0)
#define SIRF_MSG_SSB_EPHEMERIS              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0F,    0)
#define SIRF_MSG_SSB_RTCM                   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x11,    0)
#define SIRF_MSG_SSB_OK_TO_SEND             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x12,    0)
#define SIRF_MSG_SSB_RECEIVER_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x13,    0)
#define SIRF_MSG_SSB_TEST_MODE_DATA         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x14,    0)
#define SIRF_MSG_SSB_DGPS_STATUS            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1B,    0)
#define SIRF_MSG_SSB_NL_MEAS_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1C,    0)
#define SIRF_MSG_SSB_NL_DGPS_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1D,    0)
#define SIRF_MSG_SSB_NL_SV_STATE_DATA       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1E,    0)
#define SIRF_MSG_SSB_NL_INIT_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1F,    0)
#define SIRF_MSG_SSB_GEODETIC_NAVIGATION    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x29,    0)
#define SIRF_MSG_SSB_QUEUE_CMD_PARAM        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B,    0)
    #define SIRF_MSG_SSB_QUEUE_CMD_NI       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x80)
    #define SIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x85)
    #define SIRF_MSG_SSB_QUEUE_CMD_SNM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x88)
    #define SIRF_MSG_SSB_QUEUE_CMD_SDM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x89)
    #define SIRF_MSG_SSB_QUEUE_CMD_SDGPSM   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8A)
    #define SIRF_MSG_SSB_QUEUE_CMD_SEM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8B)
    #define SIRF_MSG_SSB_QUEUE_CMD_SPM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8C)
    #define SIRF_MSG_SSB_QUEUE_CMD_SSN      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8F)
    #define SIRF_MSG_SSB_QUEUE_CMD_LP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x97)
    #define SIRF_MSG_SSB_QUEUE_CMD_SSBAS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0xAA)
#define SIRF_MSG_SSB_ADC_ODOMETER_DATA      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2D,    0)
#define SIRF_MSG_SSB_TEST_MODE_DATA_3       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2E,    0)
#define SIRF_MSG_SSB_DR_OUTPUT              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30,    0)
   #define SIRF_MSG_SSB_DR_NAV_STATUS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x01)
   #define SIRF_MSG_SSB_DR_NAV_STATE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x02)
   #define SIRF_MSG_SSB_DR_NAV_SUBSYS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x03)
   #define SIRF_MSG_SSB_DR_VALID            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x05)
   #define SIRF_MSG_SSB_DR_GYR_FACT_CAL     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x06)
   #define SIRF_MSG_SSB_DR_SENS_PARAM       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x07)
   #define SIRF_MSG_SSB_DR_DATA_BLK         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x08)
   #define SIRF_MSG_SSB_MMF_STATUS          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x50)
#define SIRF_MSG_SSB_SBAS_PARAM             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x32,    0)
#define SIRF_MSG_SSB_SIRFNAV_NOTIFICATION   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33,    0)
   #define SIRF_MSG_SSB_SIRFNAV_COMPLETE    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x01)
   #define SIRF_MSG_SSB_SIRFNAV_TIMING      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x02)
   #define SIRF_MSG_SSB_DEMO_TIMING         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x03)
   #define SIRF_MSG_SSB_SIRFNAV_TIME_TAGS   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x04)
   #define SIRF_MSG_SSB_TRACKER_LOADER_STATE SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x33, 0x06)
   #define SIRF_MSG_SSB_SIRFNAV_START       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x07)
   #define SIRF_MSG_SSB_SIRFNAV_STOP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x08)
#define SIRF_MSG_SSB_EVENT                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x36,    0)
   #define SIRF_MSG_SSB_STARTUP_INFO        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x36, 0x01)
#define SIRF_MSG_SSB_EE                     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,    0)
   #define SIRF_MSG_SSB_EE_GPS_TIME_INFO    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x01)
   #define SIRF_MSG_SSB_EE_INTEGRITY        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x02)
   #define SIRF_MSG_SSB_EE_STATE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x03)
   #define SIRF_MSG_SSB_EE_CLK_BIAS_ADJ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x04)
   #define SIRF_MSG_SSB_EE_EPHEMERIS_AGE    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x11)
   #define SIRF_MSG_SSB_EE_ACK              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0xFF)
#define SIRF_MSG_SSB_TEST_MODE_DATA_7       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x3F, 0x07)
#define SIRF_MSG_SSB_NL_AUX_DATA            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40,    0)
    #define SIRF_MSG_SSB_NL_AUX_INIT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x01)
    #define SIRF_MSG_SSB_NL_AUX_MEAS_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x02)
    #define SIRF_MSG_SSB_NL_AUX_AID_DATA    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x03)
#define SIRF_MSG_SSB_TRACKER_DATA                SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x41,    0)
   #define SIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x41, 0xC0)
#define SIRF_MSG_SSB_DOP_VALUES             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x42,    0)
#define SIRF_MSG_SSB_TRKR_DBG               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x44,    0)
#define SIRF_MSG_SSB_50BPS_DATA_VERIFIED    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x52,    0) // Message structure is identical to SIRF_MSG_SSB_50BPS_DATA
#define SIRF_MSG_SSB_PASSTHRU_OUTPUT_BEGIN  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x61,    0)
#define SIRF_MSG_SSB_SIRF_INTERNAL_OUT      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE1,    0)
#define SIRF_MSG_SSB_PASSTHRU_OUTPUT_END    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x7F,    0)
#define SIRF_MSG_SSB_TEXT                   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xFF,    0)

/*******************************************************************************
   SSB Message IDs - Input
*******************************************************************************/

/* Messages to GPS (input):                                    LC           MID   SID */
#define SIRF_MSG_SSB_INITIALIZE               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x80,    0)
#define SIRF_MSG_SSB_SET_NMEA_MODE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x81,    0)
#define SIRF_MSG_SSB_SET_ALMANAC              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x82,    0)
#define SIRF_MSG_SSB_POLL_SW_VERSION          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x84,    0)
#define SIRF_MSG_SSB_SET_DGPS_SOURCE          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x85,    0)
#define SIRF_MSG_SSB_SET_NAV_MODE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x88,    0)
#define SIRF_MSG_SSB_SET_DOP_MODE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x89,    0)
#define SIRF_MSG_SSB_SET_DGPS_MODE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8A,    0)
#define SIRF_MSG_SSB_SET_ELEV_MASK            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8B,    0)
#define SIRF_MSG_SSB_SET_POWER_MASK           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8C,    0)
#define SIRF_MSG_SSB_SET_STAT_NAV             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8F,    0)
#define SIRF_MSG_SSB_POLL_CLOCK_STATUS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x90,    0)
#define SIRF_MSG_SSB_POLL_ALMANAC             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x92,    0)
#define SIRF_MSG_SSB_POLL_EPHEMERIS           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x93,    0)
#define SIRF_MSG_SSB_SET_EPHEMERIS            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x95,    0)
#define SIRF_MSG_SSB_SET_OP_MODE              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x96,    0)
#define SIRF_MSG_SSB_SET_LOW_POWER            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x97,    0)
#define SIRF_MSG_SSB_POLL_RECEIVER_PARAMS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x98,    0)
#define SIRF_MSG_SSB_SIRFNAV_COMMAND          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1,    0)
   #define SIRF_MSG_SSB_DEMO_SET_RESTART_MODE SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x01)
   #define SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x02)
   #define SIRF_MSG_SSB_DEMO_STOP_TEST_APP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x03)
   #define SIRF_MSG_SSB_DEMO_START_GPS_ENGINE SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x05)
   #define SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x06)
   #define SIRF_MSG_SSB_SIRFNAV_STORE_NOW     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x07)
#define SIRF_MSG_SSB_SET_MSG_RATE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA6,    0)
#define SIRF_MSG_SSB_SET_LOW_POWER_PARAMS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA7,    0)
#define SIRF_MSG_SSB_POLL_CMD_PARAM           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA8,    0)
#define SIRF_MSG_SSB_SET_SBAS_PRN             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAA,    0)
#define SIRF_MSG_SSB_ADVANCED_NAV_INIT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC,    0)
   #define SIRF_MSG_SSB_DR_SET_NAV_INIT       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x01)
   #define SIRF_MSG_SSB_DR_SET_NAV_MODE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x02)
   #define SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x03)
   #define SIRF_MSG_SSB_DR_SET_SENS_PARAM     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x04)
   #define SIRF_MSG_SSB_DR_POLL_VALID         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x05)
   #define SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x06)
   #define SIRF_MSG_SSB_DR_POLL_SENS_PARAM    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x07)
   #define SIRF_MSG_SSB_DR_CAR_BUS_DATA       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x09)
   #define SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x0A)
   #define SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x0B)
   #define SIRF_MSG_SSB_MMF_DATA              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x50)
   #define SIRF_MSG_SSB_MMF_SET_MODE          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x51)
#define SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB4,    0)
#define SIRF_MSG_SSB_PASSTHRU_INPUT_END       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xC7,    0)
#define SIRF_MSG_SSB_SW_CONTROL               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD,    0)
    #define SIRF_MSG_SSB_SW_COMMANDED_OFF     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD, 0x10)
#define SIRF_MSG_SSB_TRK_HW_CONFIG            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCE,    0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCF,    0)
#define SIRF_MSG_SSB_SIRF_INTERNAL            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE4,    0)
#define SIRF_MSG_SSB_EE_INPUT                 SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8,    0)
    #define SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x01)
    #define SIRF_MSG_SSB_EE_POLL_STATE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x02)
    #define SIRF_MSG_SSB_EE_FILE_DOWNLOAD     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x10)
    #define SIRF_MSG_SSB_EE_QUERY_AGE         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x11)
    #define SIRF_MSG_SSB_EE_FILE_PART         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x12)
    #define SIRF_MSG_SSB_EE_DOWNLOAD_TCP      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x13)
    #define SIRF_MSG_SSB_EE_SET_EPHEMERIS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x14)
    #define SIRF_MSG_SSB_EE_FILE_STATUS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x15)
    #define SIRF_MSG_SSB_EE_DISABLE_EE_SECS   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFE)
    #define SIRF_MSG_SSB_EE_DEBUG             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFF)

/* COMMON CONSTANTS ----------------------------------------------------------*/

/*******************************************************************************
   The maximum message length that will ever be used in a message buffer.
   Currently the largest SSB message structure (tSIRF_MSG_SSB_SET_ALMANAC)
   is under 900 bytes.
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_MESSAGE_LEN    ( 1024 )

#define SIRF_TIME_LSB                   ( 100.0 )
#define SIRF_NUM_CHANNELS               ( 12 )
#define SIRF_MAX_SVID_CNT               ( 32 )

/* If the EPE-major Axis is exceeded, then a full fix will not be sent */
#define SIRF_MAX_EPE_FOR_VALID_SSB      (600.0F)

/* SIRF_MSG_SSB_MEASURED_NAVIGATION -----------------------------------------------*/

#define SIRF_MSG_SSB_VELOCITY_LSB               ( 0.125F )
#define SIRF_MSG_SSB_DOP_LSB                    ( 0.2F )

#define SIRF_MSG_SSB_MODE_MASK                  ( 0x07 )
#define SIRF_MSG_SSB_MODE_DOP_MASK_EXCEED       ( 0x40 )
#define SIRF_MSG_SSB_MODE_DGPS_USED             ( 0x80 )

#define SIRF_MSG_SSB_MODE2_SOLUTION_VALIDATED   ( 0x02 )
#define SIRF_MSG_SSB_MODE2_DR_TIMED_OUT         ( 0x04 )
#define SIRF_MSG_SSB_MODE2_VELOCITY_INVALID     ( 0x10 )

/*******************************************************************************
   SSB Message Structures
*******************************************************************************/

/*******************************************************************************
   SIRF_MSG_SSB_MEASURED_NAVIGATION    ( 0x02 ) 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_MEASURED_NAVIGATION_tag
{
   tSIRF_INT32  ecef_x;       /* estimated ECEF X position in meters */
   tSIRF_INT32  ecef_y;       /* estimated ECEF Y position in meters */
   tSIRF_INT32  ecef_z;       /* estimated ECEF Z position in meters */
   tSIRF_INT16  ecef_vel_x;   /* estimated ECEF X velocity in 0.125 m/sec */
   tSIRF_INT16  ecef_vel_y;   /* estimated ECEF Y velocity in 0.125 m/sec */
   tSIRF_INT16  ecef_vel_z;   /* estimated ECEF Z velocity in 0.125 m/sec */
   tSIRF_UINT8   nav_mode;    /* current mode of operation */
   tSIRF_UINT8   hdop;        /* horizontal dilution of precision, LSB=0.2 */
   tSIRF_UINT8   nav_mode2;   /* additional mode information */
   tSIRF_UINT16  gps_week;    /* GPS week number in weeks */
   tSIRF_UINT32  gps_tow;     /* GPS time of week in 10 ms, 0..60,479,900 ms */
   tSIRF_UINT8   sv_used_cnt; /* number of SVs used, 0..12 */
   tSIRF_UINT8   sv_used[SIRF_NUM_CHANNELS];  /* IDs of SVs used, 1..32 */

}  tSIRF_MSG_SSB_MEASURED_NAVIGATION;

/*******************************************************************************
   SIRF_MSG_SSB_MEASURED_TRACKER       ( 0x04 ) 4
*******************************************************************************/

#define SIRF_MSG_SSB_AZIMUTH_LSB                ( 1.5 )
#define SIRF_MSG_SSB_ELEVATION_LSB              ( 0.5 )
#define SIRF_CNO_MAX                    ( 60 )
#define SIRF_NUM_POINTS                 ( 10 )

typedef struct tSIRF_MSG_SSB_MEASURED_TRACKER_tag
{
   tSIRF_INT16 gps_week;               /* GPS week number in weeks */
   tSIRF_UINT32 gps_tow;                /* GPS time of week in 10 ms,
                                          0..60,479,900 */
   tSIRF_UINT8  chnl_cnt;               /* ch count, 0..12 */
   struct tSIRF_SV_INFO_tag
   {
      tSIRF_UINT8  svid;                /* ID of SV used */
      tSIRF_UINT8  azimuth;             /* SV azimuth in 1.5 deg */
      tSIRF_UINT8  elevation;           /* SV elevation in 0.5 deg */
      tSIRF_UINT16 state;               /* tracking state, 1-locked,
                                          0-not locked, b0 @ t0 */
      tSIRF_UINT8  cno[SIRF_NUM_POINTS]; /* CNo in dB-Hz, or CPower in dBW */
   } chnl[SIRF_NUM_CHANNELS];

}  tSIRF_MSG_SSB_MEASURED_TRACKER;

/*******************************************************************************
   SIRF_MSG_SSB_RAW_TRACKER              ( 0x05 ) 5
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_RAW_TRACKER_tag
{
   tSIRF_INT32 channel;             /* Channel number in tracking [0 to 11] */
   tSIRF_INT16 svid;                /* Satellite id  [1 to 32] */
   tSIRF_INT16 state;               /* Status of the tracker channel
                                       (mask = 0x1FF) */
   tSIRF_INT32 bit_number;          /* Bits at 50 bps = 20 ms */
   tSIRF_INT16 msec_number;         /* Represents time in units of msec */
   tSIRF_INT16 chip_number;         /* Represents time in units of CA chips */
   tSIRF_INT32 code_phase;          /* Represents time in units of chips */
   tSIRF_INT32 carrier_doppler;     /* Doppler frequency */
   tSIRF_INT32 measure_timetag;     /* Measurement time tag */
   tSIRF_INT32 delta_carrier_phase; /* Current carrier phase */
   tSIRF_INT16 search_cnt;          /* How many times to search for a SV */
   tSIRF_UINT8  cno[SIRF_NUM_POINTS]; /* C/No in dB-Hz */
   tSIRF_UINT8  power_bad_count;     /* Count of Power in 20 ms below 31 dB-Hz */
   tSIRF_UINT8  phase_bad_count;     /* Count of Power in 20 ms below 31 dB-Hz */
   tSIRF_INT16 delta_car_interval;  /* Count of ms contained in delta_carrier
                                       phase */
   tSIRF_INT16 correl_interval;     /* Correlation interval */

}  tSIRF_MSG_SSB_RAW_TRACKER;

/*******************************************************************************
   SIRF_MSG_SSB_SW_VERSION             ( 0x06 ) 6
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SW_VERSION_tag
{
   tSIRF_INT8  sw_version[80];
}  tSIRF_MSG_SSB_SW_VERSION;

/*******************************************************************************
   SIRF_MSG_SSB_CLOCK_STATUS           ( 0x07 ) 7
**************************************************************************/

typedef struct
{
   tSIRF_UINT16 gps_week;        /* GPS week number in weeks                   */
   tSIRF_UINT32 gps_tow;         /* GPS time of week in 10 ms; 0..60,479,900 ms*/
   tSIRF_UINT8  sv_used_cnt;     /* number of SVs used, 0..12                  */
   tSIRF_UINT32 clk_offset;      /* clock Drift in Hz                          */
   tSIRF_UINT32 clk_bias;        /* clock Bias in nanoseconds                  */
   tSIRF_UINT32 est_gps_time;    /* estimated gps time in milliseconds         */

}  tSIRF_MSG_SSB_CLOCK_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_50BPS_DATA             ( 0x08 )  8
   SIRF_MSG_SSB_50BPS_DATA_VERIFIED    ( 0x52 ) 82
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_50BPS_DATA_tag
{
   tSIRF_UINT8  chnl;
   tSIRF_UINT8  svid;
   tSIRF_UINT32 word[10];
}  tSIRF_MSG_SSB_50BPS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_THROUGHPUT             ( 0x09 ) 9
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_THROUGHPUT_tag
{
   tSIRF_UINT16 seg_stat_max;  /* Maximum number at which Tracker finished
                                 processing */
   tSIRF_UINT16 seg_stat_lat;  /* Maximum Tracker interrupt latency over 1
                                 second */
   tSIRF_UINT16 avg_trk_time;  /* Average time spent in tracker
                                 SegStatTTL/TTLcnt */
   tSIRF_UINT16 last_ms;       /* ms on which nav finished processing
                                 measurements on the PREVIOUS nav cycle.
                                 range 0-1000 */
}  tSIRF_MSG_SSB_THROUGHPUT;

/*******************************************************************************
   SIRF_MSG_SSB_ERROR                  ( 0x0A ) 10
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_ERROR_PARAMS (5)

#define SIRF_MSG_SSB_ERRID(base,id) (base | id)

/*  err_id bases: */
#define SIRF_MSG_SSB_ERRCB_NOTIFY                    ( 0x0000 )
#define SIRF_MSG_SSB_ERRCB_WARNING                   ( 0x1000 )
#define SIRF_MSG_SSB_ERRCB_ALERT                     ( 0x2000 )

/* err_id: SIRF_MSG_SSB_ERRCB_NOTIFY */
#define SIRF_MSG_SSB_ERRID_NONE                      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x00)
#define SIRF_MSG_SSB_ERRID_SP_SERIALPARITY           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x01)
#define SIRF_MSG_SSB_ERRID_CS_SVPARITY               SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x02)
#define SIRF_MSG_SSB_ERRID_KFS_BADALTITUDE           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x08)
#define SIRF_MSG_SSB_ERRID_RMC_GETTINGPOSITION       SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x09)
#define SIRF_MSG_SSB_ERRID_RXM_TIMEEXCEEDED          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0A)
#define SIRF_MSG_SSB_ERRID_RXM_TDOPOVERFLOW          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0B)
#define SIRF_MSG_SSB_ERRID_RXM_VALIDDURATIONEXCEEDED SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0C)
#define SIRF_MSG_SSB_ERRID_STRTP_BADPOSTION          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0D)

/* err_id: SIRF_MSG_SSB_ERRCB_WARNING */
#define SIRF_MSG_SSB_ERRID_MI_VCOCLOCKLOST           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x01)
#define SIRF_MSG_SSB_ERRID_MI_FALSEACQRECEIVERRESET  SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x03)
#define SIRF_MSG_SSB_ERRID_KFC_KILLCHANNEL           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x04)
#define SIRF_MSG_SSB_ERRID_KRS_BADSOLUTION           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x07)
#define SIRF_MSG_SSB_ERRID_STRTP_SRAMCKSUM           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x08)
#define SIRF_MSG_SSB_ERRID_STRTP_RTCTIMEINVALID      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x09)
#define SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_VELOCITY SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x0a)
#define SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_NUMSV    SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x0b)

/* err_id: SIRF_MSG_SSB_ERRCB_ALERT */
#define SIRF_MSG_SSB_ERRID_MI_BUFFERALLOCFAILURE     SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x01)
#define SIRF_MSG_SSB_ERRID_MI_UPDATETIMEFAILURE      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x02)
#define SIRF_MSG_SSB_ERRID_MI_MEMORYTESTFAILED       SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x03)

typedef struct tSIRF_MSG_SSB_ERROR_tag
{
   tSIRF_UINT16 err_id;
   tSIRF_UINT16 param_cnt;
   tSIRF_UINT32 param[SIRF_MSG_SSB_MAX_ERROR_PARAMS];
}  tSIRF_MSG_SSB_ERROR;

/*******************************************************************************
   SIRF_MSG_SSB_ACK                    ( 0x0B ) 11
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_ACK_tag
{
   tSIRF_UINT8  msg_id;
   tSIRF_UINT8  sub_id;
}  tSIRF_MSG_SSB_ACK;

/*******************************************************************************
   SIRF_MSG_SSB_NAK                    ( 0x0C ) 12
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NAK_tag
{
   tSIRF_UINT8  msg_id;
   tSIRF_UINT8  sub_id;
}  tSIRF_MSG_SSB_NAK;

/*******************************************************************************
   SIRF_MSG_SSB_VISIBILITY_LIST        ( 0x0D ) 13
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_VISIBILITY_LIST_tag
{
   tSIRF_UINT8  svid_cnt;        /* number of valid entries in aVis[] element*/
   struct
   {
      tSIRF_UINT8  svid;         /* sat PRN id */
      tSIRF_INT16 azimuth;
      tSIRF_INT16 elevation;
   } visible[SIRF_MAX_SVID_CNT]; /* visible sat info */
}  tSIRF_MSG_SSB_VISIBILITY_LIST;

/*******************************************************************************
   SIRF_MSG_SSB_ALMANAC                ( 0x0E ) 14
*******************************************************************************/

#define SIRF_MSG_SSB_ALMANAC_GET_WEEK(week_and_status) \
    ((week_and_status & 0xFFC0) >> 6)
/* 1 = good; 0 = bad */
#define SIRF_MSG_SSB_ALMANAC_GET_STATUS(week_and_status) \
    ((week_and_status & 0x3F) >> 6)
#define SIRF_MSG_SSB_ALMANAC_MAKE_WEEK_AND_STATUS(week,status)\
    (((week & 0x3FF) << 6) | (status & 0x3F))

/* Careful This is NOT the same as SIRF_MSG_SSB_ALMANAC_DATA_ENTRY which is 14 */
#define SIRF_MSG_SSB_ALMANAC_ENTRIES (12)
typedef struct tSIRF_MSG_SSB_ALMANAC_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 week_and_status;
   tSIRF_UINT16 data[SIRF_MSG_SSB_ALMANAC_ENTRIES];
   tSIRF_UINT16 almanac_checksum;
}  tSIRF_MSG_SSB_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_EPHEMERIS              ( 0x0F ) 15
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EPHEMERIS_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 subframe[3][15];
}  tSIRF_MSG_SSB_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_RTCM                   ( 0x11 ) 17
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_RTCM_BUFFER (SIRF_MSG_SSB_MAX_MESSAGE_LEN - sizeof(tSIRF_UINT16))
typedef struct tSIRF_MSG_SSB_RTCM_tag
{
   tSIRF_UINT16      buffer_len;
   tSIRF_UINT8       buffer[SIRF_MSG_SSB_MAX_RTCM_BUFFER];
}tSIRF_MSG_SSB_RTCM;

/*******************************************************************************
   SIRF_MSG_SSB_OK_TO_SEND             ( 0x12 ) 18
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_OK_TO_SEND_tag
{
   tSIRF_UINT8 input_enabled;  /* 1=receiver is not in sleep mode          */
                              /* 0= receiver is about to be in sleep mode */
}  tSIRF_MSG_SSB_OK_TO_SEND;

/*******************************************************************************
   SIRF_MSG_SSB_RECEIVER_PARAMS        ( 0x13 ) 19
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_RECEIVER_PARAMS_tag
{
   tSIRF_UINT32 reserved_1;
   tSIRF_UINT8  alt_mode;
   tSIRF_UINT8  alt_src;
   tSIRF_INT16 alt_input;
   tSIRF_UINT8  degraded_mode;
   tSIRF_UINT8  degraded_timeout;
   tSIRF_UINT8  dr_timeout;
   tSIRF_UINT8  trk_smooth;
   tSIRF_UINT8  static_nav_mode;
   tSIRF_UINT8  enable_3sv_lsq;
   tSIRF_UINT32 reserved_2;
   tSIRF_UINT8  dop_mask_mode;
   tSIRF_INT16 nav_elev_mask;
   tSIRF_UINT8  nav_pwr_mask;
   tSIRF_UINT32 reserved_3;
   tSIRF_UINT8  dgps_src;
   tSIRF_UINT8  dgps_mode;
   tSIRF_UINT8  dgps_timeout;
   tSIRF_UINT32 reserved_4;
   tSIRF_UINT8  lp_push_to_fix;
   tSIRF_INT32 lp_on_time;
   tSIRF_INT32 lp_interval;
   tSIRF_UINT8  lp_user_tasks_enabled;
   tSIRF_INT32 lp_user_task_interval;
   tSIRF_UINT8  lp_pwr_cycling_enabled;
   tSIRF_UINT32 lp_max_acq_time;
   tSIRF_UINT32 lp_max_off_time;
   tSIRF_UINT8 apm_enabled_power_duty_cycle;
   tSIRF_UINT16 number_of_fixes;
   tSIRF_UINT16 time_between_fixes;
   tSIRF_UINT8 horz_vert_error_max;
   tSIRF_UINT8 response_time_max;
   tSIRF_UINT8 time_accu_time_duty_cycle_priority;
}  tSIRF_MSG_SSB_RECEIVER_PARAMS;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA         ( 0x14 ) 20
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_tag
{
   tSIRF_UINT16 svid;                /* fixed SVid to search on all channels*/
   tSIRF_UINT16 period;              /* number of seconds statistics are
                                       accumulated over */
   tSIRF_UINT16 bit_synch_time;      /* time to first bit synch              */
   tSIRF_UINT16 bit_count;           /* Count of data bits came out during a
                                       period */
   tSIRF_UINT16 poor_status_count;   /* Count of 100ms periods tracker spent in
                                       any status < 3F */
   tSIRF_UINT16 good_status_count;   /* Count of 100ms periods tracker spent in
                                       status 3F */
   tSIRF_UINT16 parity_error_count;  /* Number of word parity errors          */
   tSIRF_UINT16 lost_vco_count;      /* number of msec VCO lock loss was
                                       detected */
   tSIRF_UINT16 frame_synch_time;    /* time to first frame synch             */
   tSIRF_INT16 cno_mean;            /* c/No mean in 0.1 dB-Hz                 */
   tSIRF_INT16 cno_sigma;           /* c/No sigma in 0.1 dB                   */
   tSIRF_INT16 clock_drift;         /* clock drift in 0.1 Hz                  */
   tSIRF_INT32 clock_offset_mean;   /* average clock offset in 0.1 Hz         */

   /*  For bit test at a high c/no - Test Mode 3 only */
   tSIRF_INT16 bad_1khz_bit_count;  /* bad bit count out of 10,000
                                       (10 seconds * 1000 bits )     */
   tSIRF_INT32 abs_i20ms;           /* phase noise estimate I20ms sum         */
   tSIRF_INT32 abs_q1ms;            /* phase noise estimate Q1ms sum          */
   tSIRF_INT32 reserved[3];
}  tSIRF_MSG_SSB_TEST_MODE_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_DGPS_STATUS            ( 0x1B ) 27
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DGPS_STATUS_tag
{
   /* this message is not right */
   tSIRF_UINT8  src; /* Reuse the SIRF_MSG_SSB_DGPS_SRC_<flag> defines */
   tSIRF_UINT8  cor_age[SIRF_NUM_CHANNELS];
   tSIRF_UINT8  reserved[2];
   struct
   {      tSIRF_UINT8  prn;
      tSIRF_INT16 cor;
   } corrections[SIRF_NUM_CHANNELS];
}  tSIRF_MSG_SSB_DGPS_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_NL_MEAS_DATA           ( 0x1C ) 28
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_MEAS_DATA_tag
{
   tSIRF_UINT8  Chnl;                /* Channel number which is tracking */
   tSIRF_UINT32 Timetag;             /* time in ms of the measurement block in
                                       Rx SW time */
   tSIRF_UINT8  svid;                /* Acquired satellite id */
   tSIRF_DOUBLE gps_sw_time;         /* GPS Time estimated by software (ms)*/
   tSIRF_DOUBLE pseudorange;         /* Pseudorange measurment in centimeters */
   tSIRF_FLOAT  carrier_freq;        /* Either delta PR normalized or freq from
                                       AFC loop */
   tSIRF_DOUBLE carrier_phase;       /* The integrated carrier phase in
                                       millimeters */
   tSIRF_UINT16 time_in_track;       /* The count in ms of time in track for SV*/
   tSIRF_UINT8  sync_flags;          /* 2 bits, bit 0: Coherent Integrat
                                       Interval, bit 1: Sync */
   tSIRF_UINT8  cton[SIRF_NUM_POINTS];/* avg signal pwr of prev second in dB-Hz
                                       for each channel*/
   tSIRF_UINT16 delta_range_interval;/* delta PR measurement interval for the
                                       preceding sec */
   tSIRF_UINT16 mean_delta_range_time;/* mean time of delta PR interval in ms */
   tSIRF_INT16 extrapolation_time;  /* PR extrap time in ms to reach common
                                       timetag value */
   tSIRF_UINT8  phase_error_count;   /* cnt of phase errors > 60 deg measured in
                                       preceding sec */
   tSIRF_UINT8  low_power_count;     /* cnt of power measurements < 28 dB-Hz in
                                       preceding sec */
}  tSIRF_MSG_SSB_NL_MEAS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_DGPS_DATA           ( 0x1D ) 29
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_DGPS_DATA_tag
{
   tSIRF_INT16 svid;
   tSIRF_INT16 iod;
   tSIRF_UINT8  source;
   tSIRF_FLOAT  pr_correction;   /* (m)   */
   tSIRF_FLOAT  prr_correction;  /* (m/s) */
   tSIRF_FLOAT  correction_age;  /* (s)   */
   tSIRF_FLOAT  prc_variance;    /* (m)   */
   tSIRF_FLOAT  prrc_variance;   /* (m/s) */
}  tSIRF_MSG_SSB_NL_DGPS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_SV_STATE_DATA       ( 0x1E ) 30
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_SV_STATE_DATA_tag
{
   tSIRF_UINT8  svid;
   tSIRF_DOUBLE time;
   tSIRF_DOUBLE pos[3];
   tSIRF_DOUBLE vel[3];
   tSIRF_DOUBLE clk;
   tSIRF_FLOAT  clf;
   tSIRF_UINT8  eph;
   tSIRF_FLOAT  posvar;
   tSIRF_FLOAT  clkvar;
   tSIRF_FLOAT  iono;
}  tSIRF_MSG_SSB_NL_SV_STATE_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_INIT_DATA           ( 0x1F ) 31
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_INIT_DATA_tag
{
   /* From NL_ControlBlock */
   tSIRF_UINT8  enable_con_alt_mode;
   tSIRF_UINT8  alt_mode;
   tSIRF_UINT8  alt_source;
   tSIRF_FLOAT  altitude;
   tSIRF_UINT8  degraded_mode;
   tSIRF_INT16 degraded_timeout;
   tSIRF_INT16 dr_timeout;
   tSIRF_INT16 coast_timeout;
   tSIRF_UINT8  tracksmooth_mode;
   tSIRF_UINT8  dop_selection;
   tSIRF_INT16 hdop_thresh;
   tSIRF_INT16 gdop_thresh;
   tSIRF_INT16 pdop_thresh;
   tSIRF_UINT8  dgps_selection;
   tSIRF_INT16 dgps_timeout;
   tSIRF_INT16 elev_nav_mask;
   tSIRF_INT16 pow_nav_mask;
   tSIRF_UINT8  editing_residual_mode;
   tSIRF_INT16 editing_residual_threshold;
   tSIRF_UINT8  ssd_mode;
   tSIRF_INT16 ssd_threshold;
   tSIRF_UINT8  static_nav_mode;
   tSIRF_INT16 static_nav_threshold;

   /* From NL_PositionInitStruct */
   tSIRF_DOUBLE ecef_x;
   tSIRF_DOUBLE ecef_y;
   tSIRF_DOUBLE ecef_z;
   tSIRF_UINT8  position_init_source;

   /* From NL_TimeInitStruct */
   tSIRF_DOUBLE gps_time;
   tSIRF_INT16 gps_week;
   tSIRF_UINT8  time_init_source;

   /* From NL_ClockDriftInitStruct */
   tSIRF_DOUBLE clk_offset;
   tSIRF_UINT8  clk_offset_init_source;
}  tSIRF_MSG_SSB_NL_INIT_DATA;


/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_INIT_DATA           0x40 0x01
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_INIT_DATA_tag
{
   tSIRF_UINT32 time_init_unc;         /* Initial time uncertainty (usec) */
   tSIRF_UINT16 saved_pos_week;        /* Week of saved position */ 
   tSIRF_UINT32 saved_pos_tow;         /* Time of week of saved position (sec) */
   tSIRF_UINT16 saved_pos_ehe;         /* EHE of saved position (100m) */
   tSIRF_UINT16 saved_pos_eve;         /* EVE of saved position (m) */
   tSIRF_UINT8  sw_version;            /* Tracker SW version */
   tSIRF_UINT8  icd_version;           /* Tracker ICD version */
   tSIRF_UINT16 chip_version;          /* Tracker HW Chip version */
   tSIRF_UINT32 acq_clk_speed;         /* Default rate of Tracker's internal clock (Hz) */
   tSIRF_UINT32 default_clock_offset;  /* Default freq offset of Tracker's internal clock (Hz) */
   tSIRF_UINT32 tracker_status;        /* Tracker Status:
                                          Bit 0: Status 0=good; 1=bad
                                          Bit 1: Cache  0=Disable; 1=Enable
                                          Bit 2: RTC    0=Invalid; 1=Valid
                                          Bit 3-31: reserved */ 
   tSIRF_UINT32 reserved;
}  tSIRF_MSG_SSB_NL_AUX_INIT_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_MEAS_DATA           0x40 0x02
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_MEAS_DATA_tag
{
   tSIRF_UINT8  sv_prn;              /* Satellite PRN number */
   tSIRF_UINT8  status;              /* Tracker system status:
                                        0x01 Trickle Power Active
                                        0x02 Scalable Tracking Loop Active
                                        0x04 SCL_MEAS Active */
   tSIRF_UINT8  extended_status;     /* Tracker channel status:
                                        0x02 Subframe sync verified
                                        0x04 Possible cycle slip
                                        0x08 Subframe sync lost
                                        0x10 Multipath detected
                                        0x20 Multipath-only detected
                                        0x40 Weak frame sync done */
   tSIRF_UINT8  bit_sync_qual;       /* Confidence metric for bitsync */
   tSIRF_UINT32 time_tag;            /* Measurement time tag (acqclk) */
   tSIRF_UINT32 code_phase;          /* Code Phase (2^-11 chip) */
   tSIRF_INT32  carrier_phase;       /* Carrier Phase (L1 cycle) */
   tSIRF_INT32  carrier_freq;        /* Carrier Frequency (0.000476Hz) */
   tSIRF_INT16  carrier_accel;       /* Doppler Rate (0.1m/s/s) */
   tSIRF_INT16  ms_num;              /* Millisecond number (0 to 19) */
   tSIRF_INT32  bit_num;             /* Bit number (0 to 30239999) */
   tSIRF_INT32  code_correction;     /* For code smoothing (cycle) */
   tSIRF_INT32  smooth_code;         /* For PR smoothing (2^-10 cycle) */ 
   tSIRF_INT32  code_offset;         /* Code offset (2^-11 chip)*/ 
   tSIRF_INT16  pseudorange_noise;   /* Pseudorange noise estimate */
   tSIRF_INT16  delta_range_qual;    /* Deltarange accuracy estimate */
   tSIRF_INT16  phase_lock_qual;     /* Phase lock accuracy estimate */
   tSIRF_INT16  ms_uncertainty;      /* Millisecond uncertainty */
   tSIRF_UINT16 sum_abs_I;           /* Sum |I| for this measurement */
   tSIRF_UINT16 sum_abs_Q;           /* Sum |Q| for this measurement */
   tSIRF_INT32  sv_bit_num;          /* Bit number of last SV bit */
   tSIRF_INT16  mp_los_det_value;    /* Multipath line-of-sight detection value */
   tSIRF_INT16  mp_only_det_value;   /* Multipath-only line-of-sight detection value */
   tSIRF_UINT8  recovery_status;     /* Recovery status:
                                        0x01  WBS active
                                        0x02  False Lock
                                        0x04  Bad prepos, wrong BS   
                                        0x08  Bad prepos, wrong FS
                                        0x10  Bad prepos, others */
   tSIRF_UINT32 sw_time_uncertainty; /* SW time uncertainty (usec) */
}  tSIRF_MSG_SSB_NL_AUX_MEAS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_AID_DATA           0x40 0x03
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_AID_DATA_tag
{
   tSIRF_INT32  ecef_x;         /* ECEF X position (m) */
   tSIRF_INT32  ecef_y;         /* ECEF Y position (m) */
   tSIRF_INT32  ecef_z;         /* ECEF Z position (m) */
   tSIRF_UINT32 horiz_pos_unc;  /* Horizontal position uncertainty (m) */ 
   tSIRF_UINT16 alt_unc;        /* Altitude uncertainty (m) */
   tSIRF_UINT32 sw_tow;         /* Time of the week (msec) */
}  tSIRF_MSG_SSB_NL_AUX_AID_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_GEODETIC_NAVIGATION    ( 0x29 ) 41
*******************************************************************************/

#define SIRF_MSG_SSB_VALID_NO_TRACKER           ( 0x8000 )
#define SIRF_MSG_SSB_TRACKER_LOADING            ( 0x4000 )
#define SIRF_MSG_SSB_GPS_TEST_MODE              ( 0x2000 )
#define SIRF_MSG_SSB_GPS_FIX_INVALID            ( 0x0001 )


/** Additional mode information */

/* Bit 0: Feedback enabled,  or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_FEEDBACK_ENABLED  (0x01) 
/* Bit 1: Feedback received, or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_FEEDBACK_RECEIVED (0x02) 
/* Bit 2: Map Matching used in position compution, or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_USED_IN_POSITION  (0x04) 
/* Bit 3: GPS time and week set, or not = not set */
#define SIRF_MSG_SSB_GN_GPS_TIME_AND_WEEK_SET          (0x08) 
/* Bit 4: Verified by satellite, or not = not set */
#define SIRF_MSG_SSB_GN_UTC_OFFSET_VERIFIED            (0x10) 
/* Bit 5: Reserved */
/* Bit 6: Car Bus signal enabled , or not = not set */
#define SIRF_MSG_SSB_GN_CAR_BUS_SIGNAL_ENABLED         (0x40) 
/* Bit 7: DR direction = reverse, or forward = not set */
#define SIRF_MSG_SSB_GN_DR_DIRECTION_REVERSE           (0x80) 

typedef struct tSIRF_MSG_SSB_GEODETIC_NAVIGATION_tag
{
   tSIRF_UINT16 nav_valid;  /* GPS validity bits; 0 is all valid               */
   tSIRF_UINT16 nav_mode;   /* GPS mode flags                                  */
   tSIRF_UINT16 gps_week;   /* GPS week number in weeks                        */
   tSIRF_UINT32 gps_tow;    /* GPS time of week in 1 ms, 0..604,799,000 ms     */
   tSIRF_UINT16 utc_year;
   tSIRF_UINT8  utc_month;
   tSIRF_UINT8  utc_day;
   tSIRF_UINT8  utc_hour;
   tSIRF_UINT8  utc_min;
   tSIRF_UINT16 utc_sec;
   tSIRF_UINT32 sv_used;    /* SVs used in fix (bitmap) */
   tSIRF_INT32 lat;        /* Latitude in 1e-7 degrees,
                              -900,000,000..900,000,000 */
   tSIRF_INT32 lon;        /* Longitude in 1e-7 degrees,
                              -1,800,000,000..1,800,000,000 */
   tSIRF_INT32 alt_ellips; /* Altitude from Ellipsoid in 0.01 meters,
                              -200,000..10,000,000 */
   tSIRF_INT32 alt_msl;    /* Altitude from Mean Sea Level in 0.01 meters,
                              -200,000..10,000,000 */
   tSIRF_UINT8  datum;
   tSIRF_UINT16 sog;        /* Speed Over Ground in 0.01 meters/sec, 0..65535  */
   tSIRF_UINT16 hdg;        /* Heading, from True North in 0.01 degrees,
                              0..36,000 */
   tSIRF_INT16 mag_var;
   tSIRF_INT16 climb_rate;
   tSIRF_INT16 heading_rate;
   tSIRF_UINT32 ehpe;
   tSIRF_UINT32 evpe;
   tSIRF_UINT32 ete;
   tSIRF_UINT16 ehve;
   tSIRF_INT32 clk_bias;
   tSIRF_UINT32 clk_bias_error;
   tSIRF_INT32 clk_offset;
   tSIRF_UINT32 clk_offset_error;
   tSIRF_UINT32 distance_travelled;
   tSIRF_UINT16 distance_travelled_error;
   tSIRF_UINT16 heading_error;
   tSIRF_UINT8  sv_used_cnt;/* Number of SVs used in fix                       */
   tSIRF_UINT8  hdop;       /* Horizontal Dillution Of Precision               */
   tSIRF_UINT8  additional_mode_info;  /*  Additional mode information         */
}  tSIRF_MSG_SSB_GEODETIC_NAVIGATION;
/*******************************************************************************
 SIRF_MSG_SSB_QUEUE_CMD_PARAM        ( 0x2B ) 0x43
 ******************************************************************************/
/* This message contains the Polled Msg ID as the message Sub ID below */
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_NI       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X80)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_NI tSIRF_MSG_SSB_INITIALIZE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X85)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC tSIRF_MSG_SSB_SET_DGPS_SOURCE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SNM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 088)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SNM tSIRF_MSG_SSB_SET_NAV_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SDM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 089)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SDM tSIRF_MSG_SSB_SET_DOP_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SDGPSM   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X8A)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SDGPSM tSIRF_MSG_SSB_SET_DGPS_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SEM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8B)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SEM tSIRF_MSG_SSB_SET_ELEV_MASK
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SPM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8C)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SPM tSIRF_MSG_SSB_SET_POWER_MASK
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SSN      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8F)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SSN tSIRF_MSG_SSB_SET_STAT_NAV
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_LP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x97)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_LP tSIRF_MSG_SSB_SET_LOW_POWER
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SSBAS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0AA)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SSBAS tSIRF_MSG_SSB_SET_SBAS_PRN

/*******************************************************************************
   SIRF_MSG_SSB_ADC_ODOMETER_DATA          ( 0x2D ) 45
*******************************************************************************/

#define SIRF_MSG_SSB_DR_DATASET_LENGTH 10

typedef struct tSIRF_MSG_SSB_ADC_ODOMETER_DATA_tag /* 10Hz */
{
   tSIRF_UINT32 current_time;     /* Tracker Time, millisecond counts */
   tSIRF_INT16 adc2_avg;         /* Averaged measurement from ADC[2] input */
   tSIRF_INT16 adc3_avg;         /* Averaged measurement from ADC[3] input */
   tSIRF_UINT16 odo_count;        /* Odometer counter measurement
                                    at the most recent 100ms tracker input */
   tSIRF_UINT8  gpio_stat;        /* GPIO input states
                                    at the most recent 100ms tracker input:
                                    bit 0: GPIO[0]  input
                                    bit 1: GPIO[2]  input
                                    bit 2: GPIO[3]  input
                                    bit 3: GPIO[4]  input
                                    bit 4: GPIO[5]  input
                                    bit 5: GPIO[7]  input
                                    bit 6: GPIO[8]  input
                                    bit 7: GPIO[12] input */
}  tSIRF_MSG_SSB_ADC_ODOMETER_DATA;

typedef struct tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ_tag /* 1Hz-10 measurements per sec*/
{
   tSIRF_MSG_SSB_ADC_ODOMETER_DATA dataset[SIRF_MSG_SSB_DR_DATASET_LENGTH];
}  tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ;
/*******************************************************************************
  SIRF_MSG_SSB_TEST_MODE_DATA_3       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2E,    0)
*******************************************************************************/
#define SIRF_MSG_SSB_TEST_MODE_PHASE_LOCK_SCALE        (0.001)
typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_3_tag
{
   tSIRF_UINT16 svid;                /* fixed SVid to search on all channels*/
   tSIRF_UINT16 period;              /* number of seconds statistics are
                                       accumulated over */
   tSIRF_UINT16 bit_synch_time;      /* time to first bit synch              */
   tSIRF_UINT16 bit_count;           /* Count of data bits came out during a
                                       period */
   tSIRF_UINT16 poor_status_count;   /* Count of 100ms periods tracker spent in
                                       any status < 3F */
   tSIRF_UINT16 good_status_count;   /* Count of 100ms periods tracker spent in
                                       status 3F */
   tSIRF_UINT16 parity_error_count;  /* Number of word parity errors          */
   tSIRF_UINT16 lost_vco_count;      /* number of msec VCO lock loss was
                                       detected */
   tSIRF_UINT16 frame_synch_time;    /* time to first frame synch             */
   tSIRF_INT16 cno_mean;            /* c/No mean in 0.1 dB-Hz                 */
   tSIRF_INT16 cno_sigma;           /* c/No sigma in 0.1 dB                   */
   tSIRF_INT16 clock_drift;         /* clock drift in 0.1 Hz                  */
   tSIRF_INT32 clock_offset;        /* clock offset in 0.1 Hz         */

   /*  For bit test at a high c/no - Test Mode 3 only */
   tSIRF_INT16 bad_1khz_bit_count;  /* bad bit count out of 10,000
                                       (10 seconds * 1000 bits )     */
   tSIRF_INT32 abs_i20ms;           /* phase noise estimate I20ms sum         */
   tSIRF_INT32 abs_q20ms;           /* phase noise estimate Q20ms sum          */

   tSIRF_INT32 phase_lock;          /* phase lock indicator. LSB = 0.001 */
   tSIRF_UINT16 rtc_frequency;      /*  RTC Frequency. Unit: Hz */
   tSIRF_UINT16 e_to_acq_ratio;     /*  ECLK to ACQ Clock ratio  */
   tSIRF_UINT8  t_sync_agc_gain;    /*  Tsync and AGC Gain value */
   tSIRF_UINT8  tm_5_ready;         /*  Ready for TM5 switch? >= 0x80
                                       Yes, else No */
   tSIRF_UINT16 ClkDriftUnc;        /* Clock Drift (Frequency) Uncertainty in Hz */

} tSIRF_MSG_SSB_TEST_MODE_DATA_3;

/*******************************************************************************
   SIRF_MSG_SSB_DR_OUTPUT              ( 0x30 ) 48
*******************************************************************************/

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_STATUS         ( 0x0130 ) 48 SID 1
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_STATUS_tag
{
   tSIRF_UINT8  nav;
   tSIRF_UINT16 data;
   tSIRF_UINT8  cal_gb_cal;
   tSIRF_UINT8  gsf_cal_ssf_cal;
   tSIRF_UINT8  nav_across_reset_pos;
   tSIRF_UINT8  hd;
   tSIRF_UINT8  gyr_sub_op_spd_sub_op;
   tSIRF_UINT8  nav_st_int_ran_z_gb_cal_upd;
   tSIRF_UINT8  gbsf_cal_upd_spd_cal_upd_upd_nav_st;
   tSIRF_UINT8  gps_upd_pos;
   tSIRF_UINT8  gps_upd_hd;
   tSIRF_UINT8  gps_pos_gps_vel;

   /* Where did these fields come from? */
   tSIRF_UINT8  dws_hd_rt_s_f_cal_valid;
   tSIRF_UINT8  dws_hd_rt_s_f_cal_upd;
   tSIRF_UINT16 dws_spd_s_f_cal_valid;
   tSIRF_UINT8  dws_spd_s_f_cal_upd;
}  tSIRF_MSG_SSB_DR_NAV_STATUS;

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_STATE          ( 0x0230 ) 48 SID 2
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_STATE_tag
{
   tSIRF_UINT16 spd;
   tSIRF_UINT16 spd_e;
   tSIRF_INT16 ssf;
   tSIRF_UINT16 ssf_e;
   tSIRF_INT16 hd_rte;
   tSIRF_UINT16 hd_rte_e;
   tSIRF_INT16 gb;
   tSIRF_UINT16 gbE;
   tSIRF_INT16 gsf;
   tSIRF_UINT16 gsf_e;
   tSIRF_UINT32 tpe;
   tSIRF_UINT16 the;
   tSIRF_UINT8  nav_ctrl;
   tSIRF_UINT8  reverse;
   tSIRF_UINT16 hd;
   /* where did these fields come from */
   tSIRF_UINT8  sensor_pkg; /* Identify which sensor package used:
                           *   0 = Gyro and Odo
                           *   1 = Wheel Speed and Odo
                           */
   tSIRF_UINT16 odo_spd;
   tSIRF_INT16 odo_spd_s_f;
   tSIRF_UINT16 odo_spd_s_f_err;
   tSIRF_INT16 lf_wheel_spd_sf;
   tSIRF_UINT16 lf_wheel_spd_sf_err;
   tSIRF_INT16 rf_wheel_spd_sf;
   tSIRF_UINT16 rf_wheel_spd_sf_err;
   tSIRF_INT16 lr_wheel_spd_sf;
   tSIRF_UINT16 lr_wheel_spd_sf_err;
   tSIRF_INT16 rr_wheel_spd_sf;
   tSIRF_UINT16 rr_wheel_spd_sf_err;
   tSIRF_INT16 rear_axle_spd_delta;
   tSIRF_UINT16 rear_axle_avg_spd;
   tSIRF_UINT16 rear_axle_spd_err;
   tSIRF_INT16 rear_axle_hd_rt;
   tSIRF_UINT16 rear_axle_hd_rt_err;
   tSIRF_INT16 front_axle_spd_delta;
   tSIRF_UINT16 front_axle_avg_spd;
   tSIRF_UINT16 front_axle_spd_err;
   tSIRF_INT16 front_axle_hd_rt;
   tSIRF_UINT16 front_axle_hd_rt_err;
}  tSIRF_MSG_SSB_DR_NAV_STATE;

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_SUBSYS         ( 0x0330 ) 48 SID 3
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_SUBSYS_tag
{
   tSIRF_INT16 gps_hd_rte;
   tSIRF_UINT16 gps_hd_rte_e;
   tSIRF_UINT16 gps_hd;
   tSIRF_UINT16 gps_hd_e;
   tSIRF_UINT16 gps_spd;
   tSIRF_UINT16 gps_spd_e;
   tSIRF_UINT32 gps_pos_e;
   tSIRF_INT16 dr_hd_rte;
   tSIRF_UINT16 dr_hd_rte_e;
   tSIRF_UINT16 dr_hd;
   tSIRF_UINT16 dr_hd_e;
   tSIRF_UINT16 dr_spd;
   tSIRF_UINT16 dr_spd_e;
   tSIRF_UINT32 dr_pos_e;
   tSIRF_UINT8  reserved[2];
}  tSIRF_MSG_SSB_DR_NAV_SUBSYS;

/*******************************************************************************
       SIRF_MSG_SSB_DR_VALID              ( 0x0530 ) 48 SID 5
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_VALID_tag
{
   tSIRF_UINT32 valid;  /* bit  0: invalid position */
                       /* bit  1: invalid position error */
                       /* bit  2: invalid heading */
                       /* bit  3: invalid heading error */
                       /* bit  4: invalid speed scale factor */
                       /* bit  5: invalid speed scale factor error */
                       /* bit  6: invalid gyro bias */
                       /* bit  7: invalid gyro bias error */
                       /* bit  8: invalid gyro scale factor */
                       /* bit  9: invalid gyro scale factor error */
                       /* bit 10: invalid baseline speed scale factor */
                       /* bit 11: invalid baseline gyro bias */
                       /* bit 12: invalid baseline gyro scale factor */
                       /* bit 13 - 31: reserved */
   tSIRF_UINT32 reserved;
}  tSIRF_MSG_SSB_DR_VALID;

/*******************************************************************************
       SIRF_MSG_SSB_DR_GYR_FACT_CAL       ( 0x0630 ) 48 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  cal;  /* bit 0: start gyro bias calibration */
                     /* bit 1: start gyro scale factor calibration */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_GYR_FACT_CAL;

/*******************************************************************************
       SIRF_MSG_SSB_DR_SENS_PARAM         ( 0x0730 ) 48 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SENS_PARAM_tag
{
   tSIRF_UINT8  base_ssf;  /* in ticks/m */
   tSIRF_UINT16 base_gb;   /* in zero rate volts */
   tSIRF_UINT16 base_gsf;  /* in mV / (deg/s) */
}  tSIRF_MSG_SSB_DR_SENS_PARAM;

/*******************************************************************************
       SIRF_MSG_SSB_DR_DATA_BLK           ( 0x0830 ) 48 SID 8
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_DATA_BLK_tag
{
   tSIRF_UINT8  meas_type;
   tSIRF_UINT8  valid_cnt;
   tSIRF_UINT16 bkup_flgs;
   struct tSIRF_DR_DATA_tag
   {
      tSIRF_UINT32 tag;
      tSIRF_UINT16 spd;
      tSIRF_INT16 hd_rte;
   } blk[SIRF_MSG_SSB_DR_DATASET_LENGTH];
}  tSIRF_MSG_SSB_DR_DATA_BLK;

/*******************************************************************************
       SIRF_MSG_SSB_MMF_STATUS            ( 0x5030 ) 48 SID 80
*******************************************************************************/
/* Map matching is enabled */
#define SIRF_MSG_SSB_STATUS_MMF_ENABLED                  0x80000000
/* Map matching calibration is enabled. */
#define SIRF_MSG_SSB_STATUS_MMF_CALIBRATION_ENABLED      0x40000000
/* Map matching retroloop is enabled. */
#define SIRF_MSG_SSB_STATUS_MMF_RETROLOOP_ENABLED        0x20000000
/* We received a MMF packet */
#define SIRF_MSG_SSB_STATUS_GOT_DATA                     0x10000000
/* Altitude updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_ALTITUDE_VALID        0x08000000
/* Heading updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_HEADING_VALID         0x04000000
/* Position updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_POSITION_VALID        0x02000000
/* Incorrect number of data sets inside MMF packet. */
#define SIRF_MSG_SSB_STATUS_INVALID_DATA_SIZE            0x01000000
#define SIRF_MSG_SSB_STATUS_HEADING_OUT_OF_RANGE         0x00800000
#define SIRF_MSG_SSB_STATUS_HEADING_DELTA_TOO_BIG        0x00400000
#define SIRF_MSG_SSB_STATUS_POSITION_DRIFT_TOO_BIG       0x00200000
/* Indicates that newer MMF packet(s) has overwritten older one before data
   was processed */
#define SIRF_MSG_SSB_STATUS_DATA_OVERFLOW                0x00100000
/* Indicates that received data was too old for processing */
#define SIRF_MSG_SSB_STATUS_DATA_TOO_OLD                 0x00080000
/* Nav was updated with MMF feedback */
#define SIRF_MSG_SSB_STATUS_NAV_UPDATED                  0x00040000
/* Nav is valid */
#define SIRF_MSG_SSB_STATUS_NAV_VALID                    0x00020000
/* Copies of MMF data packet control register */
#define SIRF_MSG_SSB_DATA_ALTITUDE_VALID                 0x00000020
#define SIRF_MSG_SSB_DATA_HEADING_VALID                  0x00000010
#define SIRF_MSG_SSB_DATA_POSITION_VALID                 0x00000008
#define SIRF_MSG_SSB_DATA_ALTITUDE_FORCED                0x00000004
#define SIRF_MSG_SSB_DATA_HEADING_FORCED                 0x00000002
#define SIRF_MSG_SSB_DATA_POSITION_FORCED                0x00000001

typedef struct tSIRF_MSG_SSB_MMF_STATUS_tag
{
   tSIRF_UINT32   status;     /* Status see above for bit definitions */
   tSIRF_UINT16   sys_hdg;    /* For the last received MMF packet this was the
                              * system's heading, LLA and TOW */
   tSIRF_INT32   sys_lat;
   tSIRF_INT32   sys_lon;
   tSIRF_INT32   sys_alt;
   tSIRF_UINT32   sys_tow;
   tSIRF_UINT16   mmf_hdg;    /* For the last received MMF packet this was the
                              * MMF's heading, LLA and TOW */
   tSIRF_INT32   mmf_lat;
   tSIRF_INT32   mmf_lon;
   tSIRF_INT32   mmf_alt;
   tSIRF_UINT32   mmf_tow;
}  tSIRF_MSG_SSB_MMF_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_SBAS_PARAM                 ( 0x32 ) 50
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SBAS_PARAM_tag
{
   tSIRF_UINT8  prn;      /* PRN                                               */
   tSIRF_UINT8  mode;     /* 0=TESTING, 1=INTEGRITY                            */
   tSIRF_UINT8  timeout;  /* 1 - 255 seconds;
                            zero will bring the timeout value to default      */
   tSIRF_UINT8  flg_bits; /* bit0: Timeout;     0=default; 1=user              */
                         /* bit1: Hlth;        0=ON; 1=OFF                    */
                         /* bit2: Corrections; 0=ON; 1=OFF                    */
                         /* bit3: SBAS PRN;    0=AutoScan; 1=Manual (user)    */
}  tSIRF_MSG_SSB_SBAS_PARAM;

/*******************************************************************************
   SIRF_MSG_SSB_SIRFNAV_NOTIFICATION   ( 0x33 ) 51
      SIRF_MSG_SSB_SIRFNAV_COMPLETE      ( 0x0133 )
      SIRF_MSG_SSB_SIRFNAV_TIMING        ( 0x0233 )
      SIRF_MSG_SSB_DEMO_TIMING           ( 0x0333 )
      SIRF_MSG_SSB_SIRFNAV_TIME_TAGS     ( 0x0433 )
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIRFNAV_COMPLETE_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_SIRFNAV_COMPLETE;

typedef struct tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS_tag
{
   tSIRF_UINT32 tracker_time_tag;  /* Tracker's time tag in [ms]*/
   tSIRF_UINT16 gps_week;          /* GPS week number in weeks */
   tSIRF_UINT32 gps_tow;           /* GPS time of week in 1 ms,
                                     0..604,799,000 ms */
   tSIRF_UINT8  tracker_rtc_day;
   tSIRF_UINT8  tracker_rtc_hour;
   tSIRF_UINT8  tracker_rtc_min;
   tSIRF_UINT8  tracker_rtc_sec;
   tSIRF_UINT16 tracker_rtc_clkctr;
   tSIRF_UINT8  tracker_rtc_ok;
   tSIRF_UINT8  tracker_rtc_rollover_complete;
   tSIRF_UINT32 tracker_rtc_rollover_seconds;
} tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS;


typedef struct tSIRF_MSG_SSB_TRACKER_LOADER_STATE_tag
{
   tSIRF_UINT32 loader_state;
   tSIRF_UINT32 percentage_loaded;
   tSIRF_UINT32 loader_error;
   tSIRF_UINT32 time_tag;

} tSIRF_MSG_SSB_TRACKER_LOADER_STATE;


typedef struct tSIRF_MSG_SSB_SIRFNAV_START_tag
{
   tSIRF_UINT32 start_mode;
   tSIRF_UINT32 clock_offset;
   tSIRF_UINT32 port_num;
   tSIRF_UINT32 baud_rate;
}  tSIRF_MSG_SSB_SIRFNAV_START;


typedef struct tSIRF_MSG_SSB_SIRFNAV_STOP_tag
{
   tSIRF_UINT32 stop_mode;
}  tSIRF_MSG_SSB_SIRFNAV_STOP;


/*******************************************************************************
      SIRF_MSG_SSB_STARTUP_INFO      ( 0x0136 ) 54 SID 1
*******************************************************************************/

/* GPS start mode */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_SNAP             (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_HOT              (0x01)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_WARM             (0x02)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_WARM_WITH_INIT   (0x03)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_COLD             (0x04)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_FACTORY          (0x05)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_FAST             (0x06)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INVALID          (0x0F)

/* start reason */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_EXTERNAL         (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INTERNAL         (0x01 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_PTF              (0x02 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_APM              (0x03 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INITIAL          (0x04 << 4)

/* restart type */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_GPS_ONLY_RESTART (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_SYSTEM_RESTART   (0x01 << 7)

/* rtc data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_RTC_DATA_INVALID     (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_RTC_DATA_VALID       (0x01)

/* storage data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_STORAGE_DATA_INVALID (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_STORAGE_DATA_VALID   (0x01 << 1)

/* flash data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_FLASH_DATA_INVALID   (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_FLASH_DATA_VALID     (0x01 << 2)

/* dr status validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_DR_STATUS_INVALID (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_DR_STATUS_VALID   (0x01)

typedef struct tSIRF_MSG_SSB_STARTUP_INFO_tag
{
   tSIRF_UINT8  mode;          /* Bit field:*/
                              /* Bits 3..0 - GPS start mode*/
                              /*         0 - snap*/
                              /*         1 - hot*/
                              /*         2 - warm*/
                              /*         3 - warm with initialization*/
                              /*         4 - cold*/
                              /*         5 - factory*/
                              /*         6 - fast*/
                              /* Bits 6..4 - start reason*/
                              /*         0 - commanded externally*/
                              /*         1 - internal reason (recovery mode)*/
                              /*         2 - Push-To-Fix commanded*/
                              /*         3 - Advanced-Power-Mode commanded*/
                              /*         4 - initial*/
                              /* Bit 7     - restart type*/
                              /*         0 - GPS only restart*/
                              /*         1 - system restart*/
   tSIRF_UINT8  status;        /* Bit field:*/
                              /* Bit 0     - RTC status*/
                              /*         0 - RTC data invalid*/
                              /*         1 - RTC data valid*/
                              /* Bit 1     - storage status*/
                              /*         0 - data in storage is not valid*/
                              /*         1 - data in storage is valid*/
                              /* Bit 2     - Flash storage status*/
                              /*         0 - data in storage is not valid*/
                              /*         1 - data in storage is valid*/
   tSIRF_UINT8  dr_status;     /* Bit field:*/
                              /* Bit 0     - DR status*/
                              /*         0 - sensor data invalid*/
                              /*         1 - sensor data valid*/
   tSIRF_UINT32 gps_off_time;  /* Number of seconds GPS was off*/
   tSIRF_UINT16 gps_week;      /* GPS week number in weeks                     */
   tSIRF_UINT32 gps_tow;       /* GPS time of week in 1 ms, 0..604,799,000 ms  */
   tSIRF_UINT16 utc_year;
   tSIRF_UINT8  utc_month;
   tSIRF_UINT8  utc_day;
   tSIRF_UINT8  utc_hour;
   tSIRF_UINT8  utc_min;
   tSIRF_UINT16 utc_sec;
   tSIRF_INT32 lat;           /* Latitude in 1e-7 degrees,
                                 -900,000,000..900,000,000 */
   tSIRF_INT32 lon;           /* Longitude in 1e-7 degrees,
                                 -1,800,000,000..1,800,000,000 */
   tSIRF_INT32 alt_ellips;    /* Altitude from Ellipsoid in 0.01 meters,
                                 -200,000..10,000,000 */
   tSIRF_UINT16 heading;       /* Heading, from True North in 0.01 degrees,
                                 0..36,000    */
   tSIRF_UINT32 valid_ephemeris;        /* SV with valid ephemeris data. Bitmap.
                                          Bit 0: SV1, bit 1: SV2, ... */
   tSIRF_UINT32 collected_almanac;      /* SV with collected almanac data,
                                          i.e. with almanac info newer than
                                          factory almanac. Bitmap.
                                          Bit 0: SV1, bit 1: SV2, ... */
   tSIRF_UINT16 collected_almanac_week; /* week of the oldest collected almanac
                                        * entry */
   tSIRF_UINT16 factory_almanac_week;   /*week of the factory collected almanac*/
   tSIRF_INT32 clk_offset;
   tSIRF_UINT32 reserved;
} tSIRF_MSG_SSB_STARTUP_INFO;

/*******************************************************************************
   SIRF_MSG_SSB_EE_GPS_TIME_INFO          ( 0x0138 ) 56 SID 1
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_GPS_TIME_INFO_tag
{
   tSIRF_UINT8  time_valid_flag;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT32 gps_tow;
   tSIRF_UINT32 ephemeris_request_mask; /* bitfield for each sat */
}tSIRF_MSG_SSB_EE_GPS_TIME_INFO;

/*******************************************************************************
   SIRF_MSG_SSB_EE_INTEGRITY         ( 0x0238 ) 56 SID 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_INTEGRITY_tag
{
   tSIRF_UINT32 sat_pos_validity_mask;
   tSIRF_UINT32 sat_clk_validity_mask;
   tSIRF_UINT32 sat_health_mask;
}tSIRF_MSG_SSB_EE_INTEGRITY;

/*******************************************************************************
   SIRF_MSG_SSB_EE_STATE         ( 0x0338 ) 56 SID 3
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_STATE_tag
{
   tSIRF_UINT8 svid;
   tSIRF_UINT8 eph_source_flag;
   tSIRF_UINT16 week_no;
   tSIRF_UINT16 toe;              /* in seconds */
   tSIRF_UINT8 eph_integrity;
   tSIRF_UINT8 eph_age;           /* in days    */
}tSIRF_MSG_SSB_EE_STATE;

/*******************************************************************************
   SIRF_MSG_SSB_EE_CLK_BIAS_ADJ         ( 0x0438 ) 56 SID 4
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 toe;
   tSIRF_INT32  clk_bias_adjust;
}tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ;

/*******************************************************************************
   SIRF_MSG_SSB_EE_ACK               ( 0xFF38 ) 56 SID 255
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_ACK_tag
{
   tSIRF_UINT8 acq_id;
   tSIRF_UINT8 acq_sub_id;
}tSIRF_MSG_SSB_EE_ACK;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA_7    ( 0x073F ) 63 SID 7
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_7_tag
{
   tSIRF_UINT8  test_status;        /* 0 = in progress, 1 = complete */
   tSIRF_UINT32 spur1_frequency;    /* first spur frequency */
   tSIRF_UINT16 spur1_sig_to_noise; /* first spur power */
   tSIRF_UINT32 spur2_frequency;    /* second spur frequency */
   tSIRF_UINT16 spur2_sig_to_noise; /* second spur power */
   tSIRF_UINT32 spur3_frequency;    /* third spur frequency */
   tSIRF_UINT16 spur3_sig_to_noise; /* third spur power */
   tSIRF_UINT32 spur4_frequency;    /* fourth spur frequency */
   tSIRF_UINT16 spur4_sig_to_noise; /* fourth spur power */
}tSIRF_MSG_SSB_TEST_MODE_DATA_7;

typedef struct tSIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE_tag
{
   tSIRF_UINT16 gpio_state; /* GPIO0 state is on bit 0, GPIO1 on bit 1, etc. */

} tSIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE;

/*******************************************************************************
   SIRF_MSG_SSB_DOP_VALUES             ( 0x42 ) 66
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DOP_VALUES_tag
{
   tSIRF_UINT32  gps_tow;   /* GPS time of week in 1 ms, 0..604,799,000 ms*/
   tSIRF_UINT16  gdop;      /* scaled by 10*/
   tSIRF_UINT16  pdop;      /* scaled by 10*/
   tSIRF_UINT16  hdop;      /* scaled by 10*/
   tSIRF_UINT16  vdop;      /* scaled by 10*/
   tSIRF_UINT16  tdop;      /* scaled by 10*/
}  tSIRF_MSG_SSB_DOP_VALUES;

/*******************************************************************************
   SIRF_MSG_SSB_INITIALIZE             ( 0x80 ) 128
*******************************************************************************/

/* configuration flags:*/
#define SIRF_MSG_SSB_RESTART_HOT                   0x00
#define SIRF_MSG_SSB_RESTART_WARM_NOINIT           0x02
#define SIRF_MSG_SSB_RESTART_WARM_INIT             0x03
#define SIRF_MSG_SSB_RESTART_COLD                  0x04
#define SIRF_MSG_SSB_RESTART_FACTORY               0x08

/* additional flags:*/
#define SIRF_MSG_SSB_RESTART_RAW_TRK_OUT           0x10
#define SIRF_MSG_SSB_RESTART_TEXTOUT               0x20  /* Enable debug data output */

typedef struct tSIRF_MSG_SSB_INITIALIZE_tag
{
   tSIRF_INT32 ecef_x;        /* Position - X in meters */
   tSIRF_INT32 ecef_y;        /* Position - Y in meters */
   tSIRF_INT32 ecef_z;        /* Position - Z in meters */
   tSIRF_INT32 clk_offset;    /* Clock Offset in Hz */
   tSIRF_UINT32 gps_tow;       /* GPS Time of Week, in 10ms */
   tSIRF_UINT16 gps_week;      /* Week number */
   tSIRF_UINT8  chnl_cnt;      /* Number of channels */
   tSIRF_UINT8  restart_flags; /* Restart Configuration */
} tSIRF_MSG_SSB_INITIALIZE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_NMEA_MODE          ( 0x81 ) 129
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_NUM_NMEA_MSG ( 10 )

typedef struct tSIRF_MSG_SSB_NMEA_CFG_tag
{
   tSIRF_UINT8  rate;
   tSIRF_UINT8  cksum;
} tSIRF_MSG_SSB_NMEA_CFG;

typedef struct tSIRF_MSG_SSB_SET_NMEA_MODE_tag
{
   tSIRF_UINT8        mode;
   tSIRF_MSG_SSB_NMEA_CFG nmea_cfg[SIRF_MSG_SSB_MAX_NUM_NMEA_MSG];
   tSIRF_UINT16       baud_rate;
}  tSIRF_MSG_SSB_SET_NMEA_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_ALMANAC            ( 0x82 ) 130
*******************************************************************************/

#define SIRF_MSG_SSB_ALMANAC_DATA_ENTRY      ( 14 )
#define SIRF_MSG_SSB_ALMANAC_SIZE       (SIRF_MAX_SVID_CNT*SIRF_MSG_SSB_ALMANAC_DATA_ENTRY)

typedef struct tSIRF_MSG_SSB_ALM_DATA_tag
{
   tSIRF_INT16 data[SIRF_MSG_SSB_ALMANAC_DATA_ENTRY];
} tSIRF_MSG_SSB_ALM_DATA;

typedef struct tSIRF_MSG_SSB_SET_ALMANAC_tag
{
   tSIRF_MSG_SSB_ALM_DATA almanac[SIRF_MAX_SVID_CNT];
}  tSIRF_MSG_SSB_SET_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_SW_VERSION        ( 0x84 ) 132
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_SW_VERSION_tag
{
   tSIRF_UINT8  reserved; /* ignored */
}  tSIRF_MSG_SSB_POLL_SW_VERSION;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DGPS_SOURCE        ( 0x85 ) 133
*******************************************************************************/

#define SIRF_MSG_SSB_DGPS_SRC_NONE         0  /* Use no corrections       */
#define SIRF_MSG_SSB_DGPS_SRC_SBAS         1  /* Use SBAS channel         */
#define SIRF_MSG_SSB_DGPS_SRC_USER         2  /* Use external receiver    */
#define SIRF_MSG_SSB_DGPS_SRC_INTERNAL     3  /* Use internal DGPS Beacon */
#define SIRF_MSG_SSB_DGPS_SRC_SOFTWARE     4  /* Corrections provided using a module
                                        interface routine in a custon user
                                        application    */

typedef struct tSIRF_MSG_SSB_SET_DGPS_SOURCE_tag
{
   tSIRF_UINT8  src;
   tSIRF_UINT32 freq;           /* unused */
   tSIRF_UINT8  bit_rate;       /* unused */
}  tSIRF_MSG_SSB_SET_DGPS_SOURCE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_NAV_MODE           ( 0x88 ) 136
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_NAV_MODE_tag
{
   tSIRF_UINT8  enable_3d;        /* TRUE/FALSE                */
   tSIRF_UINT8  enable_con_alt;   /* TRUE/FALSE                */
   tSIRF_UINT8  degraded_mode;    /* 0=d_t, 1=t_d, 2=d_only,
                                    3=t_only, 4=disabled*/
   tSIRF_UINT8  pad;              /* TRUE                      */
   tSIRF_UINT8  dr_enable;        /* TRUE/FALSE                */
   tSIRF_INT16 alt_input;        /* -1000..10000 m            */
   tSIRF_UINT8  alt_mode;         /* 0=auto, 1=always, 2=never */
   tSIRF_UINT8  alt_src;          /* 0=last, 1=fixed, 2=dynamic*/
   tSIRF_UINT8  coast_timeout;    /* 0 to 120 sec, def 2  sec  */
   tSIRF_UINT8  degraded_timeout; /* 1 to 120 sec, def 20 sec  */
   tSIRF_UINT8  dr_timeout;       /* 1 to 120 sec, def 5  sec  */
   tSIRF_UINT8  trk_smooth;       /* Track Smooth Algorithm enable/disable */
}  tSIRF_MSG_SSB_SET_NAV_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DOP_MODE           ( 0x89 ) 137
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_DOP_MODE_tag
{
   tSIRF_UINT8  mode;     /* 0=auto P/H, 1=PDOP ,2=HDOP, 3=GDOP, 4=never */
   tSIRF_UINT8  gdop_th;  /* 1 to 50, def 8                     */
   tSIRF_UINT8  pdop_th;  /* 1 to 50, def 8                     */
   tSIRF_UINT8  hdop_th;  /* 1 to 50, def 8                     */
}  tSIRF_MSG_SSB_SET_DOP_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DGPS_MODE          ( 0x8A ) 138
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_DGPS_MODE_tag
{
   tSIRF_UINT8  mode;
   tSIRF_UINT8  timeout;
}  tSIRF_MSG_SSB_SET_DGPS_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_ELEV_MASK          ( 0x8B ) 139
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_ELEV_MASK_tag
{
   tSIRF_INT16  trk;  /* -200 to 900 1/10 deg, def 50 */
   tSIRF_INT16  nav;  /* -200 to 900 1/10 deg, def 75 */
}  tSIRF_MSG_SSB_SET_ELEV_MASK;

/*******************************************************************************
   SIRF_MSG_SSB_SET_POWER_MASK         ( 0x8C ) 140
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_POWER_MASK_tag
{
   tSIRF_UINT8  trk;  /* 20 to 50 dBHz, def 28 */
   tSIRF_UINT8  nav;  /* 20 to 50 dBHz, def 30 */
}  tSIRF_MSG_SSB_SET_POWER_MASK;

/*******************************************************************************
   SIRF_MSG_SSB_SET_STAT_NAV           ( 0x8F ) 143
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_STAT_NAV_tag
{
   tSIRF_UINT8  staticnav;
}  tSIRF_MSG_SSB_SET_STAT_NAV;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_CLOCK_STATUS      ( 0x90 ) 144
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_CLOCK_STATUS_tag
{
   tSIRF_UINT8  reserved; /* ignored */
}  tSIRF_MSG_SSB_POLL_CLOCK_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_ALMANAC           ( 0x92 ) 146
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_ALMANAC_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_POLL_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_EPHEMERIS         ( 0x93 ) 147
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_EPHEMERIS_tag
{
   tSIRF_UINT8  svid; /* 0 = send all Ephemeris */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_POLL_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_SET_EPHEMERIS          ( 0x95 ) 149
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_EPHEMERIS_tag
{
   tSIRF_UINT16 subframe[3][15];
}  tSIRF_MSG_SSB_SET_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_SET_OP_MODE            ( 0x96 ) 150
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_OP_MODE_tag
{
   tSIRF_UINT16 mode;         /**< Op Mode:  0 = Normal, 0x1E51 = Test,
                                             0x1e52 - test2, . . . */
   tSIRF_UINT16 svid;         /**< SVid to search for (in test mode)   */
   tSIRF_UINT16 period;       /**< output message period (test mode)   */
   tSIRF_UINT8  tm5Cmd;       /**< Testmode 5 Low Power Command 0=No 1=Yes */
}  tSIRF_MSG_SSB_SET_OP_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_LOW_POWER          ( 0x97 ) 151
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_LOW_POWER_tag
{
   tSIRF_UINT16 push_to_fix;
   tSIRF_UINT16 duty_cycle;
   tSIRF_UINT32 on_time;
}  tSIRF_MSG_SSB_SET_LOW_POWER;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_RECEIVER_PARAMS   ( 0x98 ) 152
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS;

/*******************************************************************************
   SIRF_MSG_SSB_SIRFNAV_COMMAND        ( 0xA1 ) 161
*******************************************************************************/

#define SIRF_MSG_SSB_RESTART_MODE_REINITIALIZE       ( 0x00 )
#define SIRF_MSG_SSB_RESTART_MODE_START_STOP         ( 0x01 )

typedef struct tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE_tag
{
   tSIRF_UINT8  control;
}  tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE;

typedef struct tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS_tag
{
   tSIRF_UINT16 TaskID ;
   tSIRF_UINT32 PeriodOn ;
   tSIRF_UINT32 PeriodOff ;
   tSIRF_UINT32 LoadDuration ;
   tSIRF_UINT32 FullLoadDuration ;
   tSIRF_UINT32 OffsetTime ;
}  tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS;

typedef struct tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE_tag
{
   tSIRF_UINT32 start_mode;
   tSIRF_UINT32 clock_offset;
   tSIRF_UINT32 port_num;
   tSIRF_UINT32 baud_rate;
}  tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE;

typedef struct tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE_tag
{
   tSIRF_UINT32 stop_mode;
}  tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE;

typedef struct tSIRF_MSG_SSB_DEMO_STOP_TEST_APP_tag
{
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_DEMO_STOP_TEST_APP;

typedef struct tSIRF_MSG_SSB_SIRFNAV_STORE_NOW_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_SIRFNAV_STORE_NOW;

/*******************************************************************************
   SIRF_MSG_SSB_SET_MSG_RATE           ( 0xA6 ) 166
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_MSG_RATE_tag
{
   tSIRF_UINT8  mode;        /* mode: 0= set rate, 1 = query */
   tSIRF_UINT8  msg_id;      /* msg id to control */
   tSIRF_UINT8  rate;        /* rate of xmission (0: disable) */
   tSIRF_UINT8  sub_id;      /* sub id to control */
   tSIRF_UINT8  reserved[3]; /* reserved */
}  tSIRF_MSG_SSB_SET_MSG_RATE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_LOW_POWER_PARAMS   ( 0xA7 ) 167
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS_tag
{
   tSIRF_UINT32 MaxOffTime;
   tSIRF_UINT32 MaxAcqTime;
   tSIRF_UINT32 push_to_fix_period;
   tSIRF_UINT16 adaptive_tp;
}  tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS;


/*******************************************************************************
   SIRF_MSG_SSB_POLL_CMD_PARAM   ( 0xA8 ) 168
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_CMD_PARAM_tag
{
   tSIRF_UINT8 req_msg_id;
} tSIRF_MSG_SSB_POLL_CMD_PARAM;

/*******************************************************************************
   SIRF_MSG_SSB_SET_SBAS_PRN           ( 0xAA ) 170
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_SBAS_PRN_tag
{
   tSIRF_UINT8  prn;
   tSIRF_UINT32 reserved[2];
}  tSIRF_MSG_SSB_SET_SBAS_PRN;


/*******************************************************************************
      SIRF_MSG_SSB_ADVANCED_NAV_INIT ( 0x00AC ) 172  SID 0
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_ADVANCED_NAV_INIT_tag
{
   struct
   {
      tSIRF_UINT32 timeMode     : 3;  /* see resetInitModeEnum */
      tSIRF_UINT32 posMode      : 3;  /* see resetInitModeEnum */
      tSIRF_UINT32 clkMode      : 3;  /* see resetInitModeEnum */
      tSIRF_UINT32 resetEphem   : 1;  /* 1 to clear ephemeris */
      tSIRF_UINT32 fullReset    : 1;  /* 1 for full reset, 0 for gps reset only */
   } control;
   tSIRF_INT32  timeSkew;     /* time skew in microseconds */
   tSIRF_UINT32 timeUncert;   /* time uncertainty in microseconds */
   tSIRF_INT32  pos[3];       /* position x,y,z in m */
   tSIRF_UINT16 horzUncert;   /* horizontal position uncertainty in km */
   tSIRF_UINT16 vertUncert;   /* vertical position uncertainty in km */
   tSIRF_INT32  clkSkew;      /* clock skew in Hz */
   tSIRF_UINT32 clkUncert;    /* clock uncertainty in Hz */
   /** Note that this message actually contains 6 more reserved bytes */
} tSIRF_MSG_SSB_ADVANCED_NAV_INIT;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_INIT       ( 0x01AC )
      SIRF_MSG_SSB_DR_SET_NAV_MODE       ( 0x02AC )
      SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   ( 0x03AC )
      SIRF_MSG_SSB_DR_SET_SENS_PARAM     ( 0x04AC )
      SIRF_MSG_SSB_DR_POLL_VALID         ( 0x05AC )
      SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  ( 0x06AC )
      SIRF_MSG_SSB_DR_POLL_SENS_PARAM    ( 0x07AC )
      SIRF_MSG_SSB_DR_CAR_BUS_DATA       ( 0x09AC )
      SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    ( 0x0AAC )
      SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   ( 0x0BAC )
      SIRF_MSG_SSB_MMF_DATA              ( 0x50AC )
      SIRF_MSG_SSB_MMF_SET_MODE          ( 0x51AC )
*******************************************************************************/

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_INIT       ( 0x01AC ) 172 SID 1
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DR_SET_NAV_INIT_tag
{
   tSIRF_INT32 lat;          /* in deg                                 */
   tSIRF_INT32 lon;          /* in deg                                 */
   tSIRF_INT32 alt_ellips;   /* in m; from ellipsoid                   */
   tSIRF_UINT16 heading;      /* in deg; true                           */
   tSIRF_INT32 clk_offset;   /* Clock Offset in Hz                     */
   tSIRF_UINT32 gps_tow;      /* GPS Time of Week, in seconds           */
   tSIRF_UINT16 gps_week;     /* Week number                            */
   tSIRF_UINT8  chnl_cnt;     /* Number of channels                     */
   tSIRF_UINT8  restart_flags;/* Reset Configuration                    */
                             /*   Bit 0: Init Data Valid Bit, 1-valid  */
                             /*   Bit 1: Clear Ephemeris Flag, 1-clear */
                             /*   Bit 2: Clear Memory Flag, 1-clear    */
}  tSIRF_MSG_SSB_DR_SET_NAV_INIT;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_MODE       ( 0x02AC ) 172 SID 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DR_SET_NAV_MODE_tag
{
   tSIRF_UINT8  mode;    /* bit 0: GPS nav only                                */
                        /* bit 1: DR nav okay with stored/default calibration */
                        /* bit 2: DR nav okay with current GPS calibration    */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_SET_NAV_MODE;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   ( 0x03AC ) 172 SID 3
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  cal;     /* bit 0: start gyro bias calibration         */
                        /* bit 1: start gyro scale factor calibration */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_SENS_PARAM     ( 0x04AC ) 172 SID 4
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SET_SENS_PARAM_tag
{
   tSIRF_UINT8  base_ssf;  /* in ticks/m                    */
   tSIRF_UINT16 base_gb;   /* in zero rate volts            */
   tSIRF_UINT16 base_gsf;  /* in mV / (deg/s)               */
}  tSIRF_MSG_SSB_DR_SET_SENS_PARAM;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_VALID         ( 0x05AC ) 172 SID 5
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_VALID_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_VALID;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  ( 0x06AC ) 172 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_SENS_PARAM    ( 0x07AC ) 172 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_SENS_PARAM_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_SENS_PARAM;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_DATA       ( 0x09AC ) 172 SID 9
*******************************************************************************/
typedef struct
{
   tSIRF_UINT8      valid_sensor_indication;
   tSIRF_UINT32     data_set_time_tag;
   tSIRF_UINT16     odometer_speed;
   tSIRF_INT16     data1;
   tSIRF_INT16     data2;
   tSIRF_INT16     data3;
   tSIRF_INT16     data4;
   tSIRF_UINT8      reserved;
} tSIRF_MSG_SSB_DR_CAR_BUS_DATA_SET;

typedef struct
{
   tSIRF_UINT8                 sensor_data_type;
   tSIRF_UINT8                 num_of_data_sets;
   tSIRF_UINT16                reverse_bitmap;
   tSIRF_MSG_SSB_DR_CAR_BUS_DATA_SET   data[11];
} tSIRF_MSG_SSB_DR_CAR_BUS_DATA;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    ( 0x0AAC ) 172 SID 10
*******************************************************************************/
typedef struct
{
   tSIRF_UINT32  mode;
} tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   ( 0x0BAC ) 172 SID 11
*******************************************************************************/
typedef struct
{
   tSIRF_UINT32  mode;
} tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED;
/*******************************************************************************
      SIRF_MSG_SSB_MMF_DATA              ( 0x50AC ) 172 SID 80
*******************************************************************************/
typedef struct
{
   tSIRF_INT32     lat;                /* deg; scaling 1e-7*/
   tSIRF_INT32     lon;                /* deg; scaling 1e-7*/
   tSIRF_UINT32     horiz_pos_uncert;   /* m;   scaling 0.01*/
   tSIRF_INT32     alt_ellips;         /* m;   scaling 0.1*/
   tSIRF_UINT32     vert_pos_uncert;    /* m;   scaling 0.1*/
   tSIRF_UINT16     heading;            /* deg; scaling 0.01*/
   tSIRF_UINT16     heading_uncert;     /* deg; scaling 0.01*/
   tSIRF_UINT16     reserved;
} tSIRF_MSG_SSB_MMF_DATA_SET;

typedef struct
{
   tSIRF_UINT32     gps_tow_reference;  /* GPS TOW (sec; scaling 1e-3) for the */
                                       /* original NAV data from which*/
                                       /*   the MMF data is derived*/
   tSIRF_UINT8      num_of_data_sets;   /* 1 to 3*/
   tSIRF_UINT8      control;            /* Bit 0 = 1: Position Forced*/
                                       /* Bit 1 = 1: Heading Forced*/
                                       /* Bit 2 = 1: Altitude Forced*/
                                       /* Bit 3 = 1: Position is Valid*/
                                       /* Bit 4 = 1: Heading is Valid*/
                                       /* Bit 5 = 1: Altitude is Valid*/
                                       /* Bits 6-7: reserved*/
   tSIRF_MSG_SSB_MMF_DATA_SET  mmf_data[3];
} tSIRF_MSG_SSB_MMF_DATA;

/*******************************************************************************
      SIRF_MSG_SSB_MMF_SET_MODE          ( 0x51AC ) 172 SID 81
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_MMF_SET_MODE_tag
{
   tSIRF_UINT32  mode;   /* Bit  0 = 1: Enable map matching*/
                        /* Bit  1 = 1: Enable map matching callibration*/
                        /* Bit  2 = 1: Enable map matching retro loop*/
                        /* Bit 16 = 1: Disable map matching*/
                        /* Bit 17 = 1: Disable map matching callibration*/
                        /* Bit 18 = 1: Disable map matching retro loop*/
}  tSIRF_MSG_SSB_MMF_SET_MODE;

/*******************************************************************************
      SIRF_MSG_SSB_TRK_HW_CONFIG          ( 0xCE ) 206
*******************************************************************************/

#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_IGNORE   (0)
#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_INTERNAL (1)
#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_EXTERNAL (2)

#define SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH (16)

typedef struct tSIRF_MSG_SSB_TRK_HW_CONFIG_tag
{
   /* External/Internal Enable: 0=Ignore, 1=Internal, 2=External */
   tSIRF_UINT8 ExtLNAEnable;
   /* SSB not allowed to change these Reserved fields */
   tSIRF_UINT8 Reserved;
   tSIRF_UINT8 Reserved1[SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH];
} tSIRF_MSG_SSB_TRK_HW_CONFIG;

/*******************************************************************************
SIRF_MSG_SSB_SW_COMMANDED_OFF SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD, 0x10)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SW_COMMANDED_OFF_tag
{
   tSIRF_UINT8 reserved; /* No field for this message */
} tSIRF_MSG_SSB_SW_COMMANDED_OFF;


/*******************************************************************************
      SIRF_MSG_SSB_TRK_HW_TEST_CONFIG          ( 0xCF ) 207
*******************************************************************************/

#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_IGNORE               (0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_ENABLE               (1)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_DISABLE              (2)

#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_IGNORE       (0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_ENABLE       (1)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_DISABLE      (2)

typedef struct tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG_tag
{
   tSIRF_UINT8 RFTestPoint;   /* RF Test Point   0=Ignore, 1=Enable, 2=Disable */
   tSIRF_UINT8 INTCpuPause;   /* INT_CPUPause      0=Ignore, 1=Enable, 2=Disable */

} tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG;

typedef struct tSIRF_MSG_SSB_SET_IF_TESTPOINT_tag
{
   tSIRF_UINT8 on_off; /* 0 = off; 1 = On */
} tSIRF_MSG_SSB_SET_IF_TESTPOINT;

/*******************************************************************************
#define GPS_SSB_SET_IF_TESTPOINT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xXX, 0x02)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SET_LNA_tag
{
   tSIRF_UINT8 internal_external; /* 0 = External, 1 = Internal */
} tSIRF_MSG_SSB_SET_LNA;



















/*******************************************************************************
   SIRF_MSG_SSB_TEXT                   ( 0xFF ) 255
*******************************************************************************/

#define SIRF_MSG_SSB_MAX_TEXT_LEN 250

typedef struct tSIRF_MSG_SSB_COMM_MESSAGE_TEXT_tag
{
   tSIRF_CHAR msg_text[SIRF_MSG_SSB_MAX_TEXT_LEN];
}  tSIRF_MSG_SSB_COMM_MESSAGE_TEXT;

typedef tSIRF_MSG_SSB_COMM_MESSAGE_TEXT  tSIRF_NAV_TEXT;
typedef tSIRF_MSG_SSB_COMM_MESSAGE_TEXT  tSIRF_COMM_MESSAGE_TR_TEXT;
/*    Later Additions  *********************/


/* Msg ID 0xAB --> MID_SiRFLocIn */
typedef struct tSIRF_MSG_SSB_SIRFLOC_IN_tag
{
   tSIRF_UINT8 sub_id;
   tSIRF_UINT16 entry;
} tSIRF_MSG_SSB_SIRFLOC_IN;

/*Msg ID 0xE0 */

typedef struct tSIRF_MSG_SSB_DATA_PEEKPOKE_tag
{
   tSIRF_UINT8 cmd;
   tSIRF_UINT8 mode;
   tSIRF_UINT32 address;
   tSIRF_UINT32 data;
} tSIRF_MSG_SSB_DATA_PEEKPOKE;

/*******************************************************************************
   SIRF_MSG_SSB_EE_INPUT                   ( 0xFF ) 255
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_INPUT_tag
{
   tSIRF_UINT8 data[500];
}tSIRF_MSG_SSB_EE_INPUT;

typedef struct tSIRF_MSG_SSB_EXTENDED_EPHEMERIS_tag
{
   tSIRF_UINT8  PRN;
   tSIRF_UINT8  ephemerisValidityFlag;
   tSIRF_UINT8  URA;
   tSIRF_UINT8  IODE;
   tSIRF_INT16  Crs;
   tSIRF_INT16  deltaN;
   tSIRF_INT32  M0;
   tSIRF_INT16  Cuc;
   tSIRF_UINT32 eccen;
   tSIRF_INT16  Cus;
   tSIRF_UINT32 sqrtA;
   tSIRF_UINT16 toe;
   tSIRF_INT16  Cic;
   tSIRF_INT32  omega0;
   tSIRF_INT16  Cis;
   tSIRF_INT32  i0;
   tSIRF_INT16  Crc;
   tSIRF_INT32  w;
   tSIRF_INT32  omegaDot;
   tSIRF_INT16  iDot;
   tSIRF_UINT16 toc;
   tSIRF_INT8   Tgd;
   tSIRF_INT8   af2;
   tSIRF_INT16  af1;
   tSIRF_INT32  af0;
   tSIRF_UINT8  age;
} tSIRF_MSG_SSB_EXTENDED_EPHEMERIS;

typedef struct tSIRF_MSG_SSB_BROADCAST_EPHEMERIS_tag
{
   tSIRF_UINT8  eph_flag;
   tSIRF_UINT8  sv_prn_num;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT8  URA;
   tSIRF_UINT8  IODE;
   tSIRF_INT16  Crs;
   tSIRF_INT16  deltaN;
   tSIRF_INT32  M0;
   tSIRF_INT16  Cuc;
   tSIRF_UINT32 eccen;
   tSIRF_INT16  Cus;
   tSIRF_UINT32 sqrtA;
   tSIRF_UINT16 toe;
   tSIRF_INT16  Cic;
   tSIRF_INT32  omega0;
   tSIRF_INT16  Cis;
   tSIRF_INT32  i0;
   tSIRF_INT16  Crc;
   tSIRF_INT32  w;
   tSIRF_INT32  omegaDot;
   tSIRF_INT16  iDot;
   tSIRF_UINT16 toc;
   tSIRF_INT8   Tgd;
   tSIRF_INT8   af2;
   tSIRF_INT16  af1;
   tSIRF_INT32  af0;
   tSIRF_UINT8  age;
} tSIRF_MSG_SSB_BROADCAST_EPHEMERIS;

typedef struct tSIRF_MSG_SSB_EXTENDED_IONO_tag
{
   tSIRF_INT8  alpha[4];
   tSIRF_INT8  beta[4];
} tSIRF_MSG_SSB_EXTENDED_IONO;

#if defined (LPL_CLM) || defined (GSW_CLM)
#define SV_PER_PACKET                          ( SIRF_MAX_SVID_CNT )
#else
#define SV_PER_PACKET                          ( 8 )
#endif

#ifdef LPL_CLM
#define SIRF_MSG_SSB_EE_AIDED_TIME_FLAG_VALID    1
#define SIRF_MSG_SSB_EE_AIDED_IONO_FLAG_VALID    1
#endif


typedef struct tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH_tag
{
   tSIRF_UINT16                       week;
   tSIRF_UINT32                       svid_mask;
   tSIRF_MSG_SSB_EXTENDED_EPHEMERIS   extended_ephemeris[SV_PER_PACKET];
   tSIRF_MSG_SSB_EXTENDED_IONO        extended_iono;
} tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH;

typedef struct tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG_tag
{
   tSIRF_UINT32 debug_flags;
}tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG;

/*******************************************************************************
#define SIRF_MSG_SSB_EE_DISABLE_EE_SECS SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFE)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_DISABLE_EE_SECS_tag
{
   tSIRF_UINT32 num_secs;  /* Number of seconds to wait before disabling CGEE generation */
}tSIRF_MSG_SSB_EE_DISABLE_EE_SECS;

#ifdef LPL_CLM
typedef struct tSIRF_MSG_SSB_EE_SEND_BROADCAST_EPH_tag
{
   tSIRF_UINT8                         iono_flag;
   tSIRF_MSG_SSB_EXTENDED_IONO         extended_iono;
   tSIRF_UINT8                         time_flag;
   tSIRF_UINT16                        extd_gps_week;
   tSIRF_UINT32                        gps_tow;
   tSIRF_UINT8                         num_eph;
   tSIRF_MSG_SSB_BROADCAST_EPHEMERIS   eph[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_EE_SEND_BROADCAST_EPH;
#endif /* LPL_CLM */

/*    Later Additions  *********************/
/*******************************************************************************
#define SIRF_MSG_SSB_SET_REGISTER    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xXX, 0x03)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_REGISTER_tag
{
   tSIRF_UINT32 reg;   /* 32 bit address of the register */
   tSIRF_UINT32 value; /* Value to set the register with */
} tSIRF_MSG_SSB_SET_REGISTER;

/*******************************************************************************
 * These are message structs left over from the gps_messages.h file.
 * @todo these all should be removed
 ******************************************************************************/
/******************************************************************************/
/*
 * for output message QueueError
 */
typedef struct tSIRF_MSG_SSB_QUEUE_ERROR_tag
{
   tSIRF_UINT16 ErrId;
   tSIRF_UINT16 count;
   tSIRF_UINT32* pPARAMS;
}tSIRF_MSG_SSB_QUEUE_ERROR;

/* for message QueueAlm (SSBOUT.C ) : indirectly related to message Id 146(0x92)Poll Almanac */


typedef struct tSIRF_MSG_SSB_QUEUE_ALM_ALL_tag
{
   /* This struct contains entire internal format almanac, size=32x14 */
   tSIRF_INT16      data[SIRF_MSG_SSB_ALMANAC_SIZE];

}tSIRF_MSG_SSB_QUEUE_ALM_ALL;

typedef struct tSIRF_MSG_SSB_QUEUE_ALM_RECORD_tag
{
   tSIRF_UINT8      svID;
   /* Data is 2 bytes=week&status then 12 bytes Alm Data = 14 */
   tSIRF_INT16      data[SIRF_MSG_SSB_ALMANAC_DATA_ENTRY];
}tSIRF_MSG_SSB_QUEUE_ALM_RECORD;

typedef struct tSIRF_MSG_SSB_SET_LOWPOWER_tag /* Added in order to accomodate the dutycycle field */
{
   tSIRF_INT16 DutyCycle;
   tSIRF_MSG_SSB_SET_LOW_POWER LowPower;
}tSIRF_MSG_SSB_SET_LOWPOWER;

typedef struct tSIRF_MSG_SSB_USER_PARAMS_tag
{
   tSIRF_UINT32  UserTasksEnabled;
   tSIRF_INT32  UserTaskInterval;  /* in milliseconds */
} tSIRF_MSG_SSB_USER_PARAMS;

 typedef struct tSIRF_MSG_SSB_DATA_RECEIVER_PARAMS_tag
{
   tSIRF_MSG_SSB_SET_NAV_MODE                RP_NavModeMask;
   tSIRF_MSG_SSB_SET_DOP_MODE                RP_DOP_Mask;
   tSIRF_MSG_SSB_SET_ELEV_MASK               RP_ElevMask;
   tSIRF_MSG_SSB_SET_POWER_MASK              RP_PwrMask;
   tSIRF_MSG_SSB_SET_DGPS_MODE               RP_DGPS_Mode;
   tSIRF_MSG_SSB_SET_LOW_POWER               RP_LP_Param;
   tSIRF_MSG_SSB_USER_PARAMS                     RP_userParam;
   tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS        RP_LP_AcqParam;
   tSIRF_UINT8                           RP_staticNav;
   tSIRF_INT32                          RP_correctionSrc;
   tSIRF_UINT8                           RP_enable3SVLSQ;
}tSIRF_MSG_SSB_DATA_RECEIVER_PARAMS;

/*
 * for input message HandleSetAdvNavInit
 */

/*******************************************************************************
   Ephemeris age, sent by CLM
*******************************************************************************/
typedef struct tSIRF_MSG_EE_AGE_tag
{
   tSIRF_UINT16 gps_week;     /* GPS week number in weeks */
   tSIRF_UINT32 gps_tow;      /* GPS time of week in seconds */
   tSIRF_UINT8  indicator;    /* 0 means age is valid */
   tSIRF_FLOAT  age;
   tSIRF_UINT8  age_in_days;
} tSIRF_MSG_EE_AGE;


/*******************************************************************************
   Extended Ephemeris input file information. The input file is provided
   from downloader when CLM operates in SGEE mode. The information contains the
   received buffer and the size of the extended ephemeris data.
*******************************************************************************/
typedef struct tSIRF_SSB_EE_FILE_PART_INFO_tag
{
   tSIRF_UINT32   file_size;    /* extended ephemeris data size */
   tSIRF_HANDLE   file_addr;    /* address of buffer containing EE data*/
}tSIRF_SSB_EE_FILE_PART_INFO;

/*
** SIRF_MSG_SSB_EE_FILE_PART_STATUS - Status of Extended Ephemeris buffer
*/
#define SIRF_MSG_SSB_EE_FILE_PART_STATUS_SUCCESS     ( 0x0 )
#define SIRF_MSG_SSB_EE_FILE_PART_STATUS_FAILED      ( 0x1 )

/*******************************************************************************
   Extended Ephemeris file processing status.
   The status contains information about the file/buffer status processed by CLM.
*******************************************************************************/
typedef struct tSIRF_SSB_EE_FILE_PART_STATUS_tag
{
   tSIRF_UINT8   status;        /* EE file part status/acknowledgement*/
}tSIRF_SSB_EE_FILE_PART_STATUS;

#endif /* __SIRF_MSG_SSB__H__ */


/**
 * @}
 */

