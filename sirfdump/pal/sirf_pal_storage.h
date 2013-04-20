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
 * @file   sirf_pal_storage.h
 *
 * @brief  SiRF PAL storage API.
 */

#ifndef SIRF_PAL_STORAGE_H_INCLUDED
#define SIRF_PAL_STORAGE_H_INCLUDED

#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */



/* SiRF PAL storage return codes. */
#define SIRF_PAL_STORAGE_NOT_AVAILABLE   0x2210
#define SIRF_PAL_STORAGE_FULL            0x2211
#define SIRF_PAL_STORAGE_EXISTS          0x2212
#define SIRF_PAL_STORAGE_ERROR           0x2213
#define SIRF_PAL_STORAGE_LENGTH_TOO_BIG  0x2214
#define SIRF_PAL_STORAGE_EMPTY           0x2215

typedef enum
{
   SIRF_PAL_STORAGE_BBRAM,  /* for BBRAM */
   SIRF_PAL_STORAGE_FLASH,  /* for FLASH */
   SIRF_PAL_STORAGE_PATCH,  /* for Host PM */
   SIRF_PAL_STORAGE_CLM,    /* for CLM */
   SIRF_PAL_STORAGE_LPL,    /* for LPL */
   SIRF_PAL_STORAGE_MAX
}SirfPalStorageEnum;


/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

tSIRF_RESULT SIRF_PAL_STORAGE_Open(  tSIRF_UINT32 storage_id );
tSIRF_RESULT SIRF_PAL_STORAGE_Close( tSIRF_UINT32 storage_id );

tSIRF_RESULT SIRF_PAL_STORAGE_Write( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length );
tSIRF_RESULT SIRF_PAL_STORAGE_Read(  tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length );

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_STORAGE_H_INCLUDED */

/**
 * @}
 * End of file.
 */


