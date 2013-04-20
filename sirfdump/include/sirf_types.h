/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2008 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_types.h
 *
 * @brief  SiRF standard types and constants.
 */

#ifndef SIRF_TYPES_H_INCLUDED
#define SIRF_TYPES_H_INCLUDED

#include <stdint.h>
#ifdef _MSC_VER
#define bool unsigned char
#else
#include <stdbool.h>
#endif

/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *   SiRF data types
 * ------------------------------------------------------------------------- */



#define tSIRF_VOID void

typedef bool		        tSIRF_BOOL;
typedef unsigned char           tSIRF_UCHAR;

typedef char                    tSIRF_CHAR;

typedef int8_t                  tSIRF_INT8;
typedef uint8_t                 tSIRF_UINT8;

typedef int16_t                 tSIRF_INT16;
typedef uint16_t                tSIRF_UINT16;

typedef int32_t                 tSIRF_INT32;
typedef uint32_t                tSIRF_UINT32;

typedef double                  tSIRF_DOUBLE;
typedef float                   tSIRF_FLOAT;

typedef void *                  tSIRF_HANDLE;

typedef unsigned long int       tSIRF_RESULT;

typedef int64_t tSIRF_INT64;
typedef uint64_t tSIRF_UINT64;

#ifdef TOOLCHAIN_VC8
   /** Support snprintf on Visual Studio 8 (2005) */
   #define snprintf _snprintf
#endif
/* ----------------------------------------------------------------------------
 *   SiRF constants
 * ------------------------------------------------------------------------- */


#define SIRF_FALSE                        (0)
#define SIRF_TRUE               (!SIRF_FALSE)

#define SIRF_SUCCESS                      (0)
#define SIRF_FAILURE                      (1)
#define SIRF_TIMEOUT_INFINITE    (0xFFFFFFFF)



#endif /* !SIRF_TYPES_H_INCLUDED */

/**
 * @}
 * End of file.
 */


