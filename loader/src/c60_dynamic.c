/*
 * c60_dynamic.c
 *
 * C6x-specific dynamic loader functionality
 *
 */

#ifdef C60_TARGET
#include "c60_elf32.h"
#include <inttypes.h>
#include <stdlib.h>
#include "dload.h"
#define puts
#define printf
/*****************************************************************************/
/* c60_process_dynamic_tag()                                                 */
/*                                                                           */
/*   Process C6x specific dynamic tags.                                      */
/*****************************************************************************/
BOOL DLDYN_c60_process_dynamic_tag(DLIMP_Dynamic_Module* dyn_module, int i) {
	switch (dyn_module->dyntab[i].d_tag) {
	/*------------------------------------------------------------------*/
	/* DT_C6000_GSYM_OFFSET: Dynamic symbol table is partitioned into   */
	/*                       local and global symbols. This tag has the */
	/*                       offset into the dynamic symbol table where */
	/*                       the global symbol table starts.            */
	/*------------------------------------------------------------------*/
	case DT_C6000_GSYM_OFFSET:
		dyn_module->gsymtab_offset = dyn_module->dyntab[i].d_un.d_val;
#if LOADER_DEBUG
		if (debugging_on)
			printf("Found global symbol table: %d\n",
					dyn_module->gsymtab_offset);
#endif
		return TRUE;

		/*------------------------------------------------------------------*/
		/* DT_C6000_GSTR_OFFSET: Contains the offset into the dynamic       */
		/*                       string table where the global symbol names */
		/*                       start.                                     */
		/*------------------------------------------------------------------*/
	case DT_C6000_GSTR_OFFSET:
		dyn_module->gstrtab_offset = dyn_module->dyntab[i].d_un.d_val;
#if LOADER_DEBUG
		if (debugging_on)
			printf("Found global string table: %d\n",
					dyn_module->gstrtab_offset);
#endif
		return TRUE;

		/*------------------------------------------------------------------*/
		/* DT_C6000_DSBT_BASE:  Contains address of DSBT in executable or   */
		/*                      shared object.                              */
		/*                      We store the tag's location in the dynamic  */
		/*                      module object so that we can update it      */
		/*                      easily after the sections have been         */
		/*                      allocated (tag value is relocated).         */
		/*------------------------------------------------------------------*/
	case DT_C6000_DSBT_BASE:
		dyn_module->dsbt_base_tagidx = i;
		return TRUE;

		/*------------------------------------------------------------------*/
		/* DT_C6000_DSBT_INDEX: Contains specific request for  a DSBT       */
		/*                      index. If this object module doesn't get    */
		/*                      the index it requested, then the load will  */
		/*                      fail (object module has already assumed     */
		/*                      that it got the DSBT index it asks for;     */
		/*                      references to the DSBT index will not have  */
		/*                      relocation entries associated with them).   */
		/*------------------------------------------------------------------*/
	case DT_C6000_DSBT_INDEX:
		dyn_module->dsbt_index = dyn_module->dyntab[i].d_un.d_val;
		return TRUE;

		/*------------------------------------------------------------------*/
		/* DT_C6000_DSBT_SIZE:  Contains the size of the DSBT allocated for */
		/*                      this object module. It must be big enough   */
		/*                      to hold the content of the master DSBT.     */
		/*------------------------------------------------------------------*/
	case DT_C6000_DSBT_SIZE:
		dyn_module->dsbt_size = dyn_module->dyntab[i].d_un.d_val;
		return TRUE;

	}

	return FALSE;
}

/*****************************************************************************/
/* DLDYN_c60_relocate_dynamic_tag_info()                                     */
/*                                                                           */
/*    Update any target specific dynamic tag values that are associated with */
/*    a section address. Return TRUE if the tag value is successfully        */
/*    updated or if the tag is not associated with a section address, and    */
/*    FALSE if we can't find the sectoin associated with the tag or if the   */
/*    tag type is not recognized.                                            */
/*                                                                           */
/*****************************************************************************/
BOOL DLDYN_c60_relocate_dynamic_tag_info(DLIMP_Dynamic_Module *dyn_module,
		int32_t i) {
	switch (dyn_module->dyntab[i].d_tag) {
	/*---------------------------------------------------------------------*/
	/* These tags do not point to sections.                                */
	/*---------------------------------------------------------------------*/
	case DT_C6000_GSYM_OFFSET:
	case DT_C6000_GSTR_OFFSET:
	case DT_C6000_DSBT_INDEX:
	case DT_C6000_DSBT_SIZE:
		return TRUE;

		/*---------------------------------------------------------------------*/
		/* DT_C6000_DSBT_BASE: This tag value provides the virtual address of  */
		/*                     the .dsbt section. We will go find the program  */
		/*                     header entry associated with the DSBT section   */
		/*                     and update this tag with the section's run      */
		/*                     address.                                        */
		/*---------------------------------------------------------------------*/
	case DT_C6000_DSBT_BASE:
		return DLIMP_update_dyntag_section_address(dyn_module, i);
	}

	DLIF_error(DLET_MISC, "Invalid dynamic tag encountered, %d\n",
			(int) dyn_module->dyntab[i].d_tag);
	return FALSE;
}

/*****************************************************************************/
/* c60_process_eiosabi()                                                     */
/*                                                                           */
/*   Process the EI_OSABI value. Verify that the OSABI is supported and set  */
/*   any variables which depend on the OSABI.                                */
/*****************************************************************************/
BOOL DLDYN_c60_process_eiosabi(DLIMP_Dynamic_Module* dyn_module) {
	uint8_t osabi = dyn_module->fhdr.e_ident[EI_OSABI];

	if (dyn_module->relocatable) {
		/*-------------------------------------------------------------------*/
		/* ELFOSABI_C6000_ELFABI - C6x Baremetal ABI                         */
		/*-------------------------------------------------------------------*/
		if (osabi == ELFOSABI_C6000_ELFABI)
			return TRUE;

#if 0
		/*-------------------------------------------------------------------*/
		/* ELFOSABI_C6000_LINUX - C6x Linux ABI                              */
		/* presently unsupported                                             */
		/*-------------------------------------------------------------------*/
		if (osabi == ELFOSABI_C6000_LINUX)
		return TRUE;
#endif
	} else {
		/*-------------------------------------------------------------------*/
		/* Static executables should have an OSABI of NONE.                  */
		/*-------------------------------------------------------------------*/
		if (osabi == ELFOSABI_NONE)
			return TRUE;
	}

	return FALSE;
}

#endif
