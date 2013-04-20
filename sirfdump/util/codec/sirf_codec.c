/**
 * @addtogroup platform_src_sirf_util_codec
 * @ingroup host
 * @{
 */

 /**************************************************************************
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
 * MODULE:  HOST
 *
 * FILENAME:  sirf_codec.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the SiRF System/F protocol from and to the Tracker API
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <stdio.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_codec_ssb.h"

#ifdef SIRF_LOC
#include "sirf_codec_f.h"
#include "sirf_codec_stats.h"
#endif

#ifdef LPL_CMDR_INCLUDED
   #include "sirf_codec_csv.h"
   #include "sirf_codec_lplc.h"
#endif

#ifndef MIN
#define MIN(a,b)   (((a) < (b)) ? (a) : (b))
#endif

/**********************************************************************/
/* Data conversions                                                   */
/**********************************************************************/

/***************************************************
 * Import routines 
 ***************************************************/

#ifdef LPL_CMDR_INCLUDED
/** 
 * Imports a tSIRF_UINT8 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ImportUINT8( tSIRF_UINT8 *Dst, 
                          tSIRF_UINT8 **pSrc, 
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportUINT8(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_UINT8(*pSrc);
      return sizeof(tSIRF_UINT8);
   }
}
/** 
 * Imports a tSIRF_UINT16 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ImportUINT16( tSIRF_UINT16 *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportUINT16(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_UINT16(*pSrc);
      return sizeof(tSIRF_UINT16);
   }
}

/** 
 * Imports a tSIRF_INT16 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
#if 0 /* Unused. Uncomment when needed */
tSIRF_UINT32 ImportINT16( tSIRF_INT16 *Dst,
                          tSIRF_UINT8 **pSrc,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportINT16(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_SINT16(*pSrc);
      return sizeof(tSIRF_SINT16);
   }
}
#endif

/** 
 * Imports a tSIRF_UINT32 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ImportUINT32( tSIRF_UINT32 *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportUINT32(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_UINT32(*pSrc);
      return sizeof(tSIRF_UINT32);
   }
}

/** 
 * Imports a tSIRF_UINT24 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ImportUINT24( tSIRF_UINT32 *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      /* CSV of a 24 bit number will fit in a 32 bit value */
      return CSV_ImportUINT32(Dst, pSrc);
   }
   else
   {
      tSIRF_UINT8 *start = *pSrc; 
      *Dst = SIRFBINARY_IMPORT_UINT24(*pSrc);
      return (tSIRF_UINT32)*pSrc - (tSIRF_UINT32)start;
   }
}

/** 
 * Imports a tSIRF_INT24 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ImportINT24( tSIRF_INT32 *Dst,
                          tSIRF_UINT8 **pSrc,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      /* CSV of a 24 bit number will fit in a 32 bit value */
      return CSV_ImportINT32(Dst, pSrc);
   }
   else
   {
      tSIRF_UINT8 *start = *pSrc; 
      *Dst = SIRFBINARY_IMPORT_SINT24(*pSrc);
      return (tSIRF_UINT32)*pSrc - (tSIRF_UINT32)start;
   }
}

/** 
 * Imports a tSIRF_INT16 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportINT32( tSIRF_INT32 *Dst, tSIRF_UINT8 **pSrc, tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportINT32(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_SINT32(*pSrc);
      return sizeof(tSIRF_INT32);
   }
}

/** 
 * Imports a tSIRF_INT64 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportINT64( tSIRF_INT64 *Dst,
                          tSIRF_UINT8 **pSrc,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportINT64(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_SINT64(*pSrc); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_INT64);
   }
}

/** 
 * Imports a tSIRF_UINT64 from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportUINT64( tSIRF_UINT64 *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportUINT64(Dst, pSrc);
   }
   else
   {
      *Dst = SIRFBINARY_IMPORT_UINT64(*pSrc); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_UINT64);
   }
}

/** 
 * Imports a tSIRF_FLOAT from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportFLOAT( tSIRF_FLOAT *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportFLOAT(Dst, pSrc);
   }
   else
   {
      SIRFBINARY_IMPORT_FLOAT(*Dst,*pSrc); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_FLOAT);
   }
}

/** 
 * Imports a tSIRF_DOUBLE from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportDOUBLE( tSIRF_DOUBLE *Dst,
                           tSIRF_UINT8 **pSrc,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportDOUBLE(Dst, pSrc);
   }
   else
   {
      SIRFBINARY_IMPORT_DOUBLE(*Dst,*pSrc); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_DOUBLE);
   }
}


/** 
 * Imports a string from either an ascii or byte buffer
 * 
 * @param Dst[out]          where to place the data
 * @param pSrc[in]          pointer to the string to import data from
 * @param ascii_packet[in]  source points to an ascii string
 * 
 * @return number of bytes converted
 */

tSIRF_UINT32 ImportSTRING( tSIRF_UINT8 *Dst, 
                           tSIRF_UINT16 *dst_size,
                           tSIRF_UINT8 **pSrc, 
                           tSIRF_UINT32 src_size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Dst || NULL == dst_size || NULL == pSrc || NULL == *pSrc) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ImportSTRING(Dst,dst_size,pSrc,src_size);
   }
   else
   {
      if (sizeof(tSIRF_UINT16) < src_size)
      {
         tSIRF_UINT16 len;
         tSIRF_UINT8 *tmp = *pSrc;
         len = SIRFBINARY_IMPORT_UINT16(*pSrc);
         /* Min of max room and what we expect the buffer to be */
         *dst_size = MIN(*dst_size,len);
         /* Min of above and input size */
         *dst_size = (tSIRF_UINT16)MIN(*dst_size,src_size-sizeof(tSIRF_UINT16));
         memcpy(Dst,*pSrc,*dst_size);
         *pSrc += *dst_size;
         return *pSrc - tmp;
      }
      else
      {
         return 0;
      }
   }
}

/***************************************************/
/* Export routines to be used here only            */
/***************************************************/

/** 
 * Exports a tSIRF_UINT8 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportUINT8( tSIRF_UINT8 *Src,
                          tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportUINT8(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT8(*Src,*pDst);
      return sizeof(tSIRF_UINT8);
   }
}

/** 
 * Exports a tSIRF_INT16 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
#if 0 /* Unused. Uncomment when needed */
tSIRF_UINT32 ExportINT16( tSIRF_INT16 *Src,
                          tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportINT16(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT16(*Src,*pDst);
      return sizeof(tSIRF_INT16);
   }
}
#endif

/** 
 * Exports a tSIRF_UINT16 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportUINT16( tSIRF_UINT16 *Src,
                           tSIRF_UINT8 **pDst,
                           tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet)
   {
      return CSV_ExportUINT16(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT16(*Src,*pDst);
      return sizeof(tSIRF_UINT16);
   }
}
/** 
 * Exports a tSIRF_INT24 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportINT24( tSIRF_INT32 *Src,
                          tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      /* Input is a 32 bit signed number store as such */
      return CSV_ExportINT32(Src, pDst,size);
   }
   else
   {
      tSIRF_UINT8* start = *pDst;
      SIRFBINARY_EXPORT24(*Src,*pDst);
      return (tSIRF_INT32)*pDst - (tSIRF_UINT32)start;
   }
}

/** 
 * Exports a tSIRF_UINT24 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportUINT24( tSIRF_UINT32 *Src,
                           tSIRF_UINT8 **pDst,
                           tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      /* Input is a 32 bit signed number store as such */
      return CSV_ExportUINT32(Src, pDst,size);
   }
   else
   {
      tSIRF_UINT8* start = *pDst;
      SIRFBINARY_EXPORT24(*Src,*pDst);
      return (tSIRF_INT32)*pDst - (tSIRF_UINT32)start;
   }
}

/** 
 * Exports a tSIRF_INT32 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportINT32( tSIRF_INT32 *Src,
                          tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportINT32(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT32(*Src,*pDst);
      return sizeof(tSIRF_INT32);
   }
}

/** 
 * Exports a tSIRF_UINT32 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportUINT32( tSIRF_UINT32 *Src,
                           tSIRF_UINT8 **pDst,
                           tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportUINT32(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT32(*Src,*pDst);
      return sizeof(tSIRF_UINT32);
   }
}

/** 
 * Exports a tSIRF_INT64 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportINT64( tSIRF_INT64 *Src,
                          tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                          tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportINT64(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT64(*Src,*pDst);
      return sizeof(tSIRF_INT64);
   }
}

/** 
 * Exports a tSIRF_UINT64 to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportUINT64( tSIRF_UINT64 *Src,
                           tSIRF_UINT8 **pDst,
                           tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportUINT64(Src, pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT64(*Src,*pDst);
      return sizeof(tSIRF_UINT64);
   }
}

/** 
 * Exports a tSIRF_FLOAT to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportFLOAT( tSIRF_FLOAT *Src,
                           tSIRF_UINT8 **pDst,
                          tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportFLOAT(Src,pDst,size);
   }
   else
   {
      SIRFBINARY_EXPORT32(*Src,*pDst); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_FLOAT);
   }
}

/** 
 * Exports a tSIRF_DOUBLE to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportDOUBLE( tSIRF_DOUBLE *Src,
                           tSIRF_UINT8 **pDst,
                           tSIRF_UINT32 size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == pDst || NULL == Src || NULL == *pDst) 
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportDOUBLE(Src,pDst,size);
   }
   else
   {
      if (sizeof(tSIRF_DOUBLE) > size) 
      {
         return 0;
      }
      SIRFBINARY_EXPORT_DOUBLE(*Src,*pDst); /* pSrc is updated +8 bytes */
      return sizeof(tSIRF_DOUBLE);
   }
}

/** 
 * Exports a string to either an ascii or byte buffer
 * 
 * @param Src[in]           pointer to data to export
 * @param pDst[out]         where to place the data
 * @param ascii_packet[in]  Export as an ascii string
 * 
 * @return number of bytes converted
 */
tSIRF_UINT32 ExportSTRING( tSIRF_UINT8 *Src, 
                           tSIRF_UINT16 src_size,
                           tSIRF_UINT8 **pDst, 
                           tSIRF_UINT32 dst_size,
                           tSIRF_BOOL ascii_packet )
{
   if (NULL == Src || NULL == pDst || NULL == *pDst)
   {
      return 0;
   }

   if (ascii_packet) 
   {
      return CSV_ExportSTRING(Src,src_size,pDst,dst_size);
   }
   else
   {
      if (sizeof(tSIRF_UINT16) < src_size)
      {
         tSIRF_UINT16 len = (tSIRF_UINT16)MIN(src_size,(dst_size-sizeof(tSIRF_UINT16)));
         tSIRF_UINT8 *tmp = *pDst;
         SIRFBINARY_EXPORT16(len,*pDst);
         memcpy(*pDst,Src,len);
         *pDst += len;
         return *pDst - tmp;
      }
      else
      {
         return 0;
      }
   }
}

#endif

#ifdef SIRF_LOC
/** 
 * Encode function for NMEA.  This function encodes the current encoded
 * output format of the NMEA messages for the strict purpose of the SLC
 * protocol.  The SLC protocol embeds the NMEA strings within a payload that 
 * needs to contain the logical channel.  In all the other codecs the logical 
 * channel is considered part of the payload so it must be added here.
 *
 * This function is NOT in sirf_codec_nmea.c because we expect to fix the
 * SN code to not output NMEA sentences but instead to output NMEA structures
 * so that the encode and decode of the structures is no longer necessary
 * reducing unnecessary computation.  Since none of the code in 
 * sirf_codec_nmea.c is currently used, but we don't want to delete it, I've
 * placed this code temporarily here until it can get refactored.
 * 
 * @param message_id         Id of the message to encode
 * @param message_structure  pointer to the message structure or data to encode
 * @param message_length     length of the data pointed to by message_length
 * @param packet             [out] pointer to store the encoded message
 * @param packet_length      [in/out] Size of packet upon entry, 
 *                           amount stored to packet upon exit
 * 
 * @return SIRF_SUCCESS or a SIRF_CODEC_ERROR_XXX error code.
 */
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode( tSIRF_UINT32  message_id,
                                            tSIRF_VOID   *message_structure,
                                            tSIRF_UINT32  message_length,
                                            tSIRF_UINT8  *packet,
                                            tSIRF_UINT32 *packet_length )
{
   (void) message_id; /* unused */
   if(NULL == message_structure ||
      NULL == packet ||
      NULL == packet_length)
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   /* We are adding a single byte to the payload, the logical channel */
   if (*packet_length >= message_length + 1)
   {
      packet[0] = (tSIRF_UINT8) SIRF_LC_NMEA;
      memcpy(&packet[1],message_structure,message_length);
      *packet_length = message_length + 1;
      return SIRF_SUCCESS;
   } 
   else
   {
      return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
   }
}
#endif
/** 
 * Generic Encode function.  If a particular logical channel and message 
 * is supported it will return with the packet, and packet_length filled out, 
 * otherwise a return code will be generated.
 * 
 * @param message_id         Id of the message to encode
 * @param message_structure  pointer to the message structure or data to encode
 * @param message_length     length of the data pointed to by message_length
 * @param packet             [out] pointer to store the encoded message
 * @param packet_length      [in/out] Size of packet upon entry, 
 *                           amount stored to packet upon exit
 * 
 * @return SIRF_SUCCESS or a SIRF_CODEC_ERROR_XXX error code.
 */

tSIRF_RESULT SIRF_CODEC_Encode( tSIRF_UINT32  message_id,
                                tSIRF_VOID   *message_structure,
                                tSIRF_UINT32  message_length,
                                tSIRF_UINT8  *packet,
                                tSIRF_UINT32 *packet_length )
{
   tSIRF_RESULT tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
   tSIRF_UINT8 LogicalChannel = SIRF_GET_LC(message_id);

#ifdef SIRF_LOC
   switch (LogicalChannel)
   {
      case SIRF_LC_SSB:
         tRet = SIRF_CODEC_SSB_Encode( message_id, message_structure, message_length,
                                       packet, packet_length );
         break;

      case SIRF_LC_F:
         tRet = SIRF_CODEC_F_EncodeSlc( message_id, message_structure,
                                        packet, packet_length );
         break;

      case SIRF_LC_STATS:
         tRet = SIRF_CODEC_STATS_encode( message_id, message_structure, message_length,
                                         packet, packet_length );
         break;

#ifdef LPL_CMDR_INCLUDED
      case SIRF_LC_LPLC:
         /* Currently a special case for the ASCII codec and must be called seperately until
          * if and when we decide to add similar support to the other codec API's
          */
         tRet = SIRF_CODEC_LPLC_Encode( message_id, message_structure, message_length,
                                        packet, packet_length, SIRF_FALSE );
         break;
#endif

      case SIRF_LC_NMEA:
         tRet = SIRF_CODEC_NMEA_Encode( message_id, message_structure, message_length,
                                        packet, packet_length );
         break;
         
      case SIRF_LC_AI3:
      case SIRF_LC_DEBUG:
      default:
         break;
   }
#else
   if (SIRF_LC_SSB == LogicalChannel)
   {
      tRet = SIRF_CODEC_SSB_Encode( message_id, message_structure, message_length,
                                    packet, packet_length );
   }
#endif /* SIRF_LOC */

   return tRet;
}

/**
 * @}
 */
