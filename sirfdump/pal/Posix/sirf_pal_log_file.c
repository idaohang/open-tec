/**
 * @addtogroup platform_src_sirf_pal_posix
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
 * @file   sirf_pal_log_file.c
 *
 * @brief  Logging module implementation using file I/O.
 */

#ifdef SIRF_EXT_LOG

#include <stdio.h>

#include "sirf_types.h"
#include "sirf_pal.h"



/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Local Functions
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */


/**
 * @brief Open the specified file for storing logging data.
 * @param[in]  filename           File name to open.
 * @param[out] log                Handle of the opened file.
 * @param[in]  mode               File open mode bitmask; modes can be combined.
 * @return                        Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Open( tSIRF_CHAR *filename, tSIRF_LOG_HANDLE *log, tSIRF_UINT32 mode  )
{
   FILE *log_file = NULL;

   if ( (mode & SIRF_PAL_LOG_MODE_MASK) == SIRF_PAL_LOG_MODE_OVERWRITE)
   {
      log_file = fopen( filename, "wb" ); 
   }
   else if ( (mode & SIRF_PAL_LOG_MODE_MASK) == SIRF_PAL_LOG_MODE_APPEND)
   {
      log_file = fopen( filename, "a+b" ); 
   }

   *log = (tSIRF_LOG_HANDLE)log_file;

   if ( NULL == log_file )
   {
      return SIRF_PAL_LOG_OPEN_ERROR;
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Open() */


/**
 * @brief Close the current log file.
 * @param[in] log                Handle of the opened file.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Close( tSIRF_LOG_HANDLE log )
{
   if ( NULL == log )
   {
      return SIRF_PAL_LOG_ALREADY_CLOSED;
   }

   fclose( (FILE *)log );

   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Close() */


/**
 * @brief Write data to the currently open log file.
 * @param[in] log                Handle of the opened file.
 * @param[in] text               Text data to log.
 * @param[in] length             Length of the text data.
 * @return                       Success code.
 */
tSIRF_RESULT SIRF_PAL_LOG_Write( tSIRF_LOG_HANDLE log, tSIRF_CHAR *text, tSIRF_UINT32 length )
{
   if ( NULL == log )
   {
      return SIRF_PAL_LOG_NOT_OPEN;
   }

   fwrite( text, 1, length, (FILE *)log );

   return SIRF_SUCCESS;

} /* SIRF_PAL_LOG_Write() */

#endif /* SIRF_EXT_LOG */



/**
 * @}
 * End of file.
 */


