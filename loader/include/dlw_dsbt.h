/*
 * dlw_dsbt.h
 *
 * Define internal data structures used by RIDL client side for
 * Dynamic Static Base Table (DSBT) support.
 *
 */

#ifndef DLW_DSBT_H
#define DLW_DSBT_H

#include "Queue.h"
#include "dload_api.h"

/*---------------------------------------------------------------------------*/
/* DSBT_Index_Request - Representation of a request for a DSBT index on      */
/*    behalf of an executable or library. This data structure will reserve   */
/*    space for the specified module in the client's model of the master     */
/*    DSBT.                                                                  */
/*---------------------------------------------------------------------------*/
typedef struct {
	char *name;
	int32_t file_handle;
	uint32_t dsbt_size;
	TARGET_ADDRESS dsbt_base;
	TARGET_ADDRESS static_base;
	int32_t requested_index;
	int32_t assigned_index;
} DSBT_Index_Request;

/*---------------------------------------------------------------------------*/
/* DSBT_Entry - Representation of one slot in the client's model of the      */
/*    master DSBT. The model is built up when DSBT indices are assigned.     */
/*    The content of the master DSBT is propagated to each module who uses   */
/*    the DSBT model.                                                        */
/*---------------------------------------------------------------------------*/
typedef struct {
	DSBT_Index_Request *index_request;
} DSBT_Entry;

/*---------------------------------------------------------------------------*/
/* DSBT_index_request_queue - This is a holding area for DSBT index requests */
/*    while allocation and relocation of symbols is in progress for the top- */
/*    level module and all of its dependents. Items will be pulled off the   */
/*    queue when we are ready to make actual DSBT index assignmments.        */
/*---------------------------------------------------------------------------*/
TYPE_QUEUE_DEFINITION(DSBT_Index_Request*, dsbt_index_request_ptr)
extern dsbt_index_request_ptr_Queue DSBT_index_request_queue;

/*---------------------------------------------------------------------------*/
/* DSBT_master - client's model of DSBT master. Vector of DSBT_Entry ptrs.   */
/*---------------------------------------------------------------------------*/
extern Array_List DSBT_master;

/*---------------------------------------------------------------------------*/
/* DSBT_release_entry()                                                      */
/*                                                                           */
/*    Release an entry in the client's model of the master DSBT. This        */
/*    happens when a module is unloaded from the target. The slot will       */
/*    remain a part of the DSBT, but it will become available to other       */
/*    objects that are subsequently loaded.                                  */
/*---------------------------------------------------------------------------*/
extern void DSBT_release_entry(int32_t file_handle);

#endif
