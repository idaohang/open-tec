/**
 * @addtogroup platform_src_sirf_include
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
 */

#ifndef SIRF_MSG_F_H_INCLUDED
#define SIRF_MSG_F_H_INCLUDED

/*-----------------------------------------------------------------------------
 *   Include Files
 *----------------------------------------------------------------------------*/

#include "sirf_types.h"

/* The maximum message length that will ever be used in a message buffer. */
#define SIRF_MSG_F_MAX_MESSAGE_LEN  256

/* F ICD version contains major version in upper nibble, and minor in lower */
#ifdef F_ICD_2_1
  #define SIRF_MSG_F_ICD_VERSION 0x21
#elif defined(F_ICD_2_0)
  #define SIRF_MSG_F_ICD_VERSION 0x20
#elif defined(F_ICD_1_9)
  #define SIRF_MSG_F_ICD_VERSION 0x19
#elif defined(F_ICD_1_8)
  #define SIRF_MSG_F_ICD_VERSION 0x18
#endif

#define SIRF_MSG_F_ICD_VERSION_MAJOR (SIRF_MSG_F_ICD_VERSION >> 4)
#define SIRF_MSG_F_ICD_VERSION_MINOR (SIRF_MSG_F_ICD_VERSION & 0x0F)



/*-----------------------------------------------------------------------------
 *   Refer to the SIRFLoc Client F ICD
 *                Revision 2.1 Draft 3
 *                2007-08-15
 *
 *  the information defined here summarizes the Input and Output messages and
 *  then defines the data structures, tracking the ICD section by section
 * 
 *  within each section, this file tracks the ICD paragraph by paragraph, to 
 *  make it easy to switch from one to the other during coding or audit.
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    7.4.1 CP Generated Messages
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_F_SESSION_OPENING_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x00, 0)
#define SIRF_MSG_F_SESSION_CLOSING_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x01, 0)
#define SIRF_MSG_F_HARDWARE_CONFIGURATION_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x10, 0)
#define SIRF_MSG_F_TIME_TRANSFER_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x11, 0)
#define SIRF_MSG_F_FREQ_TRANSFER_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x12, 0)
#define SIRF_MSG_F_APRX_MS_POSITION_RSP         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x13, 0)
#define SIRF_MSG_F_WIRELESS_POWER_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x14, 0)
#define SIRF_MSG_F_REJECT_CP                    SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x15, 0)
#define SIRF_MSG_F_RESET_GPS_COMMAND            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x16, 0)
#define SIRF_MSG_F_SOFTWARE_VERSION_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x17, 0)
#define SIRF_MSG_F_ICD_VERSION_REQ              SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x18, 0)
#define SIRF_MSG_F_SET_APM                      SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x80, 0)
#define SIRF_MSG_F_SET_BLANK                    SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x81, 0)
#define SIRF_MSG_F_TEST_MODE_CONFIGURATION_REQ  SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x82, 0)
#define SIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x90, 0)
#define SIRF_MSG_F_CHANNEL_OPEN_REQ             SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA0, 0)
#define SIRF_MSG_F_CHANNEL_CLOSE_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA1, 0)
#define SIRF_MSG_F_CHANNEL_PRIORITY_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA2, 0)
#define SIRF_MSG_F_PRIORITY_QUERY               SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA3, 0)
#define SIRF_MSG_F_CHANNEL_LOAD_QUERY           SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA8, 0)
#define SIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x30, 0)
#define SIRF_MSG_F_TIME_FREQ_APROX_POS_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x31, 0)

/*-----------------------------------------------------------------------------
    7.4.2 SLC Generated Messages
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_F_SESSION_OPENING_NOTIFICATION SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x00, 0)
#define SIRF_MSG_F_SESSION_CLOSING_NOTIFICATION SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x01, 0)
#define SIRF_MSG_F_ERROR_NOTIFICATION           SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x04, 0)
#define SIRF_MSG_F_SLC_STATUS                   SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x05, 0)
#define SIRF_MSG_F_HARDWARE_CONFIGURATION_REQ   SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x10, 0)
#define SIRF_MSG_F_TIME_TRANSFER_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x11, 0)
#define SIRF_MSG_F_FREQ_TRANSFER_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x12, 0)
#define SIRF_MSG_F_APRX_MS_POSITION_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x13, 0)
#define SIRF_MSG_F_WIRELESS_POWER_REQ           SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x14, 0)
#define SIRF_MSG_F_REJECT_SLC                   SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x15, 0)
#define SIRF_MSG_F_SOFTWARE_VERSION_RSP         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x17, 0)
#define SIRF_MSG_F_ICD_VERSION_RSP              SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x18, 0)
#define SIRF_MSG_F_ACK_APM                      SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x80, 0)
#define SIRF_MSG_F_ACK_BLANK                    SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x81, 0)
#define SIRF_MSG_F_TEST_MODE_CONFIGURATION_RSP  SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x82, 0)
#define SIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP     SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x90, 0)
#define SIRF_MSG_F_CHANNEL_OPEN_RSP             SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA0, 0)
#define SIRF_MSG_F_CHANNEL_CLOSE_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA1, 0)
#define SIRF_MSG_F_CHANNEL_PRIORITY_RSP         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA2, 0)
#define SIRF_MSG_F_PRIORITY_RSP                 SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA3, 0)
#define SIRF_MSG_F_CHANNEL_LOAD_RSP             SIRF_MAKE_MSG_ID(SIRF_LC_F, 0xA8, 0)
#define SIRF_MSG_F_PUSH_AIDING_ACK_NACK         SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x30, 0)
#define SIRF_MSG_F_TIME_FREQ_APROX_POS_RSP      SIRF_MAKE_MSG_ID(SIRF_LC_F, 0x31, 0)


/*-----------------------------------------------------------------------------
    7.5.1.1 Session Opening Request
 *----------------------------------------------------------------------------*/

/* Table 13 */
typedef struct tSIRF_MSG_F_SESSION_OPENING_REQ_tag
{
   tSIRF_UINT8 ses_open_req;
}  tSIRF_MSG_F_SESSION_OPENING_REQ;

/* Table 14 */
#define SIRF_MSG_F_SOR_STAND_ALONE                ( 0x00 ) /* deprecated */
#define SIRF_MSG_F_SOR_WITHOUT_AI3                ( 0x01 )
#define SIRF_MSG_F_SOR_CDMA_IS_801_AI3            ( 0x02 )
#define SIRF_MSG_F_SOR_TDMA_IS_136_AI3            ( 0x03 )
#define SIRF_MSG_F_SOR_AMPS_IS_817_AI3            ( 0x04 )
#define SIRF_MSG_F_SOR_PDC_SIRF_DLP_AI3           ( 0x05 )
#define SIRF_MSG_F_SOR_SIRFLOC_THIN_SERVER        ( 0x70 )
#define SIRF_MSG_F_SOR_AI3                        ( 0x71 )
#define SIRF_MSG_F_SOR_RESUME                     ( 0x80 )

/*-----------------------------------------------------------------------------
    7.5.1.2 Session Opening Notification
 *----------------------------------------------------------------------------*/

/* Table 15 */
typedef struct tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION_tag
{
   tSIRF_UINT8 ses_open_not;
}  tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION;

/* Table 16 */
#define SIRF_MSG_F_SON_OPENING_SUCCEEDED          ( 0x00 )
#define SIRF_MSG_F_SON_OPENING_FAILED             ( 0x01 )
#define SIRF_MSG_F_SON_RESUME_SUCCEEDED           ( 0x80 )
#define SIRF_MSG_F_SON_RESUME_FAILED              ( 0x81 )

/*-----------------------------------------------------------------------------
    7.5.2 Error Notification
 *----------------------------------------------------------------------------*/

/* Table 17 */
typedef struct tSIRF_MSG_F_ERROR_NOTIFICATION_tag
{
   tSIRF_UINT8 session_error_reason;
}  tSIRF_MSG_F_ERROR_NOTIFICATION;

/* Table 18 */
#define SIRF_MSG_F_SER_AI3_SINGLE_TIMEOUT          ( 0x01 )
#define SIRF_MSG_F_SER_AI3_IDLE_AFTER_TIMEOUTS     ( 0x02 )
#define SIRF_MSG_F_SER_AI3_VERSION_NOT_SUPPORTED   ( 0x03 )
#define SIRF_MSG_F_SER_UNSUPPORTED_LOGICAL_CHANNEL ( 0x04 )
#define SIRF_MSG_F_SER_MESS_ID_NOT_RECOGNIZED      ( 0x05 )

/*-----------------------------------------------------------------------------
    7.5.3 SLC Status
 *----------------------------------------------------------------------------*/

/* Table 19 */
typedef struct tSIRF_MSG_F_SLC_STATUS_tag
{
   tSIRF_UINT8 slc_status; 
}  tSIRF_MSG_F_SLC_STATUS;

/* Table 20 */
#define SIRF_MSG_F_SLCS_NO_FIX_AFTER_FULL_SEARCH    ( 0x01 )
#define SIRF_MSG_F_SLCS_OK_TO_SEND                  ( 0x04 )
#define SIRF_MSG_F_SLCS_NOT_OK_TO_SEND              ( 0x02 )
#define SIRF_MSG_F_SLCS_EXTENSION                   ( 0x80 )

/* Use this mask to extract the status bits */
#define SIRF_MSG_F_SLC_BITMASK    (0x7F)
/* Use this mask to extract the OK/Not OK bits */
#define SIRF_MSG_F_OK_BITMASK     (0x06)

/*-----------------------------------------------------------------------------
    7.5.4.1 Session Closing Request
 *----------------------------------------------------------------------------*/

/* Table 21 */
typedef struct tSIRF_MSG_F_SESSION_CLOSING_REQ_tag
{
   tSIRF_UINT8 ses_close_req;
}  tSIRF_MSG_F_SESSION_CLOSING_REQ;

/* Table 22 */
#define SIRF_MSG_F_SCR_CLOSING_REQED              ( 0x00 )
#define SIRF_MSG_F_SCR_SUSPEND_REQED              ( 0x80 )

/*-----------------------------------------------------------------------------
    7.5.4.2 Session Closing Notification
 *----------------------------------------------------------------------------*/

/* Table 23 */
typedef struct tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION_tag
{
   tSIRF_UINT8 ses_close_not;
}  tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION;

/* Table 24 */
#define SIRF_MSG_F_SCN_SESSION_CLOSED             ( 0x00 )
#define SIRF_MSG_F_SCN_SESSION_CLOSING_FAILED     ( 0x01 )
#define SIRF_MSG_F_SCN_SESSION_SUSPENDED          ( 0x80 )
#define SIRF_MSG_F_SCN_SESSION_SUSPENSION_FAILED  ( 0x81 )

/*-----------------------------------------------------------------------------
    7.5.5.1 Hardware Configuration Request
 *----------------------------------------------------------------------------*/

/* Table 25 */
typedef struct tSIRF_MSG_F_HARDWARE_CONFIGURATION_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_F_HARDWARE_CONFIGURATION_REQ;

/*-----------------------------------------------------------------------------
    7.5.5.2 Hardware Configuration Response
 *----------------------------------------------------------------------------*/

/* Table 26 */
typedef struct tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP_tag
{
   tSIRF_UINT8  hw_config_msg;
   tSIRF_UINT8  nominal_freq_high;     /* [39:32] */
   tSIRF_UINT32 nominal_freq_low;      /* [31:00] */
}  tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP;

/* Table 27 */
#define SIRF_MSG_F_HWC_PRECISE_TIME_AVAILABLE      ( 0x01 )
#define SIRF_MSG_F_HWC_PRECISE_TIME_DIRECTION_BOTH ( 0x02 )
#define SIRF_MSG_F_HWC_FREQ_TRANSFER               ( 0x04 )
#define SIRF_MSG_F_HWC_FREQ_TRANSFER_NO_COUNTER    ( 0x08 )
#define SIRF_MSG_F_HWC_RTC_AVAILABLE               ( 0x10 )
#define SIRF_MSG_F_HWC_RTC_FOR_GPS_INTERNAL        ( 0x20 )
#define SIRF_MSG_F_HWC_COARSE_TIME_AVAILABLE       ( 0x40 )
#define SIRF_MSG_F_HWC_REFERENCE_CLOCK_OFF         ( 0x80 )

/*-----------------------------------------------------------------------------
    7.5.6.1 Time Transfer Request
 *----------------------------------------------------------------------------*/

/* Table 28 */
typedef struct tSIRF_MSG_F_TIME_TRANSFER_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_F_TIME_TRANSFER_REQ;

/*-----------------------------------------------------------------------------
    7.5.6.2 Time Transfer Response
 *----------------------------------------------------------------------------*/

/* Table 29 */
typedef struct tSIRF_MSG_F_TIME_TRANSFER_RSP_tag
{
   tSIRF_UINT8  tt_type;           /* 00=Coarse, FF=Precise */
   tSIRF_UINT16 week_num;          /* modulo 1024 */
   tSIRF_UINT8  time_high;         /* Bits [39..32] */
   tSIRF_UINT32 time_low;          /* Bits [31..0] microseconds 0-604800 */
   tSIRF_INT32 delta_utc;          /* milliseconds -8388.608 < x < 8388.608 */
   tSIRF_UINT8  time_accuracy;     /* coarse=milliseconds, precise=microseconds*/
}  tSIRF_MSG_F_TIME_TRANSFER_RSP;

/*-----------------------------------------------------------------------------
    7.5.7.1 Frequency Transfer Request
 *----------------------------------------------------------------------------*/
 
/* Table 32 */
typedef struct  tSIRF_MSG_F_FREQ_TRANSFER_REQ_tag
{
   tSIRF_UINT8  req_info;
}  tSIRF_MSG_F_FREQ_TRANSFER_REQ;

/* Table 33 */
#define SIRF_MSG_F_FTRQ_MULTIPLE_REQ                ( 0x01 )
#define SIRF_MSG_F_FTRQ_TRANSFER_MODE_ON            ( 0x02 )
#define SIRF_MSG_F_FTRQ_TURN_OFF_REFERENCE_CLOCK    ( 0x04 )

/*-----------------------------------------------------------------------------
    7.5.7.2 Frequency Transfer Response
 *----------------------------------------------------------------------------*/

/* Table 34 */
typedef struct  tSIRF_MSG_F_FREQ_TRANSFER_RSP_tag
{
   tSIRF_INT16  freq_offset;                 /* Hz */
   tSIRF_UINT8  accuracy;                    /* +/- this * 1575.42 MHz */
   tSIRF_UINT32 time_tag;                    /* milliseconds tow */
   tSIRF_UINT8  clock_ref;                   /* SIRF_MSG_F_TRSP_RCI_<flag> */
   tSIRF_UINT8  nominal_freq_high;           /* high frequency bits */
   tSIRF_UINT32 nominal_freq_low;            /* low frequency bits */
}  tSIRF_MSG_F_FREQ_TRANSFER_RSP;

/* Table 36 */
#define SIRF_MSG_F_FTRSP_RCI_RELATED_TO_SLC_CLOCK                ( 0x01 )
#define SIRF_MSG_F_FTRSP_RCI_REFERENCE_CLOCK_IS_OFF              ( 0x02 )
#define SIRF_MSG_F_FTRSP_RCI_PLEASE_TURN_OFF_REFERENCE_CLOCK     ( 0x04 )
#define SIRF_MSG_F_FTRSP_RCI_NOMINAL_FREQ_INCLUDED_HERE          ( 0x08 )

/*-----------------------------------------------------------------------------
    7.5.8.1 Approximate MS Position Request
 *----------------------------------------------------------------------------*/

/* Table 37 */
typedef struct  tSIRF_MSG_F_APRX_MS_POSITION_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_F_APRX_MS_POSITION_REQ;

/*-----------------------------------------------------------------------------
    7.5.8.2 Approximate MS Position Response
 *----------------------------------------------------------------------------*/

/* Table 38 */
typedef struct  tSIRF_MSG_F_APRX_MS_POSITION_RSP_tag
{
   tSIRF_INT32 lat;             /* appoximate ms lattitude */
   tSIRF_INT32 lon;             /* appoximate ms longitude*/
   tSIRF_UINT16 alt;            /* appoximate ms altitude */
   tSIRF_UINT8  est_hor_err;    /* estimated horizontal error */
   tSIRF_UINT16 est_ver_err;    /* estimated vertical error */
   tSIRF_UINT8  use_alt_aiding; /* 0x01 ON means use altitude aiding */
}  tSIRF_MSG_F_APRX_MS_POSITION_RSP;

/*-----------------------------------------------------------------------------
    7.5.9.1 Time Frequency Approximate Position Status Request
 *----------------------------------------------------------------------------*/

/* Table 40 */
typedef struct tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ_tag
{
  tSIRF_UINT8 req_mask;
} tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ;

#define SIRF_MSG_F_TFAP_REQ_GPS_TOW_REQUESTED     ( 0x01 )
#define SIRF_MSG_F_TFAP_REQ_TIME_ACCURACY         ( 0x02 )
#define SIRF_MSG_F_TFAP_REQ_FREQ_STATUS           ( 0x04 )
#define SIRF_MSG_F_TFAP_REQ_APPROXIMATE_POSITION  ( 0x08 )
#define SIRF_MSG_F_TFAP_TIME_ACC_SCALE_FINE       ( 0x01 )
#define SIRF_MSG_F_TFAP_TIME_ACC_SCALE_COARSE     ( 0x00 )
#define SIRF_MSG_F_TFAP_FREQ_ACC_SCALE            ( 0x00 )
#define SIRF_MSG_F_TFAP_MAX_TIME_MICROSEC_ACC     ( 7680 )
#define SIRF_MSG_F_TFAP_GPS_WEEK_VALID            ( 0x01 )
#define SIRF_MSG_F_TFAP_GPS_TOW_VALID             ( 0x02 )
#define SIRF_MSG_F_TFAP_GPS_TIME_ACC_VALID        ( 0x04 )
#define SIRF_MSG_F_TFAP_GPS_FREQ_ACC_VALID        ( 0x08 )
#define SIRF_MSG_F_TFAP_GPS_HOR_UNC               ( 0x10 )
#define SIRF_MSG_F_TFAP_GPS_VER_UNC               ( 0x20 )


/*-----------------------------------------------------------------------------
    7.5.9.2 Time Frequency Approximate Position Status Response
 *----------------------------------------------------------------------------*/

/* Table 41 */
typedef struct  tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP_tag
{
   tSIRF_UINT8  status_resp_mask;      /* SIRF_MSG_F_TFAP_RSP_<flag> */
   tSIRF_UINT16 gps_week;              /* extended GPS week number */
   tSIRF_UINT32 gps_tow;               /* SLC GPS TOW, to the nearest second */
   tSIRF_UINT8  time_acc_scale;        /* 0 = 1.0, 1 = 0.125, 0xFF = unknown */
   tSIRF_UINT8  time_accuracy;         /* time accuracy status with above scale*/
   tSIRF_UINT8  freq_acc_scale;        /* 0 = 0.00390625, 0xFF = unknown */
   tSIRF_UINT8  freq_accuracy;         /* scaled frequency accuracy status */
   tSIRF_UINT16 scaled_freq_offset;    /* frequency offset measured by the SLC */
   tSIRF_UINT32 freq_time_tag;         /* time when frequency measurement taken*/
   tSIRF_UINT32 slc_hor_unc;           /* horizontal uncertaingy in meters,
                                         0xFF = unknown */
   tSIRF_UINT16 slc_ver_unc;           /* vertical uncertaingy in 0.1 meters,
                                         0xFF = unknown */
   tSIRF_UINT32 spare03;               /* spare */
   tSIRF_UINT32 spare47;               /* spare */
}  tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP;

#define SIRF_MSG_F_TFAP_RSP_GPS_WEEK_IS_VALID                   ( 0x01 )
#define SIRF_MSG_F_TFAP_RSP_GPS_TOW_IS_VALID                    ( 0x02 )
#define SIRF_MSG_F_TFAP_RSP_STATUS_TIME_IS_VALID                ( 0x04 )
#define SIRF_MSG_F_TFAP_RSP_STATUS_FREQ_IS_VALID                ( 0x08 )
#define SIRF_MSG_F_TFAP_RSP_SLC_HOR_UNC_IS_VALID                ( 0x10 )
#define SIRF_MSG_F_TFAP_RSP_SLC_VER_UNC_IS_VALID                ( 0x20 )

/*-----------------------------------------------------------------------------
    7.5.10 Push Aiding Availability
 *----------------------------------------------------------------------------*/

/* Table 42 */
typedef struct tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ_tag
{
   tSIRF_UINT8  aiding_availability_mask; /* SIRF_MSG_F_PAA_AAM_<bit> */
   tSIRF_UINT8  forced_aiding_req_mask;   /* SIRF_MSG_F_PAA_FAR_<bit> */
   tSIRF_UINT8  est_hor_err;              /* horizontal error */
   tSIRF_UINT16 est_ver_err;              /* vertical error */
   tSIRF_UINT8  rel_freq_acc;             /* relative frequency accuracy */
   tSIRF_UINT8  time_accuracy_scale;      /* time accuracy scale */
   tSIRF_UINT8  time_accuracy;            /* time accuracy */
   tSIRF_UINT16 spare;                    /* spare */
} tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ;

/* aiding_availability_mask */
#define SIRF_MSG_F_PAA_AAM_HOR_VER_ER_VALID           ( 0x01 )
#define SIRF_MSG_F_PAA_AAM_REL_FREQ_ACC_VALID         ( 0x02 )
#define SIRF_MSG_F_PAA_AAM_TIME_ACCURACY_VALID        ( 0x04 )
/* forced_aiding_req_mask */
#define SIRF_MSG_F_PAA_FAR_POSITION_AIDING            ( 0x01 )
#define SIRF_MSG_F_PAA_FAR_FREQ_AIDING                ( 0x02 )
#define SIRF_MSG_F_PAA_FAR_TIME_AIDING                ( 0x04 )

/*-----------------------------------------------------------------------------
    7.5.10.1 ACK/NACK for Push Aiding Availability
 *----------------------------------------------------------------------------*/

/* Table 43 */
typedef struct tSIRF_MSG_F_PUSH_AIDING_ACK_NACK_tag
{
   tSIRF_UINT8 ack_nack;         /* 0 = ack, 1 = nak */
   tSIRF_UINT8 message_id;       /*  message id being ack'd or nak'd */
} tSIRF_MSG_F_PUSH_AIDING_ACK_NACK;

/*-----------------------------------------------------------------------------
    7.5.11.1 Wireless Power Request
 *----------------------------------------------------------------------------*/

/* Table 44 */
typedef struct  tSIRF_MSG_F_WIRELESS_POWER_REQ_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_F_WIRELESS_POWER_REQ;

/*-----------------------------------------------------------------------------
    7.5.11.2 Wireless Power Response
 *----------------------------------------------------------------------------*/

/* Table 45 */
typedef struct  tSIRF_MSG_F_WIRELESS_POWER_RSP_tag
{
   tSIRF_UINT8  wireless_power;
}  tSIRF_MSG_F_WIRELESS_POWER_RSP;

/* Table 46 */
#define SIRF_MSG_F_WLP_ESTIMATE_NOT_AVAILABLE           ( 0x00 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_LESSTHAN_10       ( 0x01 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_10_TO_20          ( 0x02 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_20_TO_30          ( 0x03 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_30_TO_40          ( 0x04 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_40_TO_50          ( 0x05 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_50_TO_60          ( 0x06 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_60_TO_70          ( 0x07 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_70_TO_80          ( 0x08 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_80_TO_90          ( 0x09 )
#define SIRF_MSG_F_WLP_WIRELESS_POWER_MORETHAN_90       ( 0x0A )

/*-----------------------------------------------------------------------------
    7.5.12 Reject Message
 *----------------------------------------------------------------------------*/

/* Table 47 */
typedef struct  tSIRF_MSG_F_REJECT_tag
{
   tSIRF_UINT8  rej_msgid;
   tSIRF_UINT8  reason;
}  tSIRF_MSG_F_REJECT_CP, tSIRF_MSG_F_REJECT_SLC;

/* Table 48 */
#define SIRF_MSG_F_REJ_NOT_SUPPORTED                   ( 0x01 )
#define SIRF_MSG_F_REJ_NOT_READY                       ( 0x02 )
#define SIRF_MSG_F_REJ_NOT_AVAILABLE                   ( 0x04 )
#define SIRF_MSG_F_REJ_WRONGLY_FORMATTED_MESSAGE       ( 0x08 )
#define SIRF_MSG_F_REJ_NO_TIME_PULSE_DURING_PRECISE_TT ( 0x10 )
#define SIRF_MSG_F_REJ_IMPROPER_REQUEST_FOR_CHANNEL    ( 0x20 )

/*-----------------------------------------------------------------------------
    7.5.13 Reset GPS Command
 *----------------------------------------------------------------------------*/

/* Table 49 */
typedef struct  tSIRF_MSG_F_RESET_GPS_COMMAND_tag
{
   tSIRF_UINT8  reset_type;
}  tSIRF_MSG_F_RESET_GPS_COMMAND;

/* Table 50 */
#define SIRF_MSG_F_RES_HOT_RESET                  ( 0x00 )
#define SIRF_MSG_F_RES_COLD_RESET                 ( 0x01 )
#define SIRF_MSG_F_RES_FACTORY_RESET              ( 0x02 )

/*-----------------------------------------------------------------------------
    7.5.14.1 Software Version Request
 *----------------------------------------------------------------------------*/

/* Table 51 */
typedef struct tSIRF_MSG_F_SOFTWARE_VERSION_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_F_SOFTWARE_VERSION_REQ;

/*-----------------------------------------------------------------------------
    7.5.14.2 Software Version Response
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_F_SOFTWARE_VERSION_STRING_SIZE (80)
/* Table 52 */
typedef struct  tSIRF_MSG_F_SOFTWARE_VERSION_RSP_tag
{
   tSIRF_UINT8 sirf_ver_bytes;
   tSIRF_UINT8 cust_ver_bytes;
   tSIRF_UINT8 sirf_ver_str[SIRF_MSG_F_SOFTWARE_VERSION_STRING_SIZE];
   tSIRF_UINT8 cust_ver_str[SIRF_MSG_F_SOFTWARE_VERSION_STRING_SIZE];
}  tSIRF_MSG_F_SOFTWARE_VERSION_RSP;

/*-----------------------------------------------------------------------------
    7.5.15.1 Set APM
 *----------------------------------------------------------------------------*/

/* Table 53 */
typedef struct  tSIRF_MSG_F_SET_APM_tag
{
   tSIRF_UINT8 apm_enable;         /* 1=enable APM */
   tSIRF_UINT8 power_duty_cycle;   /* 1-20 in units of 5% */
   tSIRF_UINT8 time_duty_priority; /* 1=time>power, 2=else */
   tSIRF_UINT8 max_on_time_indicator; 
   tSIRF_UINT8 spare1;
}  tSIRF_MSG_F_SET_APM;

/*-----------------------------------------------------------------------------
    7.5.15.2 Ack APM
 *----------------------------------------------------------------------------*/

/* Table 54 */
typedef struct  tSIRF_MSG_F_ACK_APM_tag
{
   tSIRF_UINT8 apm_state;   /* 1 = APM enabled, 0 = not */
   tSIRF_UINT8 spare;
}  tSIRF_MSG_F_ACK_APM;

/*-----------------------------------------------------------------------------
    7.5.16.1 Serial Port Settings Request
 *----------------------------------------------------------------------------*/

/* Table 55 */
typedef struct  tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ_tag
{
   tSIRF_UINT8  port;       /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT32 baud_rate;  /* 4800, 9600, 19200, 38400, 57600, 115200 */
   tSIRF_UINT8  spare;
 }  tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ;

/*-----------------------------------------------------------------------------
    7.5.16.2 Serial Port Settings Response
 *----------------------------------------------------------------------------*/

/* Table 56 */
typedef struct  tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP_tag
{
   tSIRF_UINT8  port;         /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT32 baud_rate;    /* 4800, 9600, 19200, 38400, 57600, 115200 */
   tSIRF_UINT8  ack_number;   /* 1 = old baud rate, 2 = new baud rate */
}  tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP;

/*-----------------------------------------------------------------------------
    7.5.17.1 Channel Open Request
 *----------------------------------------------------------------------------*/

/* Table 57 */
typedef struct  tSIRF_MSG_F_CHANNEL_OPEN_REQ_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 priority;      /* 0 (default) or 1 (lo) to 20 (hi) */
}  tSIRF_MSG_F_CHANNEL_OPEN_REQ;

/*-----------------------------------------------------------------------------
    7.5.17.2 Channel Open Response
 *----------------------------------------------------------------------------*/

/* Table 58 */
typedef struct  tSIRF_MSG_F_CHANNEL_OPEN_RSP_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 priority;      /* 0 (default) or 1 (lo) to 20 (hi) */
}  tSIRF_MSG_F_CHANNEL_OPEN_RSP;


/*-----------------------------------------------------------------------------
    7.5.18.1 Channel Close Request
 *----------------------------------------------------------------------------*/

/* Table 59 */
typedef struct  tSIRF_MSG_F_CHANNEL_CLOSE_REQ_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
}  tSIRF_MSG_F_CHANNEL_CLOSE_REQ;

/*-----------------------------------------------------------------------------
    7.5.18.2 Channel Close Response
 *----------------------------------------------------------------------------*/

/* Table 60 */
typedef struct  tSIRF_MSG_F_CHANNEL_CLOSE_RSP_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
}  tSIRF_MSG_F_CHANNEL_CLOSE_RSP;

/*-----------------------------------------------------------------------------
    7.5.19.1 Channel Priority Request
 *----------------------------------------------------------------------------*/

/* Table 61 */
typedef struct  tSIRF_MSG_F_CHANNEL_PRIORITY_REQ_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 priority;      /* 0 (default) or 1 (lo) to 20 (hi) */
}  tSIRF_MSG_F_CHANNEL_PRIORITY_REQ;

/*-----------------------------------------------------------------------------
    7.5.19.2 Channel Priority Response
 *----------------------------------------------------------------------------*/

/* Table 62 */
typedef struct  tSIRF_MSG_F_CHANNEL_PRIORITY_RSP_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 priority;      /* 0 (default) or 1 (lo) to 20 (hi) */
}  tSIRF_MSG_F_CHANNEL_PRIORITY_RSP;

/*-----------------------------------------------------------------------------
    7.5.20.1 Priority Query
 *----------------------------------------------------------------------------*/

/* Table 63 */
typedef struct  tSIRF_MSG_F_PRIORITY_QUERY_tag
{
   tSIRF_UINT8 port;          /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
}  tSIRF_MSG_F_PRIORITY_QUERY;

/*-----------------------------------------------------------------------------
    7.5.20.2 Priority Response
 *----------------------------------------------------------------------------*/

typedef struct  tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL_tag
{
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 priority;      /* 0 (default) or 1 (lo) to 20 (hi) */
}  tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL;

/* Table 64 */
typedef struct  tSIRF_MSG_F_PRIORITY_RSP_tag
{
   tSIRF_UINT8 port;               /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 number_of_channels; /* count of folowing struct */
   tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL channel[SIRF_NUM_CHANNELS];
}  tSIRF_MSG_F_PRIORITY_RSP;

/*-----------------------------------------------------------------------------
    7.5.21.1 Channel Load Query
 *----------------------------------------------------------------------------*/

/* Table 65 */
typedef struct  tSIRF_MSG_F_CHANNEL_LOAD_QUERY_tag
{
   tSIRF_UINT8 port; /* 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 mode; /* SIRF_MSG_F_CLQ_MODE_<flak> */
}  tSIRF_MSG_F_CHANNEL_LOAD_QUERY;

/* Table 66 */
#define SIRF_MSG_F_CLQ_MODE_TURN_OFF_PERIODIC_MESSAGE                ( 0x00 )
#define SIRF_MSG_F_CLQ_MODE_TURN_ON_PERIODIC_MESSAGE                 ( 0x01 )
#define SIRF_MSG_F_CLQ_MODE_SEND_MESSAGE_JUST_ONCE                   ( 0x02 )

/*-----------------------------------------------------------------------------
    7.5.21.2 Channel Load Response
 *----------------------------------------------------------------------------*/

typedef struct  tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL_tag
{
   tSIRF_UINT8 channel_type;  /* LCH, F, AI3, etc... */
   tSIRF_UINT8 load;          /* Bandwidth, from 0 - 100 */
}  tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL;

/* Table 67 */
typedef struct  tSIRF_MSG_F_CHANNEL_LOAD_RSP_tag
{
   tSIRF_UINT8 port;               /* 0 = Port A, 1 = Port B */
   tSIRF_UINT8 total_load;         /* Bandwidth, from 0 - 100 */
   tSIRF_UINT8 number_of_channels; /* count of folowing struct */
   tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL load_entry[SIRF_NUM_CHANNELS];
}  tSIRF_MSG_F_CHANNEL_LOAD_RSP;

/*-----------------------------------------------------------------------------
    7.5.22.1 Tx Blanking Request
 *----------------------------------------------------------------------------*/

/* Table 68 */
typedef struct  tSIRF_MSG_F_SET_BLANK_tag
{
   tSIRF_UINT8 command;       /* 0 start, 1 stop */
   tSIRF_UINT8 air_interface; /* 0 GSM air if, others invalid */
   tSIRF_UINT8 mode;          /* included in table 69 */
   tSIRF_UINT32 spare;
}  tSIRF_MSG_F_SET_BLANK;

/* Table 69 */
#define SIRF_MSG_F_SET_BLANK_ENABLE                (0x00)
#define SIRF_MSG_F_SET_BLANK_MODE_2_SLOT_BLANKING  (0x01)
#define SIRF_MSG_F_SET_BLANK_MODE_4_SLOT_BLANKING  (0x02)

/*-----------------------------------------------------------------------------
    7.5.22.2 Tx Blanking Response
 *----------------------------------------------------------------------------*/

/* Table 70 */
typedef struct  tSIRF_MSG_F_ACK_BLANK_tag
{
   tSIRF_UINT8 ack_nack;       /* 0 ack, 1 nack */
   tSIRF_UINT8 spare;
}  tSIRF_MSG_F_ACK_BLANK;

/*-----------------------------------------------------------------------------
    7.5.23.1 Test Mode Configuration Request
 *----------------------------------------------------------------------------*/

/* Table 71 */
typedef struct tSIRF_MSG_F_TEST_MODE_CONFIGURATION_REQ_tag
{
   tSIRF_UINT8 config_1;
   tSIRF_UINT8 config_spare1;
   tSIRF_UINT8 config_spare2;
   tSIRF_UINT8 config_spare3;
}  tSIRF_MSG_F_TEST_MODE_CONFIGURATION_REQ;

/*-----------------------------------------------------------------------------
    7.5.23.2 Test Mode Configuration Response
 *----------------------------------------------------------------------------*/

/* Table 72 */
typedef struct tSIRF_MSG_F_TEST_MODE_CONFIGURATION_RSP_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_F_TEST_MODE_CONFIGURATION_RSP;

/*-----------------------------------------------------------------------------
    7.5.24.1 ICD Version Request
 *----------------------------------------------------------------------------*/

/* Table 73 */
typedef struct tSIRF_MSG_F_ICD_VERSION_REQ_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_F_ICD_VERSION_REQ;

/*-----------------------------------------------------------------------------
    7.5.24.2 ICD Version Response
 *----------------------------------------------------------------------------*/

/* Table 74 */
typedef struct tSIRF_MSG_F_ICD_VERSION_RSP_tag
{
   tSIRF_UINT8 f_icd_version_major;
   tSIRF_UINT8 f_icd_version_minor; 
   tSIRF_UINT8 ai3_icd_version_major;
   tSIRF_UINT8 ai3_icd_version_minor; 
   tSIRF_UINT8 spare1;
   tSIRF_UINT8 spare2;
}  tSIRF_MSG_F_ICD_VERSION_RSP;

#endif /* SIRF_MSG_F_H_INCLUDED */

/**
 * @}
 */

