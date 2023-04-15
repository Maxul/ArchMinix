/**
 * @file
 *
 * @brief Initialization Manager
 *
 * @ingroup ClassicRTEMS
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

/*
 *  SCORE_INIT and SAPI_INIT are defined so all of the super core and
 *  super API data will be included in this object file.
 */

#define SAPI_INIT
#define SCORE_INIT

#include <rtems/system.h>
#include <rtems/config.h>
#include <rtems/debug.h>
#include <rtems/extensionimpl.h>
#include <rtems/init.h>
#include <rtems/score/sysstate.h>

#include <rtems/score/apiext.h>
#include <rtems/score/apimutex.h>
#include <rtems/score/copyrt.h>
#include <rtems/score/cpusetimpl.h>
#include <rtems/score/heap.h>
#include <rtems/score/interr.h>
#include <rtems/score/isr.h>
#include <rtems/score/priority.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/smpimpl.h>
#include <rtems/score/timecounter.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/todimpl.h>
#include <rtems/score/userextimpl.h>
#include <rtems/score/watchdogimpl.h>
#include <rtems/score/wkspace.h>

#include <rtems/sptables.h>


#include <rtems/rtems/rtemsapi.h>
#include <rtems/posix/posixapi.h>

#ifdef RTEMS_DRVMGR_STARTUP
  #include <drvmgr/drvmgr.h>
#endif

Objects_Information *_Internal_Objects[ OBJECTS_INTERNAL_CLASSES_LAST + 1 ];

void rtems_initialize_data_structures(void)
{
  /*
   *  Dispatching and interrupts are disabled until the end of the
   *  initialization sequence.  This prevents an inadvertent context
   *  switch before the executive is initialized.
   *
   *  WARNING: Interrupts should have been disabled by the BSP and
   *           are disabled by boot_card().
   */

  #if defined(RTEMS_MULTIPROCESSING)
    /*
     *  Initialize the system state based on whether this is an MP system.
     *  In an MP configuration, internally we view single processor
     *  systems as a very restricted multiprocessor system.
     */
    _Configuration_MP_table = rtems_configuration_get_user_multiprocessing_table();

    if ( _Configuration_MP_table == NULL ) {
      _Configuration_MP_table =
	(void *)&_Initialization_Default_multiprocessing_table;
      _System_state_Handler_initialization( FALSE );
    } else {
      _System_state_Handler_initialization( TRUE );
    }
  #else
    _System_state_Handler_initialization( FALSE );
  #endif

  /*
   * Initialize any target architecture specific support as early as possible
   */
  _CPU_Initialize();

  #if defined(RTEMS_MULTIPROCESSING)
    _Objects_MP_Handler_early_initialization();
  #endif

  /*
   *  Do this as early as possible to ensure no debugging output
   *  is even attempted to be printed.
   */
  _Debug_Manager_initialization();

  _API_extensions_Initialization();

  _Thread_Dispatch_initialization();

  _User_extensions_Handler_initialization();
  _ISR_Handler_initialization();

  /*
   * Initialize the internal support API and allocator Mutex
   */
  _Objects_Information_table[OBJECTS_INTERNAL_API] = _Internal_Objects;

  _API_Mutex_Initialization( 2 );
  _API_Mutex_Allocate( &_RTEMS_Allocator_Mutex );
  _API_Mutex_Allocate( &_Once_Mutex );

  _Watchdog_Handler_initialization();
  _TOD_Handler_initialization();

  _Thread_Handler_initialization();

  _Scheduler_Handler_initialization();

  #if defined(RTEMS_MULTIPROCESSING)
    _Objects_MP_Handler_initialization();
    _MPCI_Handler_initialization( RTEMS_TIMEOUT );
  #endif

  _SMP_Handler_initialize();

  _CPU_set_Handler_initialization();

/* MANAGERS */

  _RTEMS_API_Initialize();

  _Extension_Manager_initialization();

  _POSIX_API_Initialize();

  _System_state_Set( SYSTEM_STATE_BEFORE_MULTITASKING );

  /*
   *  No threads should be created before this point!!!
   *  _Thread_Executing and _Thread_Heir are not set.
   *
   *  At this point all API extensions are in place.  After the call to
   *  _Thread_Create_idle() _Thread_Executing and _Thread_Heir will be set.
   */
  _Thread_Create_idle();

  /*
   *  Scheduling can properly occur now as long as we avoid dispatching.
   */
}

void rtems_initialize_before_drivers(void)
{
  #ifdef RTEMS_DRVMGR_STARTUP
  _DRV_Manager_initialization();
  #endif

  #if defined(RTEMS_MULTIPROCESSING)
    _MPCI_Create_server();
  #endif

  #if defined(FUNCTIONALITY_NOT_CURRENTLY_USED_BY_ANY_API)
    /*
     *  Run the API and BSPs predriver hook.
     */
    _API_extensions_Run_predriver();
  #endif
}

void rtems_initialize_device_drivers(void)
{
  /*
   *  Initialize all the device drivers and initialize the MPCI layer.
   *
   *  NOTE:  The MPCI may be build upon a device driver.
   */

  #ifdef RTEMS_DRVMGR_STARTUP
  /* BSPs has already registered their "root bus" driver in the
   * bsp_predriver hook or so.
   *
   * Init Drivers to Level 1, constraints:
   *   - Interrupts and system clock timer does not work.
   *   - malloc() work, however other memory services may not
   *     have been initialized yet.
   *   - initializes most basic stuff
   *
   * Typical setup in Level 1:
   *   - Find most devices in system, do PCI scan and configuration.
   *   - Reset hardware if needed.
   *   - Install IRQ driver
   *   - Install Timer driver
   *   - Install console driver and debug printk()
   *   - Install extra memory.
   */
  _DRV_Manager_init_level(1);
  bsp_driver_level_hook(1);
  #endif

  /* Initialize I/O drivers. 
   *
   * Driver Manager note:
   * All drivers may not be registered yet. Drivers will dynamically
   * be initialized when registered in level 2,3 and 4.
   */
  _IO_Initialize_all_drivers();

  #ifdef RTEMS_DRVMGR_STARTUP
  /* Init Drivers to Level 2, constraints:
   *  - Interrupts can be registered and enabled.
   *  - System Clock is running
   *  - Console may be used.
   *
   * This is typically where drivers are initialized
   * for the first time.
   */
  _DRV_Manager_init_level(2);
  bsp_driver_level_hook(2);

  /* Init Drivers to Level 3 
   * 
   * This is typically where normal drivers are initialized
   * for the second time, they may depend on other drivers
   * API inited in level 2
   */
  _DRV_Manager_init_level(3);
  bsp_driver_level_hook(3);

  /* Init Drivers to Level 4,
   * Init drivers that depend on services initialized in Level 3
   */
  _DRV_Manager_init_level(4);
  bsp_driver_level_hook(4);
  #endif

  #if defined(RTEMS_MULTIPROCESSING)
    if ( _System_state_Is_multiprocessing ) {
      _MPCI_Initialization();
      _MPCI_Internal_packets_Send_process_packet(
	MPCI_PACKETS_SYSTEM_VERIFY
      );
    }
  #endif

  /*
   *  Run the APIs and BSPs postdriver hooks.
   *
   *  The API extensions are supposed to create user initialization tasks.
   */
  _API_extensions_Run_postdriver();
}

void rtems_initialize_start_multitasking(void)
{
  _System_state_Set( SYSTEM_STATE_UP );

  _SMP_Request_start_multitasking();

  _Thread_Start_multitasking();

  /*******************************************************************
   *******************************************************************
   *******************************************************************
   ******                 APPLICATION RUNS HERE                 ******
   ******              THE FUNCTION NEVER RETURNS               ******
   *******************************************************************
   *******************************************************************
   *******************************************************************/
}
