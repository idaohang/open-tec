/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2008 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_hw_frequency_aiding.h
 *
 * @brief  SiRF PAL hardware Frequency Aiding API.
 */

#ifndef SIRF_PAL_HW_FREQUENCY_AIDING_H_INCLUDED
#define SIRF_PAL_HW_FREQUENCY_AIDING_H_INCLUDED

#include "sirf_types.h"

/** When request is made and response will be received with call back */
#define SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING     0x1201
/** Invalid Frequency transfer update Request*/
#define SIRF_PAL_HW_FREQ_UPDATE_REQUEST_INVALID_ID  0x1202

/** Frequency update related defines */
/** Indicates frequency value is invalid and should not be used. */
#define SIRF_PAL_HW_FREQ_UPDATE_INVALID            0x00
/** Indicates frequency will be the absolute center frequency 
   of the ECLK (Nominal frequency + delta) */
#define SIRF_PAL_HW_FREQ_UPDATE_CENTER             0x01
/** Indicates freq will be the delta from the nominal frequency 
   entered by GM_setFrequencyNominalValue*/
#define SIRF_PAL_HW_FREQ_UPDATE_DELTA_FROM_CENTER  0x02

/* Frequency Accuracy related defines */
/** Indicates that Frequency accuracy is invalid and should not be used */
#define SIRF_PAL_HW_FREQ_ACCURACY_INVALID          0x00
/** Indicates that Frequency accuracy is in units of PPM */
#define SIRF_PAL_HW_FREQ_ACCURACY_PPM              0x01
/** Indicates that Frequency accuracy is in units of Hz */
#define SIRF_PAL_HW_FREQ_ACCURACY_HZ               0x02

typedef struct  tSIRF_PAL_HW_FREQ_UPDATE_tag
{
   tSIRF_UINT32 request_id;
   tSIRF_UINT32 frequency_data_type;
   tSIRF_DOUBLE frequency;
   tSIRF_UINT32 accuracy_data_type;
   tSIRF_DOUBLE accuracy;
} tSIRF_PAL_HW_FREQ_UPDATE;

/* ----------------------------------------------------------------------------
 *   Function Prototypes
 * ------------------------------------------------------------------------- */

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the frequency transfer by turning on the ECLK clock.
 *
 * @return  SIRF_SUCCESS – ECLK is successful started
 *          SIRF_FAILURE – Failed to start ECLK
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferStart( tSIRF_VOID );

/**
 * @brief Stop the frequency transfer by turning off the ECLK clock.
 *
 * @return  SIRF_SUCCESS – ECLK is successful stopped
 *          SIRF_FAILURE – Failed to stop  ECLK
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferStop( tSIRF_VOID );

/**
 * @brief  This API is called by the LPL after calling
 * SiRF_PAL_HW_FrequencyTransferStart. This function will set the parameters in
 * frequency_update with the values of the ECLK. This function should return
 * SIRF_FAILURE until the frequency and accuracy are ready to be
 * used and are stable.
 *
 * @param[out] frequency_update  Frequency parameter to be filled if available.
 * @return    SIRF_SUCCESS – The frequency and accuracy returned are valid
 *            SIRF_FAILURE – The frequency and accuracy returned are not valid  
 *            SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING – a request is pending
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferUpdateRequest(
                                tSIRF_PAL_HW_FREQ_UPDATE *frequency_update);

/**
 * @brief This API is called by the LPL after calling
 * SiRF_PAL_HW_FrequencyTransferUpdateRequest. This is used only if
 * SiRF_PAL_HW_FrequencyTransferUpdateRequest first returns
 * SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING and the frequency transfer mechanism
 * is no longer needed.
 *
 * @param[in] request_id   The original request ID returned by 
 *                         SIRF_PAL_HW_FrequencyTransferUpdateRequest.
 *
 * @return   SIRF_SUCCESS – The frequency update request was successfully
 *                           canceled
 *           SIRF_FAILURE – Failed to cancel frequency update request
 *           SIRF_PAL_HW_FREQ_UPDATE_REQUEST_INVALID_ID – the ID passed in does
 *           not match a pending request, or that request has already
 *           been serviced
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferUpdateCancel(
                                              tSIRF_UINT32 request_id);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif


#endif /* !SIRF_PAL_HW_FREQUENCY_AIDING_H_INCLUDED */

/**
 * @}
 * End of file.
 */

