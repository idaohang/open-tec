/**
 * @addtogroup platform_src_sirf_pal_common
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
 *    This Software contains SiRF Technology Inc.�s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.�s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_pal_hw_frequency_aiding.c
 *
 * @brief  Frequency Aiding Module.
 */

/* ----------------------------------------------------------------------------
 *   Included Files
 * ------------------------------------------------------------------------- */
 
#include <stdio.h>

#include "sirf_types.h"
#include "sirf_pal.h"
//#include "lpl_cmdr_msg_handler.h" /* Helper_Send_LPL_DEBUG_OUTPUT */

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

/**
 * @brief Start the frequency transfer by turning on the ECLK clock.
 *
 * @note this is a debug implmenation only.  It depends on the lpl commander
 * and hooks in and simply outputs that it would be atempting to start
 * the ECLK at that time.
 *
 * @return  SIRF_SUCCESS � ECLK is successful started
 *          SIRF_FAILURE � Failed to start ECLK
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferStart(tSIRF_VOID)
{
   return Helper_Send_LPL_DEBUG_OUTPUT(
      "SIRF_PAL_HW_FrequencyTransferStart()");
}

/**
 * @brief Stop the frequency transfer by turning off the ECLK clock.
 *
 * @note this is a debug implmenation only.  It depends on the lpl commander
 * and hooks in and simply outputs that it would be atempting to stop
 * the ECLK at that time.
 *
 * @return  SIRF_SUCCESS � ECLK is successful stopped
 *          SIRF_FAILURE � Failed to stop  ECLK
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferStop(tSIRF_VOID)
{
   return Helper_Send_LPL_DEBUG_OUTPUT(
      "SIRF_PAL_HW_FrequencyTransferStop()");

}

/**
 * @brief  This API is called by the LPL after calling
 * SiRF_PAL_HW_FrequencyTransferStart. This function will set the parameters in
 * frequency_update with the values of the ECLK. This function should return
 * SIRF_FAILURE until the frequency and accuracy are ready to be
 * used and are stable.
 *
 * @note this is a debug implmenation only.  It depends on the lpl commander
 * and hooks in and simply outputs that it would be atempting to request
 * a frequency update. The LPL commander looks for the specific debug
 * string sent and will autoreply with the GM_updateFrequency LPL commander
 * message.
 *
 * Alternatively there is some hardcoded testing code left here that can 
 * be turned on by modifying a couple of defines.
 *
 * @param[out] frequency_update  Frequency parameter to be filled if available.
 * @return    SIRF_SUCCESS � The frequency and accuracy returned are valid
 *            SIRF_FAILURE � The frequency and accuracy returned are not valid  
 *            SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING � a request is pending
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferUpdateRequest(
                                   tSIRF_PAL_HW_FREQ_UPDATE *frequency_update)
{
   tSIRF_RESULT tRet;
   static int request_id = 0;
   frequency_update->request_id          = ++request_id;

/* This is for hard coded testing.  You can test by sending a request or
 * returning immediately with the requested information.  To send a request
 * define SEND_FREQUENCY_AIDING_REQUEST_LPLCMDR_STRING.  Otherwise comment it
 * out and define one FREQUENCY_ACCURACY_XXXX define and one FREQUENCY_VALUE_XXX
 * define */

#define SEND_FREQUENCY_AIDING_REQUEST_LPLCMDR_STRING
#ifndef SEND_FREQUENCY_AIDING_REQUEST_LPLCMDR_STRING
#   define FREQUENCY_VALUE_DELTA
/* #   define FREQUENCY_VALUE_ACTUAL */
#   define FREQUENCY_ACCURACY_PPM
/* #   define FREQUENCY_ACCURACY_HZ */
#endif

#ifdef FREQUENCY_VALUE_DELTA
   frequency_update->frequency_data_type = SIRF_PAL_HW_FREQ_UPDATE_DELTA_FROM_CENTER;
   frequency_update->frequency           = 0.0;
#endif
#ifdef FREQUENCY_VALUE_ACTUAL
   frequency_update->frequency_data_type = SIRF_PAL_HW_FREQ_UPDATE_CENTER;
   frequency_update->frequency           = 1.92e7;
#endif

#ifdef FREQUENCY_ACCURACY_PPM
   frequency_update->accuracy_data_type  = SIRF_PAL_HW_FREQ_ACCURACY_PPM;
   frequency_update->accuracy            = 0.5;
#endif
#ifdef FREQUENCY_ACCURACY_HZ
   frequency_update->accuracy_data_type  = SIRF_PAL_HW_FREQ_ACCURACY_HZ;
   frequency_update->accuracy            = 5.0e5;
#endif   

#ifdef SEND_FREQUENCY_AIDING_REQUEST_LPLCMDR_STRING
   tRet = Helper_Send_LPL_DEBUG_OUTPUT(
      "SIRF_PAL_HW_FrequencyTransferUpdateRequest(): %d", 
      request_id);
   if (SIRF_SUCCESS == tRet)
   {
      return SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING;
   }
   else
   {
      return tRet;
   }
#endif

#ifndef SEND_FREQUENCY_AIDING_REQUEST_LPLCMDR_STRING
   return (SIRF_SUCCESS);
#endif
}

/**
 * @brief This API is called by the LPL after calling
 * SiRF_PAL_HW_FrequencyTransferUpdateRequest. This is used only if
 * SiRF_PAL_HW_FrequencyTransferUpdateRequest first returns
 * SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING and the frequency transfer mechanism
 * is no longer needed.
 *
 * @note this is a debug implmenation only.  It depends on the lpl commander
 * and hooks in and simply outputs that it would be atempting to cancel a
 * frequency transfer update request.
 *
 * @param[in] request_id   The original request ID returned by 
 *                         SIRF_PAL_HW_FrequencyTransferUpdateRequest.
 *
 * @return   SIRF_SUCCESS � The frequency update request was successfully
 *                           canceled
 *           SIRF_PAL_HW_FREQ_UPDATE_REQUEST_INVALID_ID � the ID passed in does
 *           not match a pending request, or that request has already
 *           been serviced
 */
tSIRF_RESULT SIRF_PAL_HW_FrequencyTransferUpdateCancel(tSIRF_UINT32 request_id)
{
   Helper_Send_LPL_DEBUG_OUTPUT(
            "SIRF_PAL_HW_FrequencyTransferUpdateCancel(): %d", 
            request_id);

   return (SIRF_SUCCESS);
}

/**
 * @}
 */
 

