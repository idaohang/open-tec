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

#ifndef SIRF_MSG_AI3_H_INCLUDED
#define SIRF_MSG_AI3_H_INCLUDED

/*-----------------------------------------------------------------------------
 *   Include Files
 *----------------------------------------------------------------------------*/

#include "sirf_types.h"

/*-----------------------------------------------------------------------------
 *   Refer to the Aiding Independent Interoperability Interface
 *                SYSI0002 
 *                Revision 2.2 Draft 12
 *                2008-03-17
 *
 *  the information defined here summarizes the Input and Output messages and
 *  then defines the data structures, tracking the ICD section by section
 * 
 *  within each section, this file tracks the ICD paragraph by paragraph, to 
 *  make it easy to switch from one to the other during coding or audit.
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Macro Definitions
 *----------------------------------------------------------------------------*/

/* Features in the software should never be compile protected using names
   which have ICD numbers or indicators in them.  Instead they should be
   given an ICD independent name which reflects the feature and the compliance
   should be done in this file.  That way, fewer code changes are required if
   a new ICD on a different protocol requires the same feature or if a feature
   is removed in a subsequent rev of the same protocol */

#ifdef AI3_ICD_1_6
   #define SIRF_MSG_AI3_ICD_VERSION 0x16
   #define MS_ASSIST
   #define ENABLE_ACQ_ASSIST
#endif /* AI3_ICD_1_6 */

/* for variant SL3_C4_FLASH_1_9 */
#ifdef AI3_ICD_1_9
   #define AI3_ICD_1_6
   #define SIRF_MSG_AI3_ICD_VERSION 0x19
   #define MS_ASSIST
   #define ENABLE_ACQ_ASSIST
#endif /* AI3_ICD_1_9 */

/* for SL3_C5_RAM_COLOC and SL3_C6_FLASH_COLOC */
#ifdef AI3_ICD_1_9_COLOC
   #define AI3_ICD_1_9
   #define SIRF_MSG_AI3_ICD_VERSION 0x19
   #define MS_ASSIST
   #define MS_ASSIST_COLOC
#endif /* AI3_ICD_1_9_COLOC */

/* AI3: 2.0 -> Latest "Official" version            */
/* (ephemeris re-aiding, simultaneous MSA/MSB etc). */
#ifdef AI3_ICD_2_0
   #define AI3_DUAL_POS_METHOD
   #define AI3_EPH_ALM_POLL_ENABLE
   #define AI3_ICD_1_6
   #define AI3_ICD_1_9
   #define SIRF_MSG_AI3_ICD_VERSION 0x20
   #define MS_ASSIST
   #define ENABLE_ACQ_ASSIST
#endif /* AI3_ICD_2_0 */

/* AI3 2.1 = AI3 2.0 + EE (both SLC+GSW)  */
#ifdef AI3_ICD_2_1
   #define AI3_DUAL_POS_METHOD
   #define AI3_EPH_ALM_POLL_ENABLE
   #define AI3_ICD_1_6
   #define AI3_ICD_1_9
   #define AI3_ICD_2_0
   #define SIRF_MSG_AI3_ICD_VERSION 0x21
   #define MS_ASSIST
   #define ENABLE_ACQ_ASSIST
   #define SLC_EE
#endif /*AI3_ICD_2_1*/

/* AI3 2.2 = AI3 2.1 + AI3NavBits */
#ifdef AI3_ICD_2_2
   #define AI3_DUAL_POS_METHOD
   #define AI3_EPH_ALM_POLL_ENABLE
   #define AI3_ICD_1_6
   #define AI3_ICD_1_9
   #define AI3_ICD_2_0
   #define AI3_ICD_2_1
   #define SIRF_MSG_AI3_ICD_VERSION 0x22
   #define MS_ASSIST
   #define ENABLE_ACQ_ASSIST
   #define SLC_EE
   #define NAV_BIT_STRIPPING_HI_AI3 
#endif /*AI3_ICD_2_1*/

#define SIRF_MSG_AI3_ICD_VERSION_MAJOR (SIRF_MSG_AI3_ICD_VERSION >> 4)
#define SIRF_MSG_AI3_ICD_VERSION_MINOR (SIRF_MSG_AI3_ICD_VERSION & 0x0F)


#ifdef F_ICD_2_1
   #define SLC_TP
#endif

#define SIRF_MSG_AI3_MAX_STRUCT_SIZE (sizeof(tSIRF_MSG_AI3_REQ))
#ifdef AI3_ICD_2_2
 #define SIRF_MSG_AI3_RSP_NUM_SV_INFO (16)
#else
 #define SIRF_MSG_AI3_RSP_NUM_SV_INFO (10)
#endif
#define SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS (16)
#ifdef AI3_ICD_2_2
 #define SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS (16)
#else
 #define SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS (12)
#endif
#define SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED (11)
#define SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123 (113) /* 904 bits */
#define SIRF_MSG_AI3_ANS123R_NUM_SF123 (16)
#define SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45 (75) /* 600 bits */
#define SIRF_MSG_AI3_ANS45R_NUM_SF45 (25)

/* The maximum message length that will ever be used in a message buffer.
   Same as SSB for now. */
#define SIRF_MSG_AI3_MAX_MESSAGE_LEN  SIRF_MSG_SSB_MAX_MESSAGE_LEN

/*-----------------------------------------------------------------------------
 *   6.1 Detailed Message Definitions, Table 5 - AI3 Message ID
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_AI3_REQ                      SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x01, 0)
#define SIRF_MSG_AI3_RSP                      SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x02, 0)
#define SIRF_MSG_AI3_ACK_NACK                 SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x03, 0)
#define SIRF_MSG_AI3_SLC_AIDING_REQ           SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x04, 0)
#define SIRF_MSG_AI3_NAV_SF_123_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x10, 0)
#define SIRF_MSG_AI3_NAV_SF_45_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x11, 0)
#define SIRF_MSG_AI3_UNSOL_SLC_EPH_STATUS_RSP SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x12, 0)
#define SIRF_MSG_AI3_SLC_CP_ACK_NACK          SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x1F, 0)
#define SIRF_MSG_AI3_SLC_EPH_STATUS_REQ       SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x21, 0)
#define SIRF_MSG_AI3_SOL_SLC_EPH_STATUS_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x22, 0)
#define SIRF_MSG_AI3_POLL_ALMANAC_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x23, 0)
#define SIRF_MSG_AI3_POLL_ALMANAC_RSP         SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x24, 0)
#define SIRF_MSG_AI3_EE_INTEGRITY_WARNING     SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x13, 0)
#define SIRF_MSG_AI3_EE_CLKBIAS_ADJUST        SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x14, 0)
#define SIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV    SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x25, 0)
#define SIRF_MSG_AI3_BE_REQ                   SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x26, 0)
#define SIRF_MSG_AI3_BE_RSP                   SIRF_MAKE_MSG_ID(SIRF_LC_AI3, 0x15, 0)

/*-----------------------------------------------------------------------------
    6.1.1 AI3 Request
 *----------------------------------------------------------------------------*/

/* Support Structures for Table 6 */

/* Table 6, starting at NUM_FIXES */
typedef struct tSIRF_MSG_AI3_POS_REQ_INFO_tag
{
   tSIRF_UINT8 num_fixes;
   tSIRF_UINT8 time_btw_fixes;
   tSIRF_UINT8 hori_error_max;
   tSIRF_UINT8 vert_error_max;
   tSIRF_UINT8 resp_time_max;
   tSIRF_UINT8 time_acc_priority;
} tSIRF_MSG_AI3_POS_REQ_INFO;

/* Table 6, starting at ALPHA_0 */
typedef struct tSIRF_MSG_AI3_IONO_DATA_tag
{
   tSIRF_INT8 alpha_0;
   tSIRF_INT8 alpha_1;
   tSIRF_INT8 alpha_2;
   tSIRF_INT8 alpha_3;
   tSIRF_INT8 beta_0;
   tSIRF_INT8 beta_1;
   tSIRF_INT8 beta_2;
   tSIRF_INT8 beta_3;
} tSIRF_MSG_AI3_IONO_DATA;

/* Table 6, starting after EPH_FLAG */
typedef struct tSIRF_MSG_AI3_EPHEMERIS_PARAMETERS_tag
{
   tSIRF_UINT8  sv_prn_num;   
   tSIRF_UINT8  ura_ind;      
   tSIRF_UINT8  iode;         
   tSIRF_INT16  c_rs;         
   tSIRF_INT16  delta_n;      
   tSIRF_INT32  m0;           
   tSIRF_INT16  c_uc;         
   tSIRF_UINT32 eccentricity; 
   tSIRF_INT16  c_us;         
   tSIRF_UINT32 a_sqrt;       
   tSIRF_UINT16 toe;          
   tSIRF_INT16  c_ic;         
   tSIRF_INT32  omega_0;      
   tSIRF_INT16  c_is;         
   tSIRF_INT32  i0;           
   tSIRF_INT16  c_rc;         
   tSIRF_INT32  omega;        
   tSIRF_INT32  omegadot;     
   tSIRF_INT16  idot;         
   tSIRF_UINT16 toc;          
   tSIRF_INT8   t_gd;         
   tSIRF_INT8   af2;          
   tSIRF_INT16  af1;          
   tSIRF_INT32  af0;          
} tSIRF_MSG_AI3_EPHEMERIS_PARAMETERS;

/* Table 6, starting at EPH_FLAG */
typedef struct tSIRF_MSG_AI3_EPHEMERIS_INFO_PER_SV_tag
{
   tSIRF_UINT8  eph_flag;
   tSIRF_MSG_AI3_EPHEMERIS_PARAMETERS eph_data;
}tSIRF_MSG_AI3_EPHEMERIS_INFO_PER_SV;

/* Table 6, starting at EPH_FLAG */
typedef struct tSIRF_MSG_AI3_EPHEMERIS_INFO_tag
{
   tSIRF_MSG_AI3_EPHEMERIS_INFO_PER_SV sv_eph_prm[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_AI3_EPHEMERIS_INFO;

/* Table 6, starting at ALM_DATA_FLAG */
typedef struct tSIRF_MSG_AI3_ALMANAC_PARAMETERS_tag
{
   tSIRF_UINT8  alm_valid_flag;  
   tSIRF_UINT8  alm_sv_prn_num;  
   tSIRF_UINT16 alm_eccentricity;
   tSIRF_UINT8  alm_toa;         
   tSIRF_INT16 alm_delta_incl;   
   tSIRF_INT16 alm_omegadot;     
   tSIRF_UINT32 alm_a_sqrt;      
   tSIRF_INT32 alm_omega_0;      
   tSIRF_INT32 alm_omega;        
   tSIRF_INT32 alm_m0;           
   tSIRF_INT16 alm_af0;          
   tSIRF_INT16 alm_af1;          
} tSIRF_MSG_AI3_ALMANAC_PARAMETERS;

/* Table 6, starting after ALM_DATA_FLAG */
typedef struct tSIRF_MSG_AI3_ALMANAC_DATA_tag
{
   tSIRF_UINT16 alm_week_num;
   tSIRF_MSG_AI3_ALMANAC_PARAMETERS alm_params[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_AI3_ALMANAC_DATA;

/* Table 6, starting at ACQ_ASSIST_FLAG */
typedef struct tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_DATA_tag
{
   tSIRF_UINT8  acq_assist_valid_flag; 
   tSIRF_UINT8  sv_prn_number;      
   tSIRF_INT16  doppler0;          
   tSIRF_INT8   doppler1;           
   tSIRF_UINT8  doppler_uncertainty;
   tSIRF_UINT16 sv_code_phase;     
   tSIRF_UINT8  sv_code_phase_int;  
   tSIRF_UINT8  gps_bit_num;        
   tSIRF_UINT16 code_phase_uncertainty; 
   tSIRF_UINT16 azimuth;           
   tSIRF_INT8   elevation;          
} tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_DATA;

/* Table 6, starting after ACQ_ASSIST_FLAG */
typedef struct tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_INFO_tag
{
   tSIRF_UINT32 reference_time;
   tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_DATA acq_data[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_INFO;

/* Table 6 */

typedef struct  tSIRF_MSG_AI3_REQ_tag
{
   tSIRF_UINT8  icd_rev_num;
   tSIRF_UINT8  pos_req_flag;
   tSIRF_MSG_AI3_POS_REQ_INFO pos_req_infol; 
   tSIRF_UINT8  location_method;
   tSIRF_UINT8  alm_req_flag;
   tSIRF_UINT8  iono_flag;
   tSIRF_MSG_AI3_IONO_DATA iono_data;        
   tSIRF_MSG_AI3_EPHEMERIS_INFO eph_prms;    
   tSIRF_UINT8  alm_data_flag;
   tSIRF_MSG_AI3_ALMANAC_DATA alm_data;      
   tSIRF_UINT8  acq_assist_flag;    
   tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_INFO acq_info;
   tSIRF_UINT8  unhealthy_sat_flag; 
   tSIRF_UINT32 unhealthy_sat_info;
   tSIRF_UINT8  pos_req_aux_flag;   
   tSIRF_UINT8  pos_req_aux_num_bytes; 
   tSIRF_UINT8  nw_enhance_type;    
   tSIRF_INT32  coarse_pos_ref_lat;
   tSIRF_INT32  coarse_pos_ref_lon;
   tSIRF_UINT8  pos_req_id;
}  tSIRF_MSG_AI3_REQ;

/* Table 7 */
#define SIRF_MSG_AI3_REQ_HV_ERROR_1M      (0x00)
#define SIRF_MSG_AI3_REQ_HV_ERROR_5M      (0x01)
#define SIRF_MSG_AI3_REQ_HV_ERROR_10M     (0x02)
#define SIRF_MSG_AI3_REQ_HV_ERROR_20M     (0x03)
#define SIRF_MSG_AI3_REQ_HV_ERROR_40M     (0x04)
#define SIRF_MSG_AI3_REQ_HV_ERROR_80M     (0x05)
#define SIRF_MSG_AI3_REQ_HV_ERROR_160M    (0x06)
#define SIRF_MSG_AI3_REQ_HV_ERROR_NO_MAX  (0x07)

/* Table 9 */ 
#define SIRF_MSG_AI3_REQ_LOCMETH_MS_ASSISTED             (0x00)
#define SIRF_MSG_AI3_REQ_LOCMETH_MS_BASED                (0x01)
#define SIRF_MSG_AI3_REQ_LOCMETH_MS_BASED_PREFERRED      (0x02)
#define SIRF_MSG_AI3_REQ_LOCMETH_MS_ASSISTED_PREFERRED   (0x03)

/* Position request auxiliary flag validity */
#define SIRF_MSG_AI3_REQ_POSREQ_AUXVALID_NW_ENHANCE           (0x01)
#define SIRF_MSG_AI3_REQ_POSREQ_AUXVALID_NW_ENHANCE_AND_COLOC (0x03)

/* Table 10 */
#define SIRF_MSG_AI3_REQ_NW_ET_NONE                      (0x00)
#define SIRF_MSG_AI3_REQ_NW_ET_COURSE_POSITION_ENABLED   (0x02)
#define SIRF_MSG_AI3_REQ_NW_ET_AUX_NAVMODEL_SUPPORTED    (0x04)
#define SIRF_MSG_AI3_REQ_NW_ET_NAVBIT_123_SUPPORTED      (0x08)
#define SIRF_MSG_AI3_REQ_NW_ET_NAVBIT_45_SUPPORTED       (0x10)

/* Table 12 */
#define SIRF_MSG_AI3_REQ_ALM_RF_NO_ALMANAC_REQUEST            (0x00)
#define SIRF_MSG_AI3_REQ_ALM_RF_REPORT_FLASH_ALMANAC_AGE      (0x01)
#define SIRF_MSG_AI3_REQ_ALM_RF_REQUEST_COLLECTION_FROM_SV    (0x02)
#define SIRF_MSG_AI3_REQ_ALM_RF_REPORT_UPDATE_STATUS          (0x03)

/* Table 13 */
#define SIRF_MSG_AI3_EP_URA_INDEX_0002_40 (0x00)
#define SIRF_MSG_AI3_EP_URA_INDEX_0003_40 (0x01)
#define SIRF_MSG_AI3_EP_URA_INDEX_0004_85 (0x02)
#define SIRF_MSG_AI3_EP_URA_INDEX_0006_85 (0x03)
#define SIRF_MSG_AI3_EP_URA_INDEX_0009_65 (0x04)
#define SIRF_MSG_AI3_EP_URA_INDEX_0013_65 (0x05)
#define SIRF_MSG_AI3_EP_URA_INDEX_0024_00 (0x06)
#define SIRF_MSG_AI3_EP_URA_INDEX_0048_00 (0x07)
#define SIRF_MSG_AI3_EP_URA_INDEX_0096_00 (0x08)
#define SIRF_MSG_AI3_EP_URA_INDEX_0192_00 (0x09)
#define SIRF_MSG_AI3_EP_URA_INDEX_0384_00 (0x0a)
#define SIRF_MSG_AI3_EP_URA_INDEX_0768_00 (0x0b)
#define SIRF_MSG_AI3_EP_URA_INDEX_1536_00 (0x0c)
#define SIRF_MSG_AI3_EP_URA_INDEX_3072_00 (0x0d)
#define SIRF_MSG_AI3_EP_URA_INDEX_6144_00 (0x0e)
#define SIRF_MSG_AI3_EP_URA_INDEX_NONE    (0x0f)

/* Table 14 */
#define SIRF_MSG_AI3_AAD_DU_200HZ    (0x00)
#define SIRF_MSG_AI3_AAD_DU_100HZ    (0x01)
#define SIRF_MSG_AI3_AAD_DU_50HZ     (0x02)
#define SIRF_MSG_AI3_AAD_DU_25HZ     (0x03)
#define SIRF_MSG_AI3_AAD_DU_12_5HZ   (0x04)
#define SIRF_MSG_AI3_AAD_DU_UNKNOWN  (0xFF)

/*-----------------------------------------------------------------------------
    6.1.2 AI3 Response
 *----------------------------------------------------------------------------*/

/* Table 15, starting at SV_PRN */
typedef struct tSIRF_MSG_AI3_NB_SV_DATA_tag
{
   tSIRF_UINT8 sv_prn;
   tSIRF_UINT8 c_n0;
   tSIRF_UINT8 inv_weights;
} tSIRF_MSG_AI3_NB_SV_DATA;

/* Table 15, starting at POSITION_ERROR_STATUS */
typedef struct tSIRF_MSG_AI3_GPS_POSITION_DATA_tag
{
   tSIRF_UINT8    pos_err_status;
   tSIRF_UINT8    qos_pass_flag; /* note: POS_ACC_MET */
   tSIRF_UINT8    pos_type;
   tSIRF_UINT8    dgps_cor;
   tSIRF_UINT16   gps_week;
   tSIRF_UINT32   time_of_week;
   tSIRF_INT32    lat;
   tSIRF_INT32    lon;
   tSIRF_UINT8    other_sections;
   tSIRF_UINT8    hor_err_angle;
   tSIRF_UINT8    hor_err_major;
   tSIRF_UINT8    hor_err_minor;
   tSIRF_UINT16   vert_pos_height;
   tSIRF_UINT8    vert_pos_std;
   tSIRF_UINT16   velocity_horizontal;
   tSIRF_UINT16   velocity_heading;
   tSIRF_INT8     velocity_vertical;
   tSIRF_UINT8    velocity_hor_err_ang;
   tSIRF_UINT8    velocity_hor_err_maj_std;
   tSIRF_UINT8    velocity_hor_err_min_std;
   tSIRF_UINT8    velocity_ver_err;
   tSIRF_UINT8    clk_cor_time_ref;
   tSIRF_UINT16   clk_cor_clk_bias;
   tSIRF_UINT16   clk_cor_clk_drift;
   tSIRF_UINT8    clk_cor_clk_std_err;
   tSIRF_UINT8    clk_cor_utc_off;
   tSIRF_UINT8    pos_corr_num_sat;
   tSIRF_MSG_AI3_NB_SV_DATA    pos_corr_sv_info[SIRF_MSG_AI3_RSP_NUM_SV_INFO];
} tSIRF_MSG_AI3_GPS_POSITION_DATA;

/* Table 15, starting at SV_PRN */
typedef struct tSIRF_MSG_AI3_MEAS_PER_SV_DATA_tag
{
   tSIRF_UINT8  sv_prn;
   tSIRF_UINT8  c_n0;
   tSIRF_INT16 sv_doppler;
   tSIRF_UINT16 sv_code_phase_wh;
   tSIRF_UINT16 sv_code_phase_fr;
   tSIRF_UINT8  multipath_indicator;
   tSIRF_UINT8  pseudorange_rms_error;
} tSIRF_MSG_AI3_MEAS_PER_SV_DATA;

/* Table 15, starting at MEAS_ERROR_STATUS */
typedef struct tSIRF_MSG_AI3_MEASUREMENT_DATA_tag
{
   tSIRF_UINT8  meas_error_status;
   tSIRF_UINT16 meas_gps_week;
   tSIRF_UINT32 meas_gps_seconds;
   tSIRF_UINT8  time_accuracy;
   tSIRF_UINT8  num_sv;
   tSIRF_MSG_AI3_MEAS_PER_SV_DATA meas_per_sv_data [SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS];
} tSIRF_MSG_AI3_MEASUREMENT_DATA;

/* Table 15, starting at SUBALM_SV_PRN */
typedef struct tSIRF_MSG_AI3_SUBALM_INFO_tag
{
   tSIRF_UINT8  subalm_sv_prn;
   tSIRF_UINT16 subalm_week_num;
   tSIRF_UINT8  subalm_toa;
}tSIRF_MSG_AI3_SUBALM_INFO;

/* Table 15, starting at SUBALM_DATA_STAT */
typedef struct tSIRF_MSG_AI3_SUB_ALM_DATA_tag{
   tSIRF_UINT8  data_stat;
   tSIRF_MSG_AI3_SUBALM_INFO subalm_info[SIRF_MAX_SVID_CNT];
}tSIRF_MSG_AI3_SUB_ALM_DATA;

/* Table 15, starting at CP_NUM_BYTES */
typedef struct tSIRF_MSG_AI3_COARSE_POSITION_DATA_tag
{
   tSIRF_UINT8  cp_num_bytes;
   tSIRF_INT32 delta_lat;
   tSIRF_INT32 delta_lon;
   tSIRF_INT32 delta_alt;
   tSIRF_UINT8  pr_err_th;
} tSIRF_MSG_AI3_COARSE_POSITION_DATA;

/* Table 15 */
typedef struct tSIRF_MSG_AI3_RSP_tag
{
   tSIRF_UINT8  pos_results_flag;
   tSIRF_MSG_AI3_GPS_POSITION_DATA pos_data;
   tSIRF_UINT8  alm_ref_date_flag;
   tSIRF_UINT8  alm_data_stat;
   tSIRF_UINT16 alm_week_num;
   tSIRF_UINT8  alm_toa;
   tSIRF_UINT8  gps_meas_flag;
   tSIRF_MSG_AI3_MEASUREMENT_DATA gps_meas_data;
   tSIRF_UINT8  subalm_flag;
   tSIRF_MSG_AI3_SUB_ALM_DATA alm_data;
   tSIRF_UINT8  cp_valid_flag;
   tSIRF_MSG_AI3_COARSE_POSITION_DATA cp_data;
   tSIRF_UINT8  pos_req_id;
} tSIRF_MSG_AI3_RSP;

/*-----------------------------------------------------------------------------
    6.1.3 ACK/NACK Message
 *----------------------------------------------------------------------------*/

/* Table 37 */
typedef struct tSIRF_MSG_AI3_ACK_NACK_tag
{
   tSIRF_UINT8 acknak;
}  tSIRF_MSG_AI3_ACK_NACK;

/* Table 38 */
#define SIRF_MSG_AI3_AAN_ACK         (0x00)
#define SIRF_MSG_AI3_AAN_PARAM_ERR   (0xFD)
#define SIRF_MSG_AI3_AAN_ICD_REV_ERR (0xFE)
#define SIRF_MSG_AI3_AAN_NAK         (0xFF)

/*-----------------------------------------------------------------------------
    6.1.4 SLC Ephemeris Status Request
 *----------------------------------------------------------------------------*/

typedef struct tSIRF_MSG_AI3_SLC_EPH_STATUS_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_AI3_SLC_EPH_STATUS_REQ;

/*-----------------------------------------------------------------------------
    6.1.5 Unsolicited & Solicited SLC Ephemeris Status Response
 *----------------------------------------------------------------------------*/

/* Table 39, starting at SATID */
typedef struct tSIRF_MSG_AI3_EPH_INFO_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT8  sv_info_flag;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT16 gps_toe;
   tSIRF_UINT8  iode;
   tSIRF_UINT16 azimuth;
   tSIRF_INT8   elevation;
}tSIRF_MSG_AI3_EPH_INFO;

/* Table 39 */
typedef struct tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP
{
   tSIRF_UINT16 agps_status_mask;
   tSIRF_UINT8  gps_time_mask;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT32 gps_tow;
   tSIRF_UINT16 eph_status_section_size;
   tSIRF_UINT8  eph_aiding_type;
   tSIRF_UINT8  gps_time_tolerance_limit;
   tSIRF_MSG_AI3_EPH_INFO eph_info[SIRF_MAX_SVID_CNT];
}tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP, tSIRF_MSG_AI3_UNSOL_SLC_EPH_STATUS_RSP, tSIRF_MSG_AI3_SOL_SLC_EPH_STATUS_RSP;

/*-----------------------------------------------------------------------------
    6.1.6 SLC/CP Message ACK/NACK
 *----------------------------------------------------------------------------*/

/* Support for Table 40 */
#define SIRF_MSG_AI3_ASRAN_ACK                  (0x00)
#define SIRF_MSG_AI3_ASRAN_NAV_BIT_AIDING_ERR   (0xFD)
#define SIRF_MSG_AI3_ASRAN_EPH_REQ_ERR          (0xFE)
#define SIRF_MSG_AI3_ASRAN_NAK                  (0xFF)

/* Table 40 */
typedef struct tSIRF_MSG_AI3_SLC_CP_ACK_NACK_tag{
   tSIRF_UINT8  ack_nack_error;
   tSIRF_UINT8  slc_msg_id;
   tSIRF_UINT16 spare;
} tSIRF_MSG_AI3_SLC_CP_ACK_NACK;

/*-----------------------------------------------------------------------------
    6.1.7 Poll Almanac Request
 *----------------------------------------------------------------------------*/

typedef struct tSIRF_MSG_AI3_POLL_ALMANAC_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_AI3_POLL_ALMANAC_REQ;

/*-----------------------------------------------------------------------------
    6.1.8 Poll Almanac Response
 *----------------------------------------------------------------------------*/
 
/* Table 41, starting at ALM_VALID_FLAG */
typedef struct tSIRF_MSG_AI3_ALMANAC_tag
{
   tSIRF_UINT8  valid_flag;
   tSIRF_UINT8  svprnNum;
   tSIRF_UINT16 week_num;
   tSIRF_UINT16 eccentricity;
   tSIRF_UINT8  toa;
   tSIRF_INT16 delta_incl;
   tSIRF_INT16 omegadot;
   tSIRF_UINT32 a_sqrt; /* note: 24 bits */
   tSIRF_INT32 omega_0; /* note: 24 bits */
   tSIRF_INT32 omega;   /* note: 24 bits */
   tSIRF_INT32 m0;      /* note: 24 bits */
   tSIRF_INT16 af0;
   tSIRF_INT16 af1;
} tSIRF_MSG_AI3_ALMANAC;

/* Table 41 */
typedef struct tSIRF_MSG_AI3_POLL_ALMANAC_RSP_tag
{
   tSIRF_UINT8   data_flag;
   tSIRF_UINT16  extended_gps_week;
   tSIRF_UINT32  gpstow;
   tSIRF_MSG_AI3_ALMANAC alm_per_sv_list[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_AI3_POLL_ALMANAC_RSP;

/*-----------------------------------------------------------------------------
    6.1.9 Unsolicited SLC EE Integrity Warning
 *----------------------------------------------------------------------------*/
 
/* Table Label Missing from ICD - s/b e.g. 41.1 */
typedef struct tSIRF_MSG_AI3_EE_INTEGRITY_WARNING_tag /* Not to be compressed */
{
   tSIRF_UINT8  icd_rev_num;  
   tSIRF_UINT32 sat_pos_validity_mask; 
   tSIRF_UINT32 sat_clock_validity_mask; 
   tSIRF_UINT32 sat_health_indicator;
} tSIRF_MSG_AI3_EE_INTEGRITY_WARNING;

/*-----------------------------------------------------------------------------
    6.1.10 Unsolicited SLC EE Clock Bias Adjustment
 *----------------------------------------------------------------------------*/
 
/* Table, starting at SV_ID */
typedef struct tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST_DATA_tag
{
   tSIRF_UINT8  sv_id;  
   tSIRF_UINT16 ee_toe;
   tSIRF_INT32 clock_bias_adjust;
}tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST_DATA; 

/* Table */
typedef struct tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST_tag  /* To be compressed */
{
   tSIRF_UINT8  icd_rev_num;  
   tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST_DATA data[SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS];
} tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST;

/*-----------------------------------------------------------------------------
    6.1.11 CP Send Auxiliary Nav Message
 *----------------------------------------------------------------------------*/

/* Table 42, starting at NAVMODEL_SV_PRN */ 
typedef struct tSIRF_MSG_AI3_AUXILIARY_NAV_PARAMS_tag
{
   tSIRF_UINT8          svid;
   tSIRF_UINT16         toe;
   tSIRF_UINT16         iodc;
   tSIRF_UINT8          sf1_l2_info;
   tSIRF_UINT8          sf1_sv_health;
   tSIRF_UINT8          sf1_reserved[SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED];
   tSIRF_UINT8          sf1_aodo_fit_interval;
} tSIRF_MSG_AI3_AUXILIARY_NAV_PARAMS;

/* Table 42, starting at R_A0 */ 
typedef struct tSIRF_MSG_AI3_AUXILIARY_NAV_UTC_MODEL_tag
{
   tSIRF_UINT32        a0;
   tSIRF_UINT32        a1;
   tSIRF_UINT8         delta_tls;
   tSIRF_UINT8         tot;
   tSIRF_UINT8         wnt;
   tSIRF_UINT8         wnlsf;
   tSIRF_UINT8         dn;
   tSIRF_UINT8         delta_t_lsf;
} tSIRF_MSG_AI3_AUXILIARY_NAV_UTC_MODEL;

/* Table 42, starting at TOW_ASSIST_SV_PRN_NNM */ 
typedef struct tSIRF_MSG_AI3_AUXILIARY_NAV_TOW_ASSIST_tag
{
   tSIRF_UINT8          svid;
   tSIRF_UINT16         tlm_msg;
   tSIRF_UINT8          tow_assist_info;
} tSIRF_MSG_AI3_AUXILIARY_NAV_TOW_ASSIST;

/* Table 42 */ 
typedef struct tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV_tag
{
   tSIRF_UINT8          aux_nav_model_valid_flag;
   tSIRF_MSG_AI3_AUXILIARY_NAV_PARAMS  aux_nav_params[SIRF_MAX_SVID_CNT];
   tSIRF_UINT8         utc_model_valid_flag;
   tSIRF_MSG_AI3_AUXILIARY_NAV_UTC_MODEL utc_model;
   tSIRF_UINT8          gps_tow_assist_valid_flag;
   tSIRF_MSG_AI3_AUXILIARY_NAV_TOW_ASSIST  aux_gps_tow_assist[SIRF_MAX_SVID_CNT];
}tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV;

/*-----------------------------------------------------------------------------
    6.1.12 CP Aiding Request Message
 *----------------------------------------------------------------------------*/

/* Table 43 - NAV Bit Request Flag values */ 
#define SIRF_MSG_AI3_ASAR_NBRF_SF_123_REQUESTED (0x01)
#define SIRF_MSG_AI3_ASAR_NBRF_SF_45_REQUESTED  (0x02)

/* Table 43 */ 
typedef struct tSIRF_MSG_AI3_SLC_AIDING_REQ_tag
{
   tSIRF_UINT16 section_validity_flag;
   tSIRF_UINT8  section_size;
   tSIRF_UINT32 sat_mask_nav_bit;
   tSIRF_UINT8  nav_bit_req_flag;
}tSIRF_MSG_AI3_SLC_AIDING_REQ;

/*-----------------------------------------------------------------------------
    6.1.13 Nav Subframe 1_2_3 Aiding Response Message
 *----------------------------------------------------------------------------*/

/* Table 44, STARTING AT SUBF_1_2_3_FLAG */ 
typedef struct tSIRF_MSG_AI3_NAV_SF_123_tag
{
   tSIRF_UINT8 sf_123_flag;
   tSIRF_UINT8 sv_id;
   tSIRF_UINT8 sf123[SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123];
}tSIRF_MSG_AI3_NAV_SF_123;

/* Table 44 */ 
typedef struct tSIRF_AI3_NAV_SF_123_RSP_tag
{
   tSIRF_MSG_AI3_NAV_SF_123  sf123[SIRF_MSG_AI3_ANS123R_NUM_SF123];
} tSIRF_MSG_AI3_NAV_SF_123_RSP;

/*-----------------------------------------------------------------------------
    6.1.14 Nav Subframe 4_5 Aiding Response Message
 *----------------------------------------------------------------------------*/

/* Table 45, starting at FRAME_NUM */ 
typedef struct
{
   tSIRF_UINT8 frame_num;
   tSIRF_UINT8 SF45[SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45]; /* 600 bits */
}tSIRF_MSG_AI3_NAV_SF_45;

/* Table 45 */ 
typedef struct tSIRF_MSG_AI3_NAV_SF_45_RSP_tag
{
   tSIRF_UINT32            sv_list;
   tSIRF_MSG_AI3_NAV_SF_45 sf45[SIRF_MSG_AI3_ANS45R_NUM_SF45];
}tSIRF_MSG_AI3_NAV_SF_45_RSP;

/*-----------------------------------------------------------------------------
    6.1.15 Broadcast Ephemeris Request Message
 *----------------------------------------------------------------------------*/

/* Support definitions for Broadcast Ephemeris Message */ 
#define SIRF_MSG_AI3_BEREQ_EPH_INFO_STOP_OUTPUT           (0x00)
#define SIRF_MSG_AI3_BEREQ_EPH_INFO_IMMEDIATE_IF_NEWER    (0x01)
#define SIRF_MSG_AI3_BEREQ_EPH_INFO_WHEN_UPDATED          (0x02)
#define SIRF_MSG_AI3_BEREQ_EPH_INFO_WHEN_CHANGED          (0x04)
#define SIRF_MSG_AI3_BEREQ_EPH_INFO_MASK                  (0x07)
#define SIRF_MSG_AI3_BEREQ_EPH_TOE_SCALEFACTOR            (16)
#define ALL_SVS                                           (0xFFFFFFFF)

/* Broadcast Ephemeris Request Message, starting at EPH_INFO_FLAG */ 
typedef struct tSIRF_MSG_AI3_BE_REQ_PARAMS_tag{
   tSIRF_UINT8  eph_info_flag;
   tSIRF_UINT8  sv_prn_num;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT16 toe;
} tSIRF_MSG_AI3_BE_REQ_PARAMS;

/* Broadcast Ephemeris Request Message Format  */ 
typedef struct tSIRF_MSG_AI3_BE_REQ_tag
{
   tSIRF_UINT16 eph_resp_trigger;
   tSIRF_MSG_AI3_BE_REQ_PARAMS eph_params[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_AI3_BE_REQ;

/*-----------------------------------------------------------------------------
    6.1.16 Broadcast Ephemeris response Message
 *----------------------------------------------------------------------------*/

typedef struct tSIRF_MSG_AI3_BE_tag
{
   tSIRF_UINT8  eph_flag;
   tSIRF_UINT8  health;
   tSIRF_UINT16 gps_week; 
   /* reuse the same structure from the AI3 request message - see table 6 */
   tSIRF_MSG_AI3_EPHEMERIS_PARAMETERS eph;  
} tSIRF_MSG_AI3_BE;

typedef struct tSIRF_MSG_AI3_BE_RSP_tag
{
   tSIRF_UINT8 reserved;
   tSIRF_UINT8 iono_flag;
   /* reuse the same structure from the AI3 request message - see table 6 */
   tSIRF_MSG_AI3_IONO_DATA iono_data; 
   tSIRF_UINT8  time_flag;
   tSIRF_UINT16 extd_gps_week;
   tSIRF_UINT32 gps_tow;
   /* reuse the structure from 6.1.16 */
   tSIRF_MSG_AI3_BE be[SIRF_MAX_SVID_CNT]; 
} tSIRF_MSG_AI3_BE_RSP;

#endif /* SIRF_MSG_AI3_H_INCLUDED */

/**
 * @}
 */

