#include <c6x.h>
#include <tistdtypes.h>
#include <cslr_pllc.h>
#include <cslr_xmc.h>
#include <cslr_cgem.h>
#include <csl_cgemAux.h>
#include <cslr_bootcfg.h>
#include <cslr_tpcc.h>
#include <cslr_tptc.h>
#include <cslr_device.h>
#include <cslr_cpintc.h>
#include <cslr_tmr.h>
#include <cslr_srio.h>
#include <cslr_vusr.h>
#include <csl_psc.h>
#include <csl_tmr.h>
#include <csl_cache.h>
#include <csl_cacheAux.h>
#include <csl_xmcAux.h>
#include <cslr_msmc.h>
#include <csl_idma.h>
#include <cslr_mpu.h>
#include <cslr_gpio.h>
#include <cslr_emif4f.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

//#include <rtems.h>

#include "ArrayList.h"
#include "symtab.h"
#include "dload.h"
#include "dload_api.h"
#include "util.h"
#include "dlw_dsbt.h"
#include "dlw_trgmem.h"

void WritebackAllCache(void * blockPtr)
{
	if((Uint32)blockPtr>=0x0C000000&&(Uint32)blockPtr<0x0D000000)	//SL2
	{
		CACHE_wbAllL1d(CACHE_WAIT);;
	}
	else if((Uint32)blockPtr>=0x10000000) 	//DDR or other cores
	{
		CACHE_wbAllL2(CACHE_WAIT);
	}
	_mfence();
	_mfence();
}

/*---------------------------------------------------------------------------*/
/* Global flag to control debug output.                                      */
/*---------------------------------------------------------------------------*/
#if LOADER_DEBUG
BOOL debugging_on = 0;
#endif

/*---------------------------------------------------------------------------*/
/* Global flag to enable profiling.                                          */
/*---------------------------------------------------------------------------*/
#if LOADER_DEBUG || LOADER_PROFILE
BOOL profiling_on = 0;
#endif

/*---------------------------------------------------------------------------*/
/* User command data structure                                               */
/*---------------------------------------------------------------------------*/
enum command_id_t {
	DOWNLOAD_EXECUTABLE,
	LOAD_EXECUTABLE,
	EXIT,
	HELP,
	LIST,
	EXECUTE_PROGRAM,
	UNLOAD_PROGRAM,
	BASE_IMAGE,
	COMMENT,
	ERROR_ID
};

struct commands_t {
	const char *command;
	enum command_id_t id;
};

static struct commands_t commands[] = {
	{ "download_executable", DOWNLOAD_EXECUTABLE },
	{ "load_executable", LOAD_EXECUTABLE },
	{ "exit", EXIT },
	{ "help", HELP },
	{ "list", LIST },
	{ "execute_program", EXECUTE_PROGRAM },
	{ "unload_program", UNLOAD_PROGRAM },
	{ "base_image", BASE_IMAGE },
	{ "#", COMMENT },
};

/*****************************************************************************/
/* HELP() - Print a brief summary of the available commands.                 */
/*****************************************************************************/
static void help(void) {
	int i;

	fprintf(stderr, "Commands:\n");

	for (i = 0; i < sizeof(commands) / sizeof(*commands); i++)
		printf("\t%s\n", commands[i].command);
}

/*****************************************************************************/
/* FIND_COMMAND_ID() - Look up a command name or partial command name in the */
/*                     commands table.  Return an enum representing command. */
/*****************************************************************************/
static enum command_id_t find_command_id(const char *tok) {
	int i, found = -1;
	size_t tok_len = strlen(tok);

	for (i = 0; i < sizeof(commands) / sizeof(*commands); i++)
		if (!strncasecmp(tok, commands[i].command, tok_len)) {
			if (found != -1) {
				fprintf(stderr, "ERROR: Ambiguous command '%s'\n",
						tok);
				return ERROR_ID;
			}
			found = i;
		}

	if (found != -1)
		return commands[found].id;
	else {
		fprintf(stderr, "Unknown command '%s'\n", tok);
		return ERROR_ID;
	}
}

/*****************************************************************************/
/* INPUT_SERVER abstracts whether the commands come from the interactive     */
/* command line, a script file, or the --command option			     */
/*****************************************************************************/
typedef struct input_server_t {
	enum {
		IST_FILE, IST_STR
	} type;
	FILE *input_file;
	const char *string;
	size_t string_pos;
} INPUT_SERVER;

static INPUT_SERVER *cmd_set_file(FILE *input_file) {
	INPUT_SERVER *input_server = malloc(sizeof(INPUT_SERVER));
	input_server->type = IST_FILE;
	input_server->input_file = input_file;
	return input_server;
}
#if 0
static INPUT_SERVER *cmd_set_string(const char *str) {
	INPUT_SERVER *input_server = malloc(sizeof(INPUT_SERVER));
	input_server->type = IST_STR;
	input_server->string = str;
	input_server->string_pos = 0;
	return input_server;
}
#endif
static int cmd_getc(INPUT_SERVER *server) {
	if (server->type == IST_FILE)
		return fgetc(server->input_file);
	else {
		int ch;
		if (!server->string)
			return EOF;
		ch = server->string[server->string_pos++];
		if (!ch)
			return EOF;
		return ch;
	}
}

static int cmd_error(INPUT_SERVER *server) {
	if (server->type == IST_FILE)
		return ferror(server->input_file);
	else
		return server->string == NULL;
}

static int cmd_eof(INPUT_SERVER *server) {
	if (server->type == IST_FILE)
		return feof(server->input_file);
	else
		return !cmd_error(server) && !server->string[server->string_pos];
}

static int cmd_isstdin(INPUT_SERVER *server) {
	if (server->type == IST_FILE)
		return server->input_file == stdin;
	else
		return 0;
}

/*****************************************************************************/
/* GET_LINE() - Read up to buf_len bytes worth of command-line input.	     */
/*****************************************************************************/
static char *get_line(char *cmd_buffer, size_t buf_len, INPUT_SERVER *server) {
	size_t pos = 0;
	/* fgets always NUL-terminates, we'll do the same */

	while (pos < (buf_len - 1)) {
		int ch;

		if ((ch = cmd_getc(server)) == EOF) {
			if (cmd_error(server))
				return NULL;
			else if (pos)
				goto done;
			/* EOF, but read at least 1 char */
			else
				return NULL;
		} else {
			if (ch == ';' || ch == '\n') {
				/* End of command.  Discard ';' or '\n' */
				goto done;
			} else if (ch == '#') {
				/* comment start, discard everything until end-of-line */
				while (((ch = cmd_getc(server)) != EOF) && ch != '\n')
					;
				goto done;
			} else {
				cmd_buffer[pos++] = ch;
			}
		}
	}

	/* Buffer overrun */
	fprintf(stderr, "ERROR: command buffer overrun.\n");
	exit(1);

	done:

	cmd_buffer[pos] = '\0';
	return cmd_buffer;
}


/* user-defined optimizations here */
#define MAX_NUM 10
typedef int (*loaded_pointer)();

/*---------------------------------------------------------------------------*/
/* Global function pointer that will be set to point at whatever entry       */
/* point that we want to be in effect when the "execute" command is          */
/* specified.                                                                */
/*---------------------------------------------------------------------------*/
static loaded_pointer loaded_program = NULL;

/*---------------------------------------------------------------------------*/
/* Handle of the loaded program.                                             */
/*---------------------------------------------------------------------------*/
static uint32_t prog_handle;

static char * loaded_name_table[MAX_NUM];
static uint32_t prog_handle_table[MAX_NUM];
static loaded_pointer entry_point_table[MAX_NUM];

static void list_names(void)
{
	int i;
	for (i = 0; i < MAX_NUM; ++i)
	{
		if (NULL == loaded_name_table[i])
			continue;
		printf("Module: %s\n", loaded_name_table[i]);
	}
}

static int query_name(const char *name)
{
	int i;
	for (i = 0; i < MAX_NUM; ++i)
	{
		if (NULL == loaded_name_table[i])
			continue;
		if (0 == strcasecmp(name, loaded_name_table[i]))
			return i;
	}
	return -1;
}

static void remove_slot(int i)
{
	if (-1 != i)
	{
		free(loaded_name_table[i]);
		loaded_name_table[i] = NULL;
		entry_point_table[i] = NULL;
		prog_handle_table[i] = 0;
	}
}

static void insert_slot(const char *name)
{
	char *pname;
	int i, len;
	for (i = 0; i < MAX_NUM; ++i)
	{
		/* look for if loaded before*/
		if (NULL == entry_point_table[i])
		{
			len = strlen(name);
			pname = malloc(len + 1);
			/* if name is malloc'd OK, register it */
			if (pname != NULL)
			{
				strncpy(pname, name, len);
				pname[len] = '\0';
				loaded_name_table[i] = pname;
				entry_point_table[i] = loaded_program;
				prog_handle_table[i] = prog_handle;
				return;
			}
		}
	}
}

/*****************************************************************************/
/* LOAD_EXECUTABLE() - Invoke the loader to create an executable image of    */
/*      the specified program, saving the loader's return value as the       */
/*      future entry point.                                                  */
/*****************************************************************************/
static void load_executable(const char* file_name, int argc, char** argv) {
	/*------------------------------------------------------------------------*/
	/* Open specified file from "load" command, load it, then close the file. */
	/*------------------------------------------------------------------------*/

	FILE* fp = fopen(file_name, "rb");

	/*------------------------------------------------------------------------*/
	/* Were we able to open the file successfully?                            */
	/*------------------------------------------------------------------------*/
	if (!fp) {
		DLIF_error(DLET_FILE, "Failed to open file '%s'.\n", file_name);
		return;
	}


	/*------------------------------------------------------------------------*/
	/* Now, we are ready to start loading the specified file onto the target. */
	/*------------------------------------------------------------------------*/
	prog_handle = DLOAD_load(fp, argc, argv);
	fclose(fp);

	/*------------------------------------------------------------------------*/
	/* If the load was successful, then we'll need to write the debug         */
	/* information for this file into target memory.                          */
	/*------------------------------------------------------------------------*/
	if (prog_handle) {
		/*---------------------------------------------------------------------*/
		/* Find entry point associated with loaded program's handle, set up    */
		/* entry point in "loaded_program".                                    */
		/*---------------------------------------------------------------------*/
		DLOAD_get_entry_point(prog_handle, (TARGET_ADDRESS) (&loaded_program));
		printf("loaded_program: 0x%x\n", loaded_program);
	}

	/*------------------------------------------------------------------------*/
	/* Report failure to load an object file.                                 */
	/*------------------------------------------------------------------------*/
	else
		DLIF_error(DLET_MISC, "Failed load of file '%s'.\n", file_name);
}

/*****************************************************************************/
/* EXECUTE_PROGRAM() - Execute loaded program and print return value.        */
/*****************************************************************************/
static void execute_program(int argc, char** argv) {
	/*------------------------------------------------------------------------*/
	/* Have we got an entry point that we begin executing at?                 */
	/*------------------------------------------------------------------------*/
	if (loaded_program == NULL) {
		fprintf(stderr, "ERROR: No program has been loaded.\n");
		return;
	}

	/*------------------------------------------------------------------------*/
	/* Call loaded program at the entry point in "loaded_program".            */
	/*------------------------------------------------------------------------*/
	printf("Return value: %d\n", loaded_program(argc, argv));
}

static int32_t prog_argc;
static Array_List prog_argv;

#if 0
static rtems_task new_init(rtems_task_argument index)
{
	int ret = loaded_program(prog_argc, (char**) (prog_argv.buf));
	printf("Return value: %d\n", ret);
	/* Make sure we can return to shell*/
	rtems_task_delete(RTEMS_SELF);
}
#endif

void recv_task(int port, char *buffer, int len);

void dloader_main(int argc, char** argv)
{
	/*------------------------------------------------------------------------*/
	/* Current length of a command is limited to a fixed-sized buffer.   */
	/* Exceeding this fixed buffer will result in a runtime error.            */
	/*------------------------------------------------------------------------*/
	char cmd_buffer[1000];

	/*------------------------------------------------------------------------*/
	/* Identify command-line input stream.  By default, this is stdin, but    */
	/* we can take commands from a script, using the --script option.         */
	/*------------------------------------------------------------------------*/
	INPUT_SERVER *cmd_server = cmd_set_file(stdin);

	/*------------------------------------------------------------------------*/
	/* Initialize the dynamic loader.                                         */
	/*------------------------------------------------------------------------*/
	DLOAD_initialize();

	/*------------------------------------------------------------------------*/
	/* Initialize the client's model of the master DSBT.                      */
	/*------------------------------------------------------------------------*/
	AL_initialize(&DSBT_master, sizeof(DSBT_Entry), 1);

	/*------------------------------------------------------------------------*/
	/* Banner for user interaction mode.                                      */
	/*------------------------------------------------------------------------*/
	if (cmd_isstdin(cmd_server)) {
		printf("Dynamic Loader Using %s\n", DLOAD_version());
	}

	/*------------------------------------------------------------------------*/
	/* Command processing loop.                                               */
	/*------------------------------------------------------------------------*/
	cmd_buffer[0] = '\0';

	while (1) {
		int pos = -1, i;
		char* pathname;
		char* tok, *str_argc;
		char* base_image_name;
		FILE* image;
		FILE *exec_image;
		char *tmp_image;
		const int SIZE = 1024*1024;

		/*---------------------------------------------------------------------*/
		/* In user interactive mode, prompt user for next command.             */
		/*---------------------------------------------------------------------*/
		if (cmd_isstdin(cmd_server)) printf(">> ");

		/*---------------------------------------------------------------------*/
		/* Read up to semicolon, newline, or comment character.                */
		/*---------------------------------------------------------------------*/
		if (get_line(cmd_buffer, sizeof(cmd_buffer), cmd_server) == NULL) {
			if (cmd_eof(cmd_server)) {
				if (cmd_isstdin(cmd_server)) printf("exit\n");
				exit(0);
			} else {
				fprintf(stderr, "FATAL: Unknown error reading commands\n");
				exit(1);
			}
		}

		/*---------------------------------------------------------------------*/
		/* Scan first token, skip over any empty command lines.                */
		/*---------------------------------------------------------------------*/
		tok = strtok(cmd_buffer, " \n");
		if (!tok)
			continue;

		/*---------------------------------------------------------------------*/
		/* We have some kind of command.  Transfer control to the right place  */
		/* in the loader.                                                      */
		/*---------------------------------------------------------------------*/
		switch (find_command_id(tok)) {

		case EXIT:
			return;

		case HELP:
			help();
			break;

		case LIST:
			list_names();
			break;

		case DOWNLOAD_EXECUTABLE:
#if 0
			tmp_image = malloc(SIZE);
			if (tmp_image == NULL) {
				puts("DOWNLOAD_EXECUTABLE failed");
				break;
			}
			recv_task(5, tmp_image, SIZE);
			base_image_name = "__tmp_image";
			exec_image = fopen(base_image_name, "wb");
			if (fwrite(tmp_image, SIZE, 1, exec_image)) {
				puts("fwrite __tmp_image failed");
				break;
			}

			if (exec_image)
				fclose(exec_image);
#endif
			break;

			/*------------------------------------------------------------------*/
			/* "load <program>"                                                 */
			/*                                                                  */
			/* Load an executable program into memory with the supplied command */
			/* line arguments.  This will load any dependent files as well.     */
			/*------------------------------------------------------------------*/
		case LOAD_EXECUTABLE:
			pathname = strtok(NULL, " \n");

			/* make sure it has not yet been loaded */
			pos = query_name(pathname);
			if (-1 == pos)
			{
				/*---------------------------------------------------------------*/
				/* Go invoke the core loader to load the specified program.      */
				/*---------------------------------------------------------------*/
				load_executable(pathname, 0, NULL);

				/*---------------------------------------------------------------*/
				/* Did we get a valid program handle back from the loader?       */
				/*---------------------------------------------------------------*/
				if (!prog_handle && !cmd_isstdin(cmd_server)) {
					fprintf(stderr, "FATAL: load_executable failed in "
									"script. Terminating.\n");
					exit(1);
				}

				insert_slot(pathname);
			}
			else
			{
				fprintf(stderr, "ERROR: Program: %s "
						"has already been loaded\n", pathname);
			}

			WritebackAllCache((void *)0x0C000000);

			break;

			/*------------------------------------------------------------------*/
			/* "execute <program> [<argc> <argv[0]> <argv[1]> ...]"             */
			/*                                                                  */
			/* Transfer control to the most recently loaded program.  Expect    */
			/* "loaded_program" to be pointing at entry point that we want to   */
			/* start with.                                                      */
			/*------------------------------------------------------------------*/
		case EXECUTE_PROGRAM:
			pathname = strtok(NULL, " \n");
			str_argc = strtok(NULL, " \n");
			if (str_argc)
				prog_argc = strtoul(str_argc, NULL, 0);
			else
				prog_argc = 0;

			/*---------------------------------------------------------------*/
			/* Initialize Target Memory Allocator Interface                  */
			/*---------------------------------------------------------------*/
			AL_initialize(&prog_argv, sizeof(char*), 1);

			/*---------------------------------------------------------------*/
			/* Allocate a private copy of each argv string specified in the  */
			/* load command.                                                 */
			/*---------------------------------------------------------------*/
			for (i = 0; i < prog_argc; i++) {
				char* temp = malloc(100);
				strcpy(temp, strtok(NULL, " \n"));
				AL_append(&prog_argv, &temp);
			}

			/*---------------------------------------------------------------*/
			/* Write out progress information for arguments read from load   */
			/* command.                                                      */
			/*---------------------------------------------------------------*/
			for (i = 0; i < prog_argc; i++)
				printf("Arg %d: %s\n", i, *((char**) (prog_argv.buf) + i));

			pos = query_name(pathname);
			if (-1 == pos)
			{
				fprintf(stderr, "ERROR: No such program: %s "
						"has been loaded\n", pathname);
			}
			else
			{
				/* Get entry point */
				loaded_program = entry_point_table[pos];

				/*
				rtems_status_code status;
				rtems_id id;
				int ret;
				status = rtems_task_create(
						rtems_build_name('I', 'N', 'I', ' '), // INIT
						1,
						RTEMS_MINIMUM_STACK_SIZE * 2,
						RTEMS_INTERRUPT_LEVEL(31),
						RTEMS_DEFAULT_ATTRIBUTES,
						&id);

				status = rtems_task_start(id, new_init, 0);
				*/
				execute_program(prog_argc, (char**) (prog_argv.buf));
			}
			AL_destroy(&prog_argv);

			break;

			/*------------------------------------------------------------------*/
			/* "unload <program>"                                               */
			/*                                                                  */
			/* Unload the last program that was loaded into memory.  Does this  */
			/* include any base image symbols?                                  */
			/*------------------------------------------------------------------*/
			/* We'll only worry about removing debug information if the module  */
			/* is actually unloaded from target space (DLOAD_unload() returns   */
			/* TRUE only if module is no longer needed and has indeed been      */
			/* unloaded from target memory).                                    */
			/*------------------------------------------------------------------*/
		case UNLOAD_PROGRAM:
			pathname = strtok(NULL, " \n");
			pos = query_name(pathname);
			if (-1 == pos)
			{
				fprintf(stderr, "ERROR: No such program: %s "
						"has been loaded\n", pathname);
			}
			else
			{
				prog_handle = prog_handle_table[pos];
				if (DLOAD_unload(prog_handle)) {
					DSBT_release_entry(prog_handle);
				}
				remove_slot(pos);
			}
			loaded_program = NULL;

			break;

			/*------------------------------------------------------------------*/
			/* "base_image <program>"                                           */
			/*                                                                  */
			/* Load the global symbols from the dynamic symbol table of the     */
			/* specified program.  It is assumed that the specified program     */
			/* has already been loaded into target memory and is running.       */
			/*------------------------------------------------------------------*/
		case BASE_IMAGE:
			base_image_name = strtok(NULL, " \n");
			image = fopen(base_image_name, "rb");

			if (!image) {
				DLIF_error(DLET_FILE, "Failed to open base image file '%s'.\n",
						base_image_name);
				break;
			}

			/* Base image is assumed to be already loaded and running on the */
			/* target.  The dynamic loader still has to read all of the      */
			/* dynamic symbols in the base image so that we can link an      */
			/* incoming DLL against the base image.                          */

			if (!(prog_handle = DLOAD_load_symbols(image))) {
				/*------------------------------------------------------------*/
				/* If we didn't get a proper file handle back from the        */
				/* DLOAD_load_symbols() API function, then we need to recover */
				/* gracefully.                                                */
				/*------------------------------------------------------------*/
				/* If a failure occurs from a script file, then we assume a   */
				/* catastrophic failure and terminate the session.            */
				/*------------------------------------------------------------*/
				if (!prog_handle && !cmd_isstdin(cmd_server)) {
					fprintf(stderr, "FATAL: base_image failed in script. "
									"Terminating.\n");
					exit(1);
				}
			}
			if (image)
				fclose(image);

			break;

			/*------------------------------------------------------------------*/
			/* Unrecognized commands will be reported and ignored.  We just     */
			/* move onto the next prompt or line in the script.                 */
			/* Note that in a script, if a line begins with '#', it is treated  */
			/* as a comment.  Comment delimiter tokens must be first on a line. */
			/*------------------------------------------------------------------*/
		case COMMENT:
		default:
			if (tolower(tok[0]) != '#')
				fprintf(stderr, "ERROR: Unrecognized command ignored.\n");
			continue;
		}
	}
}

