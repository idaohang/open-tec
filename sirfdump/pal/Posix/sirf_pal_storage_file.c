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
 * @file   sirf_pal_storage_file.c
 *
 * @brief  SiRF PAL storage module using the filesystem.
 */

#include <stdio.h>

#include "sirf_types.h"
#include "sirf_pal.h"



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Open the storage device for a given storage ID.
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Open( tSIRF_UINT32 storage_id )
{
   /* Unused Parameters. */
   (void)storage_id;

   /* Function not used in this implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_STORAGE_Open() */


/**
 * @brief Close the storage device for a given storage ID.
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Close( tSIRF_UINT32 storage_id )
{
   /* Unused Parameters. */
   (void)storage_id;

   /* Function not used in this implementation */
   return SIRF_SUCCESS;

} /* SIRF_PAL_STORAGE_Close() */


/**
 * @brief Write data to the storage device.
 * @param[in] storage_id               Storage ID.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Data to write.
 * @param[in] length                   Length of the data.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Write( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_PAL_STORAGE_ERROR;
   FILE         *fh;
   tSIRF_CHAR   filename[30];

   snprintf( filename, sizeof(filename)-1, "SiRFNavIIIStorage%lu.sns", storage_id );

   fh = fopen(filename,"wb");
   if ( fh != NULL )  /* check if the file opens */
   {
      if (fseek(fh, offset, SEEK_SET) == 0)/* check if we can go to the place we need get to */
      {
         if (fwrite(data,length,1,fh) == 1)/* check if the write succeeded */
         {
            result = SIRF_SUCCESS;
         }
      }
      fclose(fh);/* close the file */
   }

   return result;

} /* SIRF_PAL_STORAGE_Write() */


/**
 * @brief Read data from the storage device.
 * @param[in] storage_id               Storage ID.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Buffer to hold data.
 * @param[in] length                   Length of the data to read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Read( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_PAL_STORAGE_EMPTY;
   FILE         *fh;
   tSIRF_CHAR   filename[30];

   snprintf( filename, sizeof(filename)-1, "SiRFNavIIIStorage%lu.sns", storage_id );

   fh = fopen(filename,"rb");
   if(fh != NULL)/* check if the file opens */
   {
      if (fseek(fh, offset, SEEK_SET) == 0)/* check if we can go to the place we need get to */
      {
         if (fread(data,length,1,fh) == 1)/* check if the read succeeded */
         {
            result = SIRF_SUCCESS;
         }
      }
      fclose(fh);/* close the file */
   }

   return result;

} /* SIRF_PAL_STORAGE_Read() */



/**
 * @}
 * End of file.
 */


