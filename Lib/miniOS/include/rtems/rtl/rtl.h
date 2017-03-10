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
 * @brief RTEMS Run-Time Linker
 *
 * This is the POSIX interface to run-time loading of code into RTEMS.
 */

#if !defined (_RTEMS_RTL_H_)
#define _RTEMS_RTL_H_

#include <link.h>
#include <rtems.h>
#include <rtems/chain.h>

#include <rtems/rtl/rtl-allocator.h>
#include <rtems/rtl/rtl-fwd.h>
#include <rtems/rtl/rtl-obj.h>
#include <rtems/rtl/rtl-obj-cache.h>
#include <rtems/rtl/rtl-obj-comp.h>
#include <rtems/rtl/rtl-unresolved.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup rtems_rtl RTEMS Runtime Link Editor
 *
 * The module implements a runtime link editor with the standard dlopen, and
 * dlclose family of functions.
 *
 * The runtime link editor is different to that found on Unix type systems. The
 * object modules are compiled for PIC or position indepentent code and
 * therefore require relocating when loaded.
 *
 * The object file format is currently ELF and object files can be separate
 * files or in an archive. Object files in an archive are referenced by
 * specifing 'archive:object' format. For example 'libfoo.a:bar.o'.
 */

/**
 * Macros to glue two tokens.
 */
#ifdef __STDC__
#define RTL_XGLUE(a,b) a##b
#else
#define RTL_XGLUE(a,b) a/**/b
#endif

#define RTL_GLUE(a,b) RTL_XGLUE(a,b)

/**
 * The number of buckets in the global symbol table.
 */
#define RTEMS_RTL_SYMS_GLOBAL_BUCKETS (32)

/**
 * The number of relocation record per block in the unresolved table.
 */
#define RTEMS_RTL_UNRESOLVED_BLOCK_SIZE (64)

/**
 * The global debugger interface variable.
 */
extern struct r_debug _rtld_debug;

/**
 * Debugger break function. Call when debugging to have it read the _rtld_debug
 * variable.
 */
extern void _rtld_debug_state (void);

/**
 * The type of constructor/destructor function.
 */
typedef void (*rtems_rtl_cdtor_t)(void);

/**
 * The global RTL data. This structure is allocated on the heap when the first
 * call to the RTL is made and never released.
 *
 * The global symbol table is a hash table held in this structure and the
 * actual symbols are part of the object's structure. If this is a problem we
 * could look at a hash table per object file.
 */
struct rtems_rtl_data_s
{
  rtems_id               lock;           /**< The RTL lock id */
  rtems_rtl_alloc_data_t allocator;      /**< The allocator data. */
  rtems_chain_control    objects;        /**< List if loaded object files. */
  const char*            paths;          /**< Search paths for archives. */
  rtems_rtl_symbols_t    globals;        /**< Global symbol table. */
  rtems_rtl_unresolved_t unresolved;     /**< Unresolved symbols. */
  rtems_rtl_obj_t*       base;           /**< Base object file. */
  rtems_rtl_obj_cache_t  symbols;        /**< Symbols object file cache. */
  rtems_rtl_obj_cache_t  strings;        /**< Strings object file cache. */
  rtems_rtl_obj_cache_t  relocs;         /**< Relocations object file cache. */
  rtems_rtl_obj_comp_t   decomp;         /**< The decompression compressor. */
  int                    last_errno;     /**< Last error number. */
  char                   last_error[64]; /**< Last error string. */
};

/**
 * Get the RTL data with out locking. This call assmes the RTL is locked.
 *
 * @return rtems_rtl_data_t* The RTL data after being locked.
 * @retval NULL The RTL data is not initialised.
 */
rtems_rtl_data_t* rtems_rtl_data (void);

/**
 * Get the RTL global symbol table with out locking. This call assmes the RTL
 * is locked.
 *
 * @return rtems_rtl_symbols_t* The RTL global symbols after being locked.
 * @retval NULL The RTL data is not initialised.
 */
rtems_rtl_symbols_t* rtems_rtl_global_symbols (void);

/**
 * Get the RTL resolved table with out locking. This call assmes the RTL
 * is locked.
 *
 * @return rtems_rtl_unresolv_t* The RTL unresolved symbols and reloc records.
 * @retval NULL The RTL data is not initialised.
 */
rtems_rtl_unresolved_t* rtems_rtl_unresolved (void);

/**
 * Get the RTL symbols, strings, or relocations object file caches. This call
 * assmes the RTL is locked.
 *
 * @param symbols Pointer to the location to set the cache into. Returns NULL
 *                is rtl is not initialised. If NULL is passed in no value set.
 * @param strings Pointer to the location to set the cache into. Returns NULL
 *                is rtl is not initialised. If NULL is passed in no value set.
 * @param relocs Pointer to the location to set the cache into. Returns NULL
 *               is rtl is not initialised. If NULL is passed in no value set.
 */
void rtems_rtl_obj_caches (rtems_rtl_obj_cache_t** symbols,
                           rtems_rtl_obj_cache_t** strings,
                           rtems_rtl_obj_cache_t** relocs);

/**
 * Flush all the object file caches.
 */
void rtems_rtl_obj_caches_flush (void);

/**
 * Get the RTL decompressor setting the cache and the offset in the file the
 * compressed stream starts. This call assmes the RTL is locked.
 *
 * @param decomp Pointer to the location to set the compressor into. Returns
 *               NULL is rtl is not initialised.
 * @param cache The cache to read the file with. Saves needing an extrs buffer.
 * @param offset The offset in the file the compressed stream starts.
 */
void rtems_rtl_obj_comp (rtems_rtl_obj_comp_t** decomp,
                         rtems_rtl_obj_cache_t* cache,
                         int                    fd,
                         int                    compression,
                         off_t                  offset);

/**
 * Lock the Run-time Linker.
 *
 * @return rtems_rtl_data_t* The RTL data after being locked.
 * @retval NULL The RTL data could not be initialised or locked. Typically this
 *              means the lock could not be created.
 */
rtems_rtl_data_t* rtems_rtl_lock (void);

/**
 * Unlock the Run-time Linker.
 *
 * @return True The RTL is unlocked.
 * @return False The RTL could not be unlocked. Not much you can do.
 */
bool rtems_rtl_unlock (void);

/**
 * Check a pointer is a valid object file descriptor returning the pointer as
 * that type.
 *
 * Assumes the RTL has been locked.
 *
 * @param handle Pointer to the object file to be validated.
 * @return rtl_obj* The object file descriptor. NULL is returned if invalid.
 */
rtems_rtl_obj_t* rtems_rtl_check_handle (void* handle);

/**
 * Find the object given a file name.
 *
 * @param name The name of the object file.
 * @retval NULL No object file with that name found.
 * @return rtems_rtl_obj_t* The object file descriptor.
 */
rtems_rtl_obj_t* rtems_rtl_find_obj (const char* name);

/**
 * Load an object file into memory relocating it. It will not be resolved
 * against other symbols in other object files or the base image.
 *
 * The name can be a file name for an object file or it can be encoded to
 * reference an archive of object modules (static library). This encoding is
 * specific to RTEMS and allows dependences to specify an archive without the
 * searching overhead normally incurred by linkers locating object files in an
 * archive. The file name format rules are:
 *
 *  1. Absolute file references a specific object file in the architecture
 *     specific location on the file system.
 *
 *  2. Relative file references an object format file in the search path.
 *
 *  3. Absolute archive and file reference to a specific location in the file
 *     system. The archive and file are encoded as 'archive:file [@@offset]'
 *     where 'archive' is a valid file at the absolute path in the file system,
 *     and 'file' is a contained in the archive, and optionally an offset to
 *     the 'file' in the 'archive'. If no offset is provided the archive is
 *     searched.
 *
 *  4. Relative archive and file in the search path. The encoding is the same
 *     as described in item 3 of this list.
 *
 * Assumes the RTL has been locked.
 *
 * @param name The name of the object file.
 * @param mode The mode of the load as defined by the dlopen call.
 * @return rtl_obj* The object file descriptor. NULL is returned if the load fails.
 */
rtems_rtl_obj_t* rtems_rtl_load_object (const char* name, int mode);

/**
 * Unload an object file. This only happens when the user count is 0.
 *
 * Assumes the RTL has been locked.
 *
 * @param obj The object file descriptor.
 * @retval true The object file has been unloaded.
 * @retval false The object file could not be unloaded.
 */
bool rtems_rtl_unload_object (rtems_rtl_obj_t* obj);

/**
 * Run any constructor functions the object file may contain. This call
 * assumes the linker is unlocked.
 *
 * @param obj The object file.
 */
void rtems_rtl_run_ctors (rtems_rtl_obj_t* obj);

/**
 * Get the last error message clearing it. This operation locks the run time
 * linker and therefore keeps the RTL thread safe but this call is not thread
 * safe is multiple threads are loading object files at the same time. This
 * call is provided to map to the dlopen family of calls.
 *
 * @param message Pointer to a buffer to copy the message into.
 * @param max_message The maximum message that can be copied.
 * @return int The last error number.
 */
int rtems_rtl_get_error (char* message, size_t max_message);

/**
 * Append the path to the search path.
 *
 * @param path The path to append.
 * @retval false The path could not be appended.
 * @retval true The path was appended.
 */
bool rtems_rtl_path_append (const char* path);

/**
 * Prepend the path to the search path.
 *
 * @param path The path to prepend.
 * @retval false The path could not be prepended.
 * @retval true The path was prepended.
 */

bool rtems_rtl_path_prepend (const char* path);

/**
 * Add an exported symbol table to the global symbol table. This call is
 * normally used by an object file when loaded that contains a global symbol
 * table.
 *
 * @param esyms The exported symbol table.
 * @param count The size of the exported symbol table.
 */
void rtems_rtl_base_sym_global_add (const unsigned char* esyms,
                                    unsigned int         count);

/**
 * Return the object file descriptor for the base image. The object file
 * descriptor returned is created when the run time linker is initialised.
 *
 * Assumes the RTL has been locked.
 *
 * @return rtl_obj* The object file descriptor for the base image. NULL is
 *                  returned if the load fails.
 */
rtems_rtl_obj_t* rtems_rtl_baseimage (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
