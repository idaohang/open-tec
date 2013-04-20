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
 * @file   sirf_pal_os_mem_internal.h
 *
 * @brief  Generic heap API.
 *
 * This header file defines the public interface for heap management.
 */

#ifndef __SIRF_PAL_OS_MEM_INTERNAL_H__
#define __SIRF_PAL_OS_MEM_INTERNAL_H__



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */



#include <stddef.h>



/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */



/**
 * @brief Heap handle.
 * This handle identifies a heap constructed via Heap_Create(). The heap 
 * handle must be used in all subsequent heap calls.
 */
typedef unsigned char * tHeap;

/**
 * @brief Enumeration of heap errors.
 * This enumeration contains all possible heap errors that may be reported
 * via the registered tHeapCallback function.
 */
typedef enum
{
    /**
     * @brief No error.
     * This zero-value will never be send by the callback routine it exists
     * to be used as an initial value for a global or structure variable.
     */
    HE_NoError,
    
    /**
     * @brief Bad allocation request.
     * This is called from Heap_Malloc() when the requested memory size
     * can't be satisfied by the heap. 
     */
    HE_BadRequest,

    /**
     * @brief The heap is out of memory.
     * This is called from Heap_Malloc() when there is not enough contiguous
     * free memory to satisfy the request.
     */
    HE_OutOfMemory,
    
    /**
     * @brief The heap has been corrupted.
     * This is usually called from Heap_Walk(), although any heap function
     * may trigger this event upon detecting a corruption in the heap.
     */
    HE_BlockCorrupted,

    /**
     * @brief The argument to free wasn't a valid block
     * This could happen if a block is freed twice or if a random
     * pointer is passed to free.
     */
    HE_BadFree
} tHeapError;

/**
 * @brief Heap callback procedure.
 * This procedure is executed when an error occurs within the heap library
 * such as running out of memory.
 *
 * @param[in] hHeap     Heap reporting the error.
 * @param[in] error     Specific error encountered.
 * @param[in] extra     Error-dependent additional information
 */
typedef void (*tHeapCallback)(
        tHeap hHeap,
        tHeapError error,
        unsigned long extra);

/**
 * @brief Overall heap metrics
 */
typedef struct
{
    /** Maximum free block in the heap. */
    size_t max_free;
    
    /** Total allocated memory in the heap. */
    size_t total;
    
    /** User allocated memory in the heap. */
    size_t user;
    
    /** Size of overhead (book-keeping) data in the heap. */
    size_t ohead;
    
    /** Number of heap blocks. */
    unsigned nblock;
} tHeapMetric;



/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/



/**
 * @brief Open the heap management module.
 * This function initializes global heap objects such as critical sections.
 */
extern void Heap_Open(void);

/**
 * @brief Close the heap management module.
 * This function releases global heap objects such as critical sections.
 */
extern void Heap_Close(void);

/**
 * @brief Create a new heap.
 * This function constructs a heap in the memory supplied.
 *
 * @param[in] p_callback    Heap callback function.
 * @param[in] ptr           Heap Memory Pointer
 * @param[in] len           Heap Memory Length
 * @return                  New Heap.
 */
extern tHeap Heap_Create(tHeapCallback p_callback, void *ptr, size_t len);

/**
 * @brief Allocate memory from the heap.
 * This function allocates memory from the heap and returns
 * a pointer to it.
 *
 * @param[in] p_heap        Heap handle (from Heap_Create).
 * @param[in] len           Memory request length.
 * @return                  Pointer to allocated memory.
 */
extern void* Heap_Malloc(tHeap p_heap, size_t len);

/**
 * @brief Free memory back to the heap.
 * This function frees memory previously allocated from Heap_Malloc.
 *
 * @param[in] p_heap        Heap handle (from Heap_Create).
 * @param[in] ptr           Memory pointer (from Heap_Malloc).
 */
extern void Heap_Free(tHeap p_heap, void *ptr);



/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* __SIRF_PAL_OS_MEM_INTERNAL_H__ */

/**
 * @}
 */

