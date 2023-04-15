/** 
 *   @file  cppi_listlib.c
 *
 *   @brief
 *      Implementation of a doubly linked list.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2012, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <ti/drv/cppi/cppi_types.h>
#include <ti/drv/cppi/cppi_osal.h>
#include <ti/drv/cppi/include/cppi_listlib.h>
#include <ti/drv/cppi/include/cppi_heap.h>


/**********************************************************************
 *************************** heap FUNCTIONS ************************
 **********************************************************************/
/** 
 *  @b Description
 *  @n  
 *      Prepares the cache before accessing heap.  This must be called
 *      before anything in the heap is dirtied, thus it will usually
 *      be called at the beginning of a function that may use the 
 *      heap functions.
 *
 * Cache assumptions: Invalidates desc
 *                    Invalidates head of blockFreeList
 *                    Invalidates head of allocatedBlockList
 *
 *  @param[in]  desc
 *      Heap descriptor which includes free list and size
 *      parameters
 */
void cppi_internal_heap_cache_begin (Cppi_HeapDesc *desc)
{
    Cppi_osalBeginMemAccess (desc, sizeof(*desc));
    if (desc->blockFreeList)
    {
        Cppi_osalBeginMemAccess (desc->blockFreeList, CPPI_BLOCK_CHUNK_SIZE);
    }
    if (desc->allocatedBlockList)
    {
        Cppi_osalBeginMemAccess (desc->allocatedBlockList, CPPI_BLOCK_CHUNK_SIZE);
    }
}

/** 
 *  @b Description
 *  @n  
 *      This function allocates from internal heap if available.
 *      If not available, allocates using Cppi_osalMalloc.  If
 *      Cppi_osalMalloc fails, or the size is > CPPI_BLOCK_CHUNK_SIZE,
 *      then this function CAN return NULL.
 *
 * Cache assumptions: Caller manages cache for desc.  This can be
 *                    done by calling cppi_internal_heap_cache_begin ()
 *                    BEFORE any items from the heap get dirtied.
 *
 *                    Callee writes back any changes.
 *
 *                    Function is called inside a semaphore protecting
 *                    heap from multicore
 *
 *  @param[in]  desc
 *      Heap descriptor which includes free list and size
 *      parameters
 *  @param[in]  size
 *      Allocated size.  
 *
 *  @retval
 *      New pointer or NULL on error.
 */
void *cppi_internal_heap_malloc (Cppi_HeapDesc *desc, uint32_t size)
{
    void *ptr;

    /* Make sure requested size fits in chunk */
    if (size > CPPI_BLOCK_CHUNK_SIZE)
    {
        return NULL;
    }

    /* First try to allocate from internal free list */
    ptr = cppi_list_remove (&desc->blockFreeList);

    /* If this fails, allocate and divide a new block */
    if (! ptr && desc->blockSize) 
    {
        cppi_internal_heap_add (desc, NULL, desc->blockSize);
        ptr = cppi_list_remove (&desc->blockFreeList);
    }

    return ptr; /* can be NULL if cppi_internal_add_heap() didn't work */
}

/** 
 *  @b Description
 *  @n  
 *      This function returns ptr to internal heap free list.
 *      It will never return memory through Cppi_osalFree.  Memory
 *      is only returned on cppi_exit().
 *
 * Cache assumptions: Caller manages cache for desc.  This can be
 *                    done by calling cppi_internal_heap_cache_begin ()
 *                    BEFORE any items from the heap get dirtied.
 *
 *                    Callee writes back any changes.
 *
 *                    Function is called inside a semaphore protecting
 *                    heap from multicore
 *
 *  @param[in]  desc
 *      Heap descriptor which includes free list and size
 *      parameters
 *  @param[in]  ptr
 *      Previously allocated pointer
 *
 *  @retval
 *      Not Applicable
 */
void cppi_internal_heap_free (Cppi_HeapDesc *desc, void *ptr)
{
    /* Return ptr to free list */
    cppi_list_add (&desc->blockFreeList, ptr);
}

/** 
 *  @b Description
 *  @n  
 *      This function returns all memory allocated from Cppi_osalMalloc.
 *      It should only used as part of cppi_exit().
 *
 * Cache assumptions: Caller manages cache for desc
 *
 *                    Callee manages cache for allocatedBlockList
 *
 *                    Nothing from internal heap is dirty
 *                    (caller wrote back dirty lines)
 *
 *                    Function is called inside a semaphore protecting
 *                    heap from multicore
 *
 *  @param[in]  desc
 *      Heap descriptor which includes free list and size
 *      parameters
 *
 *  @retval
 *      Not Applicable
 */
void cppi_internal_heap_release (Cppi_HeapDesc *desc)
{
    Cppi_BlockTracker *tracker;

    /* Return all allocated blocks to Cppi_osalFree on close */
    while ( (tracker = (Cppi_BlockTracker *)cppi_list_remove ((Cppi_ListNode **)&desc->allocatedBlockList)) )
    {
        uint32_t block;
        Cppi_osalBeginMemAccess (tracker, sizeof(*tracker));

        for (block = 0; block < (CPPI_BLOCK_TRACKER_MAX_BLOCKS - tracker->numBlocksRem); block++)
        {
            Cppi_osalFree (tracker->blocks[block], desc->blockSize);
        }
    }

    desc->blockFreeList = NULL;
    desc->allocatedBlockList = NULL;
    Cppi_osalEndMemAccess (desc, sizeof(*desc));
}

/** 
 *  @b Description
 *  @n  
 *      This function adds a new block to the internal heap.
 *
 *      The new chunk of memory is aligned to desc->alignPow2, which must
 *      be large enough so no partial cache lines will be on either side
 *      of the block.  
 *
 * Cache assumptions: Caller manages cache for desc.  This can be
 *                    done by calling cppi_internal_heap_cache_begin ()
 *                    BEFORE any items from the heap get dirtied.
 *
 *                    Callee writes back any changes.
 *
 *                    Function is called inside a semaphore protecting
 *                    heap from multicore
 *
 Qmss_getQosSchedStats*
 *  @param[in]  desc
 *      Heap descriptor which includes free list and size
 *      parameters
 *  @param[in]  heap
 *      Pre-allocated heap.  If NULL, will allocate a new block
 *      of heapSize from Cppi_osalMalloc().  This is normally used
 *      with a static heap else NULL.
 *  @param[in]  heapSize
 *      Size of heap, or size to allocate from Cppi_osalMalloc().
 *
 *  @retval
 *      Not Applicable
 */
void cppi_internal_heap_add (Cppi_HeapDesc *desc, void *heap, uint32_t heapSize)
{
    int heapWasMalloced = 0;
    uint32_t alignMask = (1 << desc->alignPow2) - 1;

    if (! heap)
    {
        /* use malloc to try to get a block */
        heap            = Cppi_osalMalloc (heapSize);
        heapWasMalloced = 1;
    }
    if (heap)
    {
        /* Find low address which is aligned */
        uint32_t pad              = ((uint32_t)heap) & alignMask;
        uint8_t  *alignedBase     = (uint8_t *)heap;
        uint32_t alignedBlockSize = heapSize;
        uint32_t offset;

        if (pad) {
            /* Apply the alignment only if required */
            pad              = (alignMask + 1 - pad);
            alignedBase      += pad;
            alignedBlockSize -= pad;
        }

        /* Now discard the unaligned remainder at the end */
        alignedBlockSize &= ~alignMask;

        /* Invalidate the new heap */
        /* This is "safe" because the heap is on whole cache lines no false sharing */
        Cppi_osalBeginMemAccess (alignedBase, alignedBlockSize);
        memset (alignedBase, 0, alignedBlockSize);

        /* chunk up into CPPI_BLOCK_CHUNK_SIZE */
        for (offset = 0; 
             (offset + CPPI_BLOCK_CHUNK_SIZE) < alignedBlockSize; 
             offset += CPPI_BLOCK_CHUNK_SIZE)
        {
            cppi_list_add (&desc->blockFreeList, (Cppi_ListNode *)(alignedBase + offset));
        }

        if (heapWasMalloced)
        {
            if (! offset)
            {
                /* Block was too small after aligning.  Return it and let the malloc fail */
                Cppi_osalFree (heap, heapSize);
            }
            else
            {
                /* Keep the pointer that was returned by Cppi_osalMalloc, so it can
                 * be freed when cppi_exit() is called */
                /* Cache was taken care of by precondition */
                Cppi_BlockTracker *tracker = (Cppi_BlockTracker *)cppi_list_get_head ((Cppi_ListNode **)&desc->allocatedBlockList);
                if ((! tracker) || (! tracker->numBlocksRem))
                {
                    /* Need to allocate a new tracker */
                    tracker = cppi_internal_heap_malloc (desc, sizeof(*tracker));
                    /* Do NOT need to invalidate because this came from
                     * new heap added above */
                    if (tracker)
                    {
                        memset (tracker, 0, sizeof (*tracker));
                        tracker->numBlocksRem = CPPI_BLOCK_TRACKER_MAX_BLOCKS;
                        /* tracker is written back below */
                        cppi_list_add ((Cppi_ListNode **)&desc->allocatedBlockList, (Cppi_ListNode *)tracker);
                    }
                    else
                    {
                        /* Should never get here */
                        for (;;);
                    }
                }

                tracker->blocks[CPPI_BLOCK_TRACKER_MAX_BLOCKS - tracker->numBlocksRem] = heap;
                tracker->numBlocksRem --;
                /* Writeback block record */
                Cppi_osalEndMemAccess (tracker, sizeof (*tracker));
            }
        }
    }
}

/* Nothing past this point */

