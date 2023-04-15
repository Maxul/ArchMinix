/**
 *  @file
 *
 *  @brief Status Mapping Arrays
 *  @ingroup ClassicStatus
 */

/*  COPYRIGHT (c) 1989-2013.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#include <rtems/rtems/statusimpl.h>
#include <errno.h>

const rtems_status_code _Status_Object_name_errors_to_status[] = {
  /** This maps OBJECTS_SUCCESSFUL to RTEMS_SUCCESSFUL. */
  RTEMS_SUCCESSFUL,
  /** This maps OBJECTS_INVALID_NAME to RTEMS_INVALID_NAME. */
  RTEMS_INVALID_NAME,
  /** This maps OBJECTS_INVALID_ADDRESS to RTEMS_INVALID_NAME. */
  RTEMS_INVALID_ADDRESS,
  /** This maps OBJECTS_INVALID_ID to RTEMS_INVALID_ADDRESS. */
  RTEMS_INVALID_ID,
  /** This maps OBJECTS_INVALID_NODE to RTEMS_INVALID_NODE. */
  RTEMS_INVALID_NODE
};

static const int status_code_to_errno [RTEMS_STATUS_CODES_LAST + 1] = {
  [RTEMS_SUCCESSFUL]               = 0,
  [RTEMS_TASK_EXITTED]             = EIO,
  [RTEMS_MP_NOT_CONFIGURED]        = EIO,
  [RTEMS_INVALID_NAME]             = EINVAL,
  [RTEMS_INVALID_ID]               = EIO,
  [RTEMS_TOO_MANY]                 = EIO,
  [RTEMS_TIMEOUT]                  = ETIMEDOUT,
  [RTEMS_OBJECT_WAS_DELETED]       = EIO,
  [RTEMS_INVALID_SIZE]             = EIO,
  [RTEMS_INVALID_ADDRESS]          = EIO,
  [RTEMS_INVALID_NUMBER]           = EBADF,
  [RTEMS_NOT_DEFINED]              = EIO,
  [RTEMS_RESOURCE_IN_USE]          = EBUSY,
  [RTEMS_UNSATISFIED]              = ENODEV,
  [RTEMS_INCORRECT_STATE]          = EIO,
  [RTEMS_ALREADY_SUSPENDED]        = EIO,
  [RTEMS_ILLEGAL_ON_SELF]          = EIO,
  [RTEMS_ILLEGAL_ON_REMOTE_OBJECT] = EIO,
  [RTEMS_CALLED_FROM_ISR]          = EIO,
  [RTEMS_INVALID_PRIORITY]         = EIO,
  [RTEMS_INVALID_CLOCK]            = EINVAL,
  [RTEMS_INVALID_NODE]             = EINVAL,
  [RTEMS_NOT_CONFIGURED]           = ENOSYS,
  [RTEMS_NOT_OWNER_OF_RESOURCE]    = EPERM,
  [RTEMS_NOT_IMPLEMENTED]          = ENOSYS,
  [RTEMS_INTERNAL_ERROR]           = EIO,
  [RTEMS_NO_MEMORY]                = ENOMEM,
  [RTEMS_IO_ERROR]                 = EIO,
  [RTEMS_PROXY_BLOCKING]           = EIO
};

int rtems_status_code_to_errno(rtems_status_code sc)
{
  if (sc == RTEMS_SUCCESSFUL) {
    return 0;
  } else {
    int eno = EINVAL;

    if ((unsigned) sc <= RTEMS_STATUS_CODES_LAST) {
      eno = status_code_to_errno [sc];
    }

    errno = eno;

    return -1;
  }
}
