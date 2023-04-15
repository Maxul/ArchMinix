/**
 * @file
 *
 * @brief RTEMS Semaphore Release
 * @ingroup ClassicSem Semaphores
 *
 * This file contains the implementation of the Classic API directive
 * rtems_semaphore_release().
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/attrimpl.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/semimpl.h>
#include <rtems/score/coremuteximpl.h>
#include <rtems/score/coresemimpl.h>
#include <rtems/score/thread.h>

#include <rtems/score/interr.h>

/*
 *  rtems_semaphore_release
 *
 *  This directive allows a thread to release a semaphore.
 *
 *  Input parameters:
 *    id - semaphore id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

#if defined(RTEMS_MULTIPROCESSING)
#define MUTEX_MP_SUPPORT _Semaphore_Core_mutex_mp_support
#else
#define MUTEX_MP_SUPPORT NULL
#endif

rtems_status_code rtems_semaphore_release(
  rtems_id   id
)
{
  Semaphore_Control          *the_semaphore;
  Objects_Locations           location;
  CORE_mutex_Status           mutex_status;
  CORE_semaphore_Status       semaphore_status;
  rtems_attribute             attribute_set;
  ISR_lock_Context            lock_context;

  the_semaphore = _Semaphore_Get_interrupt_disable(
    id,
    &location,
    &lock_context
  );
  switch ( location ) {

    case OBJECTS_LOCAL:
      attribute_set = the_semaphore->attribute_set;
#if defined(RTEMS_SMP)
      if ( _Attributes_Is_multiprocessor_resource_sharing( attribute_set ) ) {
        MRSP_Status mrsp_status;

        mrsp_status = _MRSP_Release(
          &the_semaphore->Core_control.mrsp,
          _Thread_Executing,
          &lock_context
        );
        return _Semaphore_Translate_MRSP_status_code( mrsp_status );
      } else
#endif
      if ( !_Attributes_Is_counting_semaphore( attribute_set ) ) {
        mutex_status = _CORE_mutex_Surrender(
          &the_semaphore->Core_control.mutex,
          id,
          MUTEX_MP_SUPPORT,
          &lock_context
        );
        return _Semaphore_Translate_core_mutex_return_code( mutex_status );
      } else {
        semaphore_status = _CORE_semaphore_Surrender(
          &the_semaphore->Core_control.semaphore,
          id,
          MUTEX_MP_SUPPORT,
          &lock_context
        );
        return
          _Semaphore_Translate_core_semaphore_return_code( semaphore_status );
      }

#if defined(RTEMS_MULTIPROCESSING)
    case OBJECTS_REMOTE:
      return _Semaphore_MP_Send_request_packet(
        SEMAPHORE_MP_RELEASE_REQUEST,
        id,
        0,                               /* Not used */
        MPCI_DEFAULT_TIMEOUT
      );
#endif

    case OBJECTS_ERROR:
      break;
  }

  return RTEMS_INVALID_ID;
}
