/**
 * @addtogroup platform_src_sirf_pal_common
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2008-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_os_mem_tlsf.c
 *
 * @brief  SiRF PAL dynamic memory module.
 */

#include "sirf_types.h"
#include "sirf_pal.h"
#include "sirf_pal_os_mem_internal.h"


#ifdef TLSF_32BIT
/* Size of free store - must be less than 2 MB. 32 bit 
 * TLS heap only allows 31 bits of address space.  Current heap size is
 * determined by the needs of the LPL which is 128k plus some small stuff.
 * Without details of how much "Small Stuff" is add an extra 64K for overhead
 * and the small stuff */
#define SIRF_PAL_OS_MEM_HEAP_SIZE ((128 + 64 )*1024)
#else
/* Size of free store - must be less than 65536 */
#define SIRF_PAL_OS_MEM_HEAP_SIZE 65520
#endif

static tSIRF_BOOL s_mem_initialized = SIRF_FALSE;
static tHeap s_MemoryHeap;
static tSIRF_MUTEX s_MemoryMutex;


/* Block of memory for the heap. 
   SIRF_PAL_OS_MEM_HEAP_SIZE may not be 8-byte aligned, so add 
   enough to ensure adequate space. */
static tSIRF_UINT64 s_HeapMem[ (SIRF_PAL_OS_MEM_HEAP_SIZE + sizeof(tSIRF_UINT64)) / sizeof(tSIRF_UINT64)];

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

 /**
 * @brief Stub heap error handler.
 */
static void NullHeapNotify(tHeap heap, tHeapError err, unsigned long extra)
{
   (void)heap;
   (void)err;
   (void)extra;
}

/**
 * @brief Initializes the TLSF based PAL dynamic memory subsystem.
 *
 * FYI: TLSF == Two-Level Segregated Fit
 *
 * Initialize Heap with static pool if it isn't already initialized.
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Init(void)
{
   tSIRF_RESULT result = SIRF_SUCCESS;
   if (SIRF_FALSE == s_mem_initialized)
   {
      result = SIRF_PAL_OS_MUTEX_Create(&s_MemoryMutex);
      if (SIRF_SUCCESS != result)
      {
         return result;
      }

      s_MemoryHeap = Heap_Create(NullHeapNotify, s_HeapMem, sizeof(s_HeapMem));
      if (NULL == s_MemoryHeap)
      {
         result = SIRF_FAILURE;
      }
      else
      {
         s_mem_initialized = SIRF_TRUE;
      }
  }

  return result;
}
 
/**
 * @brief Teardown the TLSF based PAL dynamic memory subsystem
 *
 * Mark subsystem as uninitialized if currently initialized.
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_OS_MEM_Destroy(void)
{
   tSIRF_RESULT result = SIRF_SUCCESS;
   if (SIRF_TRUE == s_mem_initialized)
   {
      result = SIRF_PAL_OS_MUTEX_Delete(s_MemoryMutex);
      s_mem_initialized = SIRF_FALSE;
   }

   return result;
}

/**
 * @brief TLSF based PAL allocation function
 *
 * @param size Amount of memory to allocate
 *
 * @return Memory address if successful, NULL otherwise.
 */
 void *SIRF_PAL_OS_MEM_Alloc(tSIRF_UINT32 size)
{
   void *ret;
   if (SIRF_FALSE == s_mem_initialized)
   {
      return NULL;
   }

   (void)SIRF_PAL_OS_MUTEX_Enter(s_MemoryMutex);
   ret = Heap_Malloc(s_MemoryHeap, size);
   (void)SIRF_PAL_OS_MUTEX_Exit(s_MemoryMutex);

   return ret;   
}

/**
 * @brief TLSP based PAL free function
 *
 * @param pMem   Memory block to free
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise
 */  
tSIRF_RESULT SIRF_PAL_OS_MEM_Free(void *pMem)
{
   tSIRF_RESULT result = SIRF_PAL_OS_ERROR;

   if (SIRF_TRUE == s_mem_initialized)
   {
      (void)SIRF_PAL_OS_MUTEX_Enter(s_MemoryMutex);
      Heap_Free(s_MemoryHeap, pMem);
      (void)SIRF_PAL_OS_MUTEX_Exit(s_MemoryMutex);

      result = SIRF_SUCCESS;
   }
   
   return result;
}

/**
 * @}
 */


