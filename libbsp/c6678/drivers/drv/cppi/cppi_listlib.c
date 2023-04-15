/** 
 *   @file  cppi_listlib.c
 *
 *   @brief
 *      Implementation of a doubly linked list.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2008-2012, Texas Instruments, Inc.
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

/**********************************************************************
 *************************** LISTLIB FUNCTIONS ************************
 **********************************************************************/
/** 
 *  @b Description
 *  @n  
 *      The function is called to add a node to the list.
 *
 * Cache assumptions: caller has invalidated *ptr_list, ptr_node,
 *                    ptr_list
 *                    Callee will write back any modified links.
 *
 *  @param[in]  ptr_list
 *      This is the list to which the node is to be added. 
 *  @param[in]  ptr_node
 *      This is the node which is to be added.
 *
 *  @retval
 *      Not Applicable
 */
void cppi_list_add (Cppi_ListNode **ptr_list, Cppi_ListNode *ptr_node)
{
    Cppi_ListNode*    ptr_head;

    /* Check if the list is empty ? */
    if (*ptr_list == NULL)
    {
        /* YES the list is empty. Initialize the links */
        ptr_node->p_next = NULL;
        ptr_node->p_prev = NULL;

        /* Initialize the LIST */
        *ptr_list = ptr_node;

        goto end;
    }

    /* No the list was NOT empty. Add the node to the beginning of list. 
     * Get the current head of the list. */
    ptr_head = *ptr_list;

    /* Initialize the new head of the list. */
    ptr_node->p_next  = ptr_head;
    ptr_node->p_prev = NULL;

    /* Update the old head to point to the new head */
    ptr_head->p_prev = ptr_node;

    /* Update the pointer to the head of the list. */
    *ptr_list = ptr_node;

    /* Writeback cache */
    Cppi_osalEndMemAccess (ptr_head, sizeof(Cppi_ListNode));
end:
    Cppi_osalEndMemAccess (ptr_node, sizeof(Cppi_ListNode));
    Cppi_osalEndMemAccess (ptr_list, sizeof(Cppi_ListNode *));
    return;
}

/** 
 *  @b Description
 *  @n  
 *      The function is called to concatenate the src list to the end of the 
 *      destination list.
 *
 * Cache assumptions: caller has invalidated *ptr_dst, *ptr_src,
 *                    ptr_dst, ptr_src, as well as all applicable
 *                    links in the list.
 *                    Callee will write back any modified links.
 *
 *  @param[in]  ptr_dst
 *      This is the head of the destination list.
 *  @param[in]  ptr_src
 *      This is the head of the source list.
 *
 *  @retval
 *      Not Applicable
 */
void cppi_list_cat (Cppi_ListNode **ptr_dst, Cppi_ListNode **ptr_src)
{
    Cppi_ListNode*    ptr_node;
    Cppi_ListNode*    ptr_prev;

    /* Is the source list empty ? */
    if (*ptr_src == NULL)
        return;

    /* Is the destination list empty ? */
    if (*ptr_dst == NULL)
    {
        /* Make the source now as the destination. */
        *ptr_dst = *ptr_src;
        /* Write back ptr_dst */
        Cppi_osalEndMemAccess (ptr_dst, sizeof(Cppi_ListNode));
        return;
    }

    /* Both the lists are not empty. */
    ptr_node = *ptr_dst;
    ptr_prev = NULL;

    /* Reach the end of the list. */
    while (ptr_node != NULL)
    {
        ptr_prev = ptr_node;
        ptr_node = ptr_node->p_next;
    }

    /* Link the last element to the source list. */
    ptr_prev->p_next = *ptr_src;
    /* Write back ptr_prev */
    Cppi_osalEndMemAccess (ptr_prev, sizeof(Cppi_ListNode));
    (*ptr_src)->p_prev = ptr_prev;
    /* Write back *ptr_src */
    Cppi_osalEndMemAccess (*ptr_src, sizeof(Cppi_ListNode));
    return;
}

/**
 *  @b Description
 *  @n  
 *      The function is called to remove the head node from the list. 
 *
 * Cache assumptions: caller has invalidated **ptr_list, ptr_list,
 *                    Callee will write back any modified links.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list from where nodes will be removed.
 *
 *  @retval
 *      Pointer to the head of the list. 
 */
Cppi_ListNode* cppi_list_remove (Cppi_ListNode **ptr_list)
{
    Cppi_ListNode *ptr_head;
    Cppi_ListNode *ptr_node;

    /* Check if the list is empty ? */
    if (*ptr_list == NULL)
        return NULL;

    /* Get the head of the list. */
    ptr_node = *ptr_list;

    /* Move the head to the next element in the list. */
    ptr_head = ptr_node->p_next;
    *ptr_list = ptr_head;

    /* Did we remove the last element?*/
    if (ptr_head != NULL)
    {
        /* No; in that case update the pointers for the new head. */
        ptr_head->p_prev = NULL;
        /* Write back new head */
        Cppi_osalEndMemAccess (ptr_head, sizeof(Cppi_ListNode));
    }

    /* Kill the links before returning the OLD head. */
    ptr_node->p_next = NULL;
    ptr_node->p_prev = NULL;
    /* Write back old head */
    Cppi_osalEndMemAccess (ptr_node, sizeof(Cppi_ListNode));
    /* Write back list ptr */
    Cppi_osalEndMemAccess (ptr_list, sizeof(Cppi_ListNode *));

    return ptr_node;
}

/**
 *  @b Description
 *  @n  
 *      The function is called to remove the specified node from the list. 
 *
 * Cache assumptions: caller has invalidated **ptr_list, ptr_remove, as well
 *                    as previous and next nodes (or whole list)
 *                    Callee will write back any modified links.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list from where node will be removed.
 *  @param[in]  ptr_remove
 *      This is the node which is to be removed.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   -1
 */
int cppi_list_remove_node (Cppi_ListNode **ptr_list, Cppi_ListNode *ptr_remove)
{
    Cppi_ListNode*    ptr_next;
    Cppi_ListNode*    ptr_prev;

    /* Are there any nodes in the list? */
    if (*ptr_list == NULL)
        return -1;

    /* Are we removing the head? */
    if (ptr_remove == *ptr_list)
    {
        /* Use the other API to acheive the needful. */
        cppi_list_remove (ptr_list);
        return 0;
    }

    /* OK; we are trying to remove a non head element; so lets get the
     * previous and next pointer of the elements that needs to be removed. */
    ptr_prev = ptr_remove->p_prev;
    ptr_next = ptr_remove->p_next;

    /* Kill the Links for element that is being removed. */
    ptr_remove->p_prev = NULL;
    ptr_remove->p_next = NULL;
    /* Write back removed node */
    Cppi_osalEndMemAccess (ptr_remove, sizeof(Cppi_ListNode));

    /* Are we removing the last element */
    if (ptr_next == NULL)
    {
        /* The last element points to nothing. */
        ptr_prev->p_next = NULL;
        /* Write last element */
        Cppi_osalEndMemAccess (ptr_prev, sizeof(Cppi_ListNode));
        return 0;
    }

    /* We are trying to remove an element in the middle of the list. */
    ptr_prev->p_next = ptr_next;
    ptr_next->p_prev = ptr_prev;
    /* write back changed nodes */
    Cppi_osalEndMemAccess (ptr_prev, sizeof(Cppi_ListNode));
    Cppi_osalEndMemAccess (ptr_next, sizeof(Cppi_ListNode));

    /* Successful. */
    return 0;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the head of the specific list
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the list. 
 *
 *  @retval
 *      Head of the list (could be NULL if the list is empty)
 */
Cppi_ListNode* cppi_list_get_head (Cppi_ListNode **ptr_list)
{
    return *ptr_list;
}

/**
 *  @b Description
 *  @n  
 *      The function is used to get the next element in the list.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the node in the list. 
 *
 *  @retval
 *      Next element in the list. (could be NULL if this is the last element)
 */
Cppi_ListNode* cppi_list_get_next (Cppi_ListNode *ptr_list)
{    
    return ptr_list->p_next;
}

/**
 *  @b Description
 *  @n  
 *      Calls Cppi_osalBeginMemAccess () for each element in a list.
 *      This must be called before anything in the same heap as the list is
 *      dirtied, otherwise the dirty data (from items outside this list)
 *      will be discarded.
 *
 *  @param[in]  ptr_list
 *      This is the pointer to the head node in the list. 
 *
 *  @retval
 *      nothing
 */
void cppi_list_cache_invalidate (Cppi_ListNode *ptr_list)
{    
    Cppi_ListNode *ptr_node;

    ptr_node = ptr_list;
    while (ptr_node)
    {
        Cppi_osalBeginMemAccess (ptr_node, sizeof(Cppi_ListNode));
        ptr_node = ptr_node->p_next;
    }
}

/* Nothing past this point */

