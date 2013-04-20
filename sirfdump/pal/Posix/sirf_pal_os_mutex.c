/**
 * @addtogroup platform_src_sirf_pal_posix
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
 *
 */

/**
 * @file   sirf_pal_os_mutex.c
 *
 * @brief  SiRF PAL mutex module.
 */

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_pal.h"

#include "sirf_pal_config.h"



/* ----------------------------------------------------------------------------
 *    Constants
 * ------------------------------------------------------------------------- */



/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */



/* Debugging definitions (addition to GPS_debug.h) */
/* We want to use our own version of VERIFY; do not change this */

#ifdef VERIFY
#  undef VERIFY
#endif /* VERIFY */

#ifdef DEBUG
#  define VERIFY(x) DEBUGCHK((x))
#else
#  define VERIFY(x) ((void)(x))
#endif /* DEBUG */



/* ----------------------------------------------------------------------------
 *   Types
 * ------------------------------------------------------------------------- */



/** 
 * @brief Internal Mutex structure. 
 *
 * This structure is used to order posix mutexes on a free list
 * in order to allow any sequencing of mutex create/delete operations.
 */
typedef struct tPALMutex_tag
{
   struct tPALMutex_tag * m_pNext;     /**< Next in the free list. */
   tSIRF_BOOL             m_bCreated;  /**< Created flag. */
   pthread_mutex_t        m_mutex;     /**< Posix mutex. */
} tPALMutex;



/* ----------------------------------------------------------------------------
 *   Local Variables
 * ------------------------------------------------------------------------- */



/** Mutex for managing mutexes. */
static pthread_mutex_t s_mutex_crit;

/** Pool of application mutexes. */
static tPALMutex s_mutex_pool[SIRF_PAL_OS_MUTEX_MAX];

/** Mutex free list. */
static tPALMutex * s_mutex_free = NULL;

/** Module Initialized flag. */
static tSIRF_BOOL s_mutex_initialized = SIRF_FALSE;



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



tSIRF_RESULT SIRF_PAL_OS_MUTEX_Create(
      tSIRF_MUTEX * mx_handle)
{
   pthread_mutexattr_t mta;
   tPALMutex *p_mutex;
   
   /* Validate the output semaphore handle pointer. */
   if (NULL == mx_handle)
      return SIRF_PAL_OS_ERROR;

   /* Initialize the globals if needed. */
   if (!s_mutex_initialized)
   {
      int i;
   
      /* Create the mutex management mutex. */
      if (pthread_mutex_init(&s_mutex_crit, NULL) != 0)
         return SIRF_PAL_OS_ERROR;

      /* Zero all descriptors. */
      memset(s_mutex_pool, 0, sizeof(s_mutex_pool));
      
      /* Initialize all descriptors. */
      s_mutex_free = NULL;
      for (i = 0; i < SIRF_PAL_OS_MUTEX_MAX; ++i)
      {
         /* Add to the free list. */
         s_mutex_pool[i].m_pNext = s_mutex_free;
         s_mutex_free = &s_mutex_pool[i];
      }

      /* Flag the module as initialized. */
      s_mutex_initialized = SIRF_TRUE;
   }
   
   /* Get the next free descriptor (if any). */
   pthread_mutex_lock(&s_mutex_crit);
   p_mutex = s_mutex_free;
   if (NULL != p_mutex)
      s_mutex_free = p_mutex->m_pNext;
   pthread_mutex_unlock(&s_mutex_crit);

   /* Save the tPALMutex (or NULL) for the return. */
   *mx_handle = (tSIRF_MUTEX)p_mutex;
   if (NULL == p_mutex)
      return SIRF_PAL_OS_ERROR;

   /* Initialize the mutex attributes. */
   if (pthread_mutexattr_init(&mta) != 0)
      return SIRF_PAL_OS_ERROR;
   
   /* Set the mutex type. */
#ifdef __linux__
   if (pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE_NP) != 0)
#else /* !__linux__ */
   if (pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE) != 0)
#endif /* __linux__ */
      return SIRF_PAL_OS_ERROR;
      
   /* Create the mutex. */
   if (pthread_mutex_init(&p_mutex->m_mutex, &mta) != 0)
      return SIRF_PAL_OS_ERROR;
   
   /* Destroy the mutex attributes object. */
   if (pthread_mutexattr_destroy(&mta) != 0)
      return SIRF_PAL_OS_ERROR;

   /* Return success. */
   p_mutex->m_bCreated = SIRF_TRUE;
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_MUTEX_Create() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Delete( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
      return SIRF_PAL_OS_ERROR;

   /* Delete the resources. */
   if (pthread_mutex_destroy(&p_mutex->m_mutex) != 0)
      return SIRF_PAL_OS_ERROR;
   
   /* Mark as deleted and place on the free list. */
   p_mutex->m_bCreated = SIRF_FALSE;
   pthread_mutex_lock(&s_mutex_crit);
   p_mutex->m_pNext = s_mutex_free;
   s_mutex_free = p_mutex;
   pthread_mutex_unlock(&s_mutex_crit);

   /* Return success. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_MUTEX_Delete() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Enter( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;
   int result;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
      return SIRF_PAL_OS_ERROR;

   /* Enter the mutex. */
   result = pthread_mutex_lock(&p_mutex->m_mutex);

   /* Examine the return value. */
   if (result == 0)
      return SIRF_SUCCESS;
   else
      return SIRF_PAL_OS_ERROR;

} /* SIRF_PAL_OS_MUTEX_Enter() */

tSIRF_RESULT SIRF_PAL_OS_MUTEX_Exit( tSIRF_MUTEX mx_handle )
{
   tPALMutex * p_mutex = (tPALMutex*)mx_handle;
   int result;

   /* Ensure the handle is valid and created. */
   if (NULL == p_mutex || !p_mutex->m_bCreated)
      return SIRF_PAL_OS_ERROR;

   /* Enter the mutex. */
   result = pthread_mutex_unlock(&p_mutex->m_mutex);

   /* Examine the return value. */
   if (result == 0)
      return SIRF_SUCCESS;
   else
      return SIRF_PAL_OS_ERROR;

} /* SIRF_PAL_OS_MUTEX_Exit() */



/**
 * @}
 * End of file.
 */


