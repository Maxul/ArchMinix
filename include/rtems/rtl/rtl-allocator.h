/*
 *  COPYRIGHT (c) 2012 Chris Johns <chrisj@rtems.org>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */
/**
 * @file
 *
 * @ingroup rtems_rtl
 *
 * @brief RTEMS Run-Time Linker Allocator
 */

#if !defined (_RTEMS_RTL_ALLOCATOR_H_)
#define _RTEMS_RTL_ALLOCATOR_H_

#include <stdbool.h>

#include "rtl-indirect-ptr.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Define the types of allocation the loader requires.
 *
 * @note It is best to use the object tag for general memory allocation and to
 *       leave the tags with specific access properties to the module data
 */
enum rtems_rtl_alloc_tags_e {
  RTEMS_RTL_ALLOC_OBJECT,     /**< A generic memory object. */
  RTEMS_RTL_ALLOC_SYMBOL,     /**< Memory used for symbols. */
  RTEMS_RTL_ALLOC_EXTERNAL,   /**< Memory used for external symbols. */
  RTEMS_RTL_ALLOC_READ,       /**< The memory is read only. */
  RTEMS_RTL_ALLOC_READ_WRITE, /**< The memory is read and write. */
  RTEMS_RTL_ALLOC_READ_EXEC   /**< The memory is read and executable. */
};

/**
 * The allocator tag type.
 */
typedef enum rtems_rtl_alloc_tags_e rtems_rtl_alloc_tag_t;

/**
 * The number of tags.
 */
#define RTEMS_RTL_ALLOC_TAGS ((size_t) (RTEMS_RTL_ALLOC_READ_EXEC + 1))

/**
 * Allocator handler handles all RTL allocations. It can be hooked and
 * overridded for customised allocation schemes or memory maps.
 *
 * @param allocation If true the request is to allocate memory else free.
 * @param tag The type of allocation request.
 * @param address Pointer to the memory address. If an allocation the value is
 *                unspecific on entry and the allocated address or NULL on
 *                exit. The NULL value means the allocation failed. If a delete
 *                or free request the memory address is the block to free. A
 *                free request of NULL is silently ignored.
 * @param size The size of the allocation if an allocation request and
 *             not used if deleting or freeing a previous allocation.
 */
typedef void (*rtems_rtl_allocator_t)(bool                  allocate,
                                      rtems_rtl_alloc_tag_t tag,
                                      void**                address,
                                      size_t                size);

/**
 * The allocator data.
 */
struct rtems_rtl_alloc_data_s {
  /**< The memory allocator handler. */
  rtems_rtl_allocator_t allocator;
  /**< The indirect pointer chains. */
  rtems_chain_control indirects[RTEMS_RTL_ALLOC_TAGS];
};

typedef struct rtems_rtl_alloc_data_s rtems_rtl_alloc_data_t;

/**
 * Initialise the allocate data.
 *
 * @param data The data to initialise.
 */
void rtems_rtl_alloc_initialise (rtems_rtl_alloc_data_t* data);

/**
 * The Runtime Loader allocator new allocates new memory and optionally clear
 * the memory if requested.
 *
 * @param tag The type of allocation request.
 * @param size The size of the allocation.
 * @param zero If true the memory is cleared.
 * @return void* The memory address or NULL is not memory available.
 */
void* rtems_rtl_alloc_new (rtems_rtl_alloc_tag_t tag, size_t size, bool zero);

/**
 * The Runtime Loader allocator delete deletes allocated memory.
 *
 * @param tag The type of allocation request.
 * @param address The memory address to delete. A NULL is ignored.
 */
void rtems_rtl_alloc_del (rtems_rtl_alloc_tag_t tag, void* address);

/**
 * Hook the Runtime Loader allocatior. A handler can call the previous handler
 * in the chain to use it for specific tags. The default handler uses the
 * system heap. Do not unhook your handler if memory it allocates has not been
 * returned.
 *
 * @param handler The handler to use as the allocator.
 * @return rtems_rtl_alloc_handler_t The previous handler.
 */
rtems_rtl_allocator_t rtems_rtl_alloc_hook (rtems_rtl_allocator_t handler);

/**
 * Allocate memory to an indirect handle.
 *
 * @param tag The type of allocation request.
 * @param handle The handle to allocate the memory to.
 * @param size The size of the allocation.
 */
void rtems_rtl_alloc_indirect_new (rtems_rtl_alloc_tag_t tag,
                                   rtems_rtl_ptr_t*      handle,
                                   size_t                size);

/**
 * Free memory from an indirect handle.
 *
 * @param tag The type of allocation request.
 * @param handle The handle to free the memory from.
 */
void rtems_rtl_alloc_indirect_del (rtems_rtl_alloc_tag_t tag,
                                   rtems_rtl_ptr_t*      handle);

/**
 * Allocate the memory for a module given the size of the text, const, data and
 * bss sections. If any part of the allocation fails the no memory is
 * allocated.
 *
 * @param text_base Pointer to the text base pointer.
 * @param text_size The size of the read/exec section.
 * @param const_base Pointer to the const base pointer.
 * @param const_size The size of the read only section.
 * @param data_base Pointer to the data base pointer.
 * @param data_size The size of the read/write secton.
 * @param bss_base Pointer to the bss base pointer.
 * @param bss_size The size of the read/write.
 * @retval true The memory has been allocated.
 * @retval false The allocation of memory has failed.
 */
bool rtems_rtl_alloc_module_new (void** text_base, size_t text_size,
                                 void** const_base, size_t const_size,
                                 void** data_base, size_t data_size,
                                 void** bss_base, size_t bss_size);

/**
 * Free the memory allocated to a module.
 *
 * @param text_base Pointer to the text base pointer.
 * @param const_base Pointer to the const base pointer.
 * @param data_base Pointer to the data base pointer.
 * @param bss_base Pointer to the bss base pointer.
 */
void rtems_rtl_alloc_module_del (void** text_base, void** const_base,
                                 void** data_base, void** bss_base);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
