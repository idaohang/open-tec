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
 * @file   sirf_pal_os_semaphore.c
 *
 * @brief  SiRF PAL semaphore module.
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_pal.h"

#include "sirf_pal_config.h"



/* ----------------------------------------------------------------------------
 *    Constants
 * ------------------------------------------------------------------------- */



/* ----------------------------------------------------------------------------
 *    Types
 * ------------------------------------------------------------------------- */



/** 
 * @brief Internal Semaphore structure. 
 *
 * This structure is used to order posix semaphores on a free list
 * in order to allow any sequencing of semaphore create/delete operations.
 */
typedef struct tPALSemaphore_tag
{
   struct tPALSemaphore_tag * m_pNext;     /**< Next in the free list. */
   tSIRF_BOOL                 m_bCreated;  /**< Created flag. */
   sem_t                      m_sem;       /**< Posix semaphore. */
} tPALSemaphore;



/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */



/* Debugging definitions (addition to GPS_debug.h) */
/* We want to use our own version of VERIFY; do not change this */

#ifdef VERIFY
   #undef VERIFY
#endif /* VERIFY */

#ifdef DEBUG
   #define VERIFY(x) DEBUGCHK((x))
#else
   #define VERIFY(x) ((void)(x))
#endif /* DEBUG */



/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */



/** Mutex for managing semaphores. */
static pthread_mutex_t s_sem_crit;

/** Pool of application semaphores. */
static tPALSemaphore s_sem_pool[SIRF_PAL_OS_SEM_MAX];

/** Semaphore free list. */
static tPALSemaphore * s_sem_free = NULL;

/** Module Initialized flag. */
static tSIRF_BOOL s_sem_initialized = SIRF_FALSE;



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Create(
      tSIRF_SEMAPHORE * sem_handle, 
      tSIRF_UINT32      init_value)
{
   tPALSemaphore *p_sem;
   
   /* Validate the output port handle pointer. */
   if (NULL == sem_handle)
      return SIRF_PAL_OS_ERROR;

   /* Initialize the globals if needed. */
   if (!s_sem_initialized)
   {
      int i;
   
      /* Create the semaphore management mutex. */
      if (pthread_mutex_init(&s_sem_crit, NULL) != 0)
         return SIRF_PAL_OS_ERROR;

      /* Zero all descriptors. */
      memset(s_sem_pool, 0, sizeof(s_sem_pool));
      
      /* Initialize all descriptors. */
      s_sem_free = NULL;
      for (i = 0; i < SIRF_PAL_OS_SEM_MAX; ++i)
      {
         /* Add to the free list. */
         s_sem_pool[i].m_pNext = s_sem_free;
         s_sem_free = &s_sem_pool[i];
      }

      /* Flag the module as initialized. */
      s_sem_initialized = SIRF_TRUE;
   }
   
   /* Get the next free descriptor (if any). */
   pthread_mutex_lock(&s_sem_crit);
   p_sem = s_sem_free;
   if (NULL != p_sem)
      s_sem_free = p_sem->m_pNext;
   pthread_mutex_unlock(&s_sem_crit);

   /* Save the tPALSemaphore (or NULL) for the return. */
   *sem_handle = (tSIRF_SEMAPHORE)p_sem;
   if (NULL == p_sem)
      return SIRF_PAL_OS_ERROR;

   /* Initialize the descriptor members. */
   p_sem->m_bCreated = SIRF_TRUE;
   if (sem_init(&p_sem->m_sem, 0, init_value) != 0)
      return SIRF_PAL_OS_ERROR;

   /* Return success. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Create() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Delete(
      tSIRF_SEMAPHORE sem_handle)
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
      return SIRF_PAL_OS_ERROR;

   /* Delete the resources. */
   if (sem_destroy(&p_sem->m_sem) != 0)
      return SIRF_PAL_OS_ERROR;
   
   /* Mark as deleted and place on the free list. */
   p_sem->m_bCreated = SIRF_FALSE;
   pthread_mutex_lock(&s_sem_crit);
   p_sem->m_pNext = s_sem_free;
   s_sem_free = p_sem;
   pthread_mutex_unlock(&s_sem_crit);

   /* Return success. */
   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Delete() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Wait(
      tSIRF_SEMAPHORE sem_handle, 
      tSIRF_UINT32    timeout)
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;
   int ret;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
      return SIRF_PAL_OS_ERROR;

   /* Handle infinite timeout. */
   if ( timeout == SIRF_TIMEOUT_INFINITE )
   {
      /* Try to grab the semaphore. */
      do
         ret = sem_wait(&p_sem->m_sem);
      while ( ret==-1 && errno==EINTR);
   }
   else
   {
      /* Get the current time. */
      struct timespec abs_timeout;
      ret = clock_gettime(CLOCK_REALTIME, &abs_timeout);
      if (ret != 0)
      {
         /* Could not get the curret time. */
         DEBUGMSG(1, (DEBUGTXT("OS_Semaphore_Wait: could not get current time, errno=%d\n"), errno));
         return SIRF_PAL_OS_ERROR;
      }

      /* Add the timeout to the absolute timeout structure and handle overflow. */
      abs_timeout.tv_sec  +=  timeout / 1000;
      abs_timeout.tv_nsec += (timeout % 1000) * 1000000;
      if (abs_timeout.tv_nsec >= 1000000000 )
      {
         abs_timeout.tv_nsec -= 1000000000;
         abs_timeout.tv_sec++;
      }

      /* Try to grab the semaphore. */
      do
         ret = sem_timedwait(&p_sem->m_sem, &abs_timeout);
      while ( ret==-1 && errno==EINTR);
   }

   /* Handle any errors. */
   if ( ret != 0 )
   {
      #ifdef _LINUX_
      if ( ret == ETIMEDOUT )
      #else
      if ( errno == ETIMEDOUT )
      #endif
      {
         return SIRF_PAL_OS_SEMAPHORE_WAIT_TIMEOUT;
      }
      else
      {
         DEBUGMSG(1, (DEBUGTXT("OS_Semaphore_Wait: ret==%d, errno=%d\n"), ret, errno));
         return SIRF_PAL_OS_ERROR;
      }
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Wait() */

tSIRF_RESULT SIRF_PAL_OS_SEMAPHORE_Release( tSIRF_SEMAPHORE sem_handle )
{
   tPALSemaphore * p_sem = (tPALSemaphore*)sem_handle;

   /* Ensure the handle is valid and created. */
   if (NULL == p_sem || !p_sem->m_bCreated)
      return SIRF_PAL_OS_ERROR;
      
   /* Attempt to release the semaphore. */
   if (sem_post(&p_sem->m_sem) != 0)
      return SIRF_PAL_OS_ERROR;

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_SEMAPHORE_Release() */



/**
 * @}
 * End of file.
 */


