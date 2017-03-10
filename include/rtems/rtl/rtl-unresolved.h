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
 * @brief RTEMS Run-Time Linker Object File Unresolved Relocations Table.
 *
 * The unresolved relocation table holds relocations in a loaded object file
 * which reference unresolved external symbols. The support is needed to allow
 * dependent object files to load. In the case of dependent object files one
 * will have unresolved externals until the dependent object file is also
 * loaded. There is no load order that resolves this.
 *
 * The unresolved relocation table is a single table used by all object files
 * with unresolved symbols. It made of blocks linked together where blocks are
 * allocated as requiered. The table is always maintained compacted. That is as
 * relocations are resolved and removed the table is compacted. The only
 * pointer in the table is the object file poniter. This is used to identify
 * which object the relocation belongs to. There are no linking or back
 * pointers in the unresolved relocations table. The table is scanned for each
 * object file's relocations. This is not fast but the table should be small
 * and if it happens to grow large you have other more pressing issues to
 * resolve in your application.
 *
 * The table holds two (2) types of records:
 *
 *  # Symbol name strings.
 *  # Relocations.
 *
 * The symbol name a relocation references is held in a specific symbol name
 * string record in the table the relocation record references. The record
 * counts the number of references and the string is removed from the table
 * when the reference count reaches 0. There can be many relocations
 * referencing the symbol. The strings are referenced by a single 16bit
 * unsigned integer which is the count of the string in the table.
 *
 * The section the relocation is for in the object is the section number. The
 * relocation data is series of machine word sized fields:
 *
 * # Offset in the section.
 * # Relocation info (format specific)
 * # Additional format specific data.
 */

#if !defined (_RTEMS_RTL_UNRESOLVED_H_)
#define _RTEMS_RTL_UNRESOLVED_H_

#include <rtems.h>
#include "rtl-obj-fwd.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Hack to work around machine size. This needs to be cleaned up
 * to better support 64bit targets.
 */
typedef uint32_t rtems_rtl_word_t;

/**
 * The types of records in the blocks.
 */
typedef enum rtems_rtl_unresolved_rtype_e
{
  rtems_rtl_unresolved_empty = 0,  /**< The records is empty. Must always be 0 */
  rtems_rtl_unresolved_name = 1,   /**< The record is a name. */
  rtems_rtl_unresolved_reloc = 2   /**< The record is a relocation record. */
} rtems_rtl_unresolved_rtype_t;

/**
 * Unresolved externals symbol names. The names are reference counted and
 * separate from the relocation records because a number of records could
 * reference the same symbol name.
 */
typedef struct rtems_rtl_unresolv_name_s
{
  uint16_t refs;       /**< The number of references to this name. */
  uint16_t length;     /**< The length of this name. */
  const char name[12]; /**< The symbol name. */
} rtems_rtl_unresolv_name_t;

/**
 * Unresolved externals symbols require the relocation records to be held
 * and references.
 */
typedef struct rtems_rtl_unresolv_reloc_s
{
  rtems_rtl_obj_t* obj;     /**< The relocation's object file. */
  uint16_t         flags;   /**< Format specific flags. */
  uint16_t         name;    /**< The symbol's name. */
  uint16_t         sect;    /**< The target section. */
  rtems_rtl_word_t rel[3];  /**< Relocation record. */
} rtems_rtl_unresolv_reloc_t;

/**
 * Unresolved externals records.
 */
typedef struct rtems_rtl_unresolv_rec_s
{
  rtems_rtl_unresolved_rtype_t type;
  union
  {
    rtems_rtl_unresolv_name_t name;    /**< The name, or */
    rtems_rtl_unresolv_reloc_t reloc;  /**< the relocation record. */
  } rec;
} rtems_rtl_unresolv_rec_t;

/**
 * Unresolved blocks.
 */
typedef struct rtems_rtl_unresolv_block_s
{
  rtems_chain_node         link; /**< Blocks are chained. */
  uint32_t                 recs; /**< The number of records in the block. */
  rtems_rtl_unresolv_rec_t rec;  /**< The records. More follow. */
} rtems_rtl_unresolv_block_t;

/**
 * Unresolved table holds the names and relocations.
 */
typedef struct rtems_rtl_unresolved_s
{
  uint32_t marker;
  size_t              block_recs; /**< The records per blocks allocated. */
  rtems_chain_control blocks;     /**< List of blocks. */
} rtems_rtl_unresolved_t;

/**
 * The iterator function used to iterate over the unresolved table.
 *
 * @param rec The current iterator.
 * @param data The user data.
 * @retval true The iterator has finished.
 * @retval false The iterator has not finished. Keep iterating.
 */
typedef bool rtems_rtl_unresolved_iterator_t (rtems_rtl_unresolv_rec_t* rec,
                                              void*                     data);

/**
 * Open an unresolved relocation table.
 *
 * @param unresolv The unresolved table to open.
 * @param block_records The number of records per block allocated.
 * @retval true The table is open.
 * @retval false The unresolved relocation table could not created. The RTL
 *               error has the error.
 */
bool rtems_rtl_unresolved_table_open (rtems_rtl_unresolved_t* unresolved,
                                      size_t                  block_records);

/**
 * Close the table and erase the blocks.
 *
 * @param unreolved Close the unresolved table.
 */
void rtems_rtl_unresolved_table_close (rtems_rtl_unresolved_t* unresolved);

/**
 * Iterate over the table of unresolved entries.
 */
bool rtems_rtl_unresolved_interate (rtems_rtl_unresolved_iterator_t iterator,
                                    void*                           data);

/**
 * Add a relocation to the list of unresolved relocations.
 *
 * @param unresolved The unresolved symbol table.
 * @param obj The object table the symbols are for.
 * @param flags Format specific flags.
 * @param name The symbol name the relocation references.
 * @param sect The target section number the relocation references.
 * @param rel The format specific relocation data.
 * @retval true The relocation has been added.
 * @retval false The relocation could not be added.
 */
bool rtems_rtl_unresolved_add (rtems_rtl_obj_t*        obj,
                               const uint16_t          flags,
                               const char*             name,
                               const uint16_t          sect,
                               const rtems_rtl_word_t* rel);

/**
 * Resolve the unresolved symbols.
 */
void rtems_rtl_unresolved_resolve (void);

/**
 * Remove a relocation from the list of unresolved relocations.
 *
 * @param unresolved The unresolved symbol table.
 * @param obj The object table the symbols are for.
 * @param esyms The exported symbol table.
 * @param size The size of the table in bytes.
 */
bool rtems_rtl_unresolved_remove (rtems_rtl_obj_t*        obj,
                                  const char*             name,
                                  const uint16_t          sect,
                                  const rtems_rtl_word_t* rel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
