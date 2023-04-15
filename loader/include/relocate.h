/*
 * relocate.h
 *
 * Declare names and IDs of all C6x-specific relocation types supported
 * in the dynamic loader.
 *
 */

#ifndef RELOCATE_H
#define RELOCATE_H

#include <inttypes.h>
#include "elf32.h"
#include "dload.h"
#include "dload_api.h"

/*---------------------------------------------------------------------------*/
/* Declare some globals that are used for internal debugging and profiling.  */
/*---------------------------------------------------------------------------*/
#if LOADER_DEBUG || LOADER_PROFILE
#include <time.h>
extern int DLREL_relocations;
extern time_t DLREL_total_reloc_time;
#endif

/*---------------------------------------------------------------------------*/
/* Landing point for core loader's relocation processor.                     */
/*---------------------------------------------------------------------------*/
void DLREL_relocate(LOADER_FILE_DESC *fd, DLIMP_Dynamic_Module *dyn_module);

#endif
