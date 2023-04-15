/*
 * symtab.h
 *
 * Specification of functions used by the core loader to create, maintain,
 * and destroy internal symbol tables.
 *
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include "ArrayList.h"
#include "dload.h"

/*****************************************************************************/
/* This is the top-level application file handle.  It should only be needed  */
/* under the Linux and DSBT models.                                          */
/*****************************************************************************/
extern int32_t DLIMP_application_handle;

/*---------------------------------------------------------------------------*/
/* Core Loader Symbol Table Management Functions                             */
/*---------------------------------------------------------------------------*/
BOOL DLSYM_canonical_lookup(int32_t sym_index, DLIMP_Dynamic_Module *dyn_module,
		Elf32_Addr *sym_value);

BOOL DLSYM_global_lookup(const char *sym_name, DLIMP_Loaded_Module *pentry,
		Elf32_Addr *sym_value);

BOOL DLSYM_lookup_local_symtab(const char *sym_name, struct Elf32_Sym *symtab,
		Elf32_Word symnum, Elf32_Addr *sym_value);

void DLSYM_copy_globals(DLIMP_Dynamic_Module *dyn_module);

#endif
