/**
 * @addtogroup platform_src_sirf_pal_posix
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2007-2008 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_config.h
 *
 * @brief  SiRF PAL platform definitions.
 */

#ifndef SIRF_PAL_CONFIG_H_INCLUDED
#define SIRF_PAL_CONFIG_H_INCLUDED

#ifdef SIRF_HOST
#include "sirfnav_config.h"
#else
#define SIRFNAV_SEM_MAX     0
#define SIRFNAV_MUTEX_MAX   0
#endif


/* Includes and Configuration for SIRFNAV_DEMO */

#ifdef SIRFNAV_DEMO
#include "sirfnav_demo_config.h"
#else
#define SIRFNAV_DEMO_SEM_MAX     0
#define SIRFNAV_DEMO_MUTEX_MAX   0
#endif


/* Includes and Configuration for LPL */

#ifdef SIRF_LPL
#include "sirflpl_config.h"
#else
#define LPL_SEM_MAX     0
#define LPL_MUTEX_MAX   0
#endif

/* Includes and Configuration for CLM */

#ifdef SIRF_CLM
#include "sirfclm_config.h"
#else
#define SIRFCLM_MUTEX_MAX   0
#endif

/* ----------------------------------------------------------------------------
 *   Platform Definitions
 * ------------------------------------------------------------------------- */



/** Timer tick resolution in milliseconds */
#define OS_TIMER_TICK            1

/** Calculate total amount of semaphore resources needed: */
#define SIRF_PAL_OS_SEM_MAX    (SIRFNAV_SEM_MAX + SIRFNAV_DEMO_SEM_MAX + LPL_SEM_MAX )

/** Calculate total amount of mutex resources needed: */
#define SIRF_PAL_OS_MUTEX_MAX  (SIRFNAV_MUTEX_MAX + SIRFNAV_DEMO_MUTEX_MAX + LPL_MUTEX_MAX + SIRFCLM_MUTEX_MAX)



#endif /* !SIRF_PAL_CONFIG_H_INCLUDED */

/**
 * @}
 * End of file.
 */


