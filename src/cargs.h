/*
 * Copyright (c) 2018, 2019, 2020, 2021, 2022, 2023, 2024
 * likle code happy UG (haftungsbeschränkt)
 * Lars Paetzke <lars.paetzke@gmail.com>
 *
 * This file is part of cargs.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CARGS_H
#define CARGS_H

//
// --- START OF FIXES ---
//
// We need these headers for the types used in the function definitions
// (FILE*, size_t, bool)
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * This is the new logic to make cargs a DLL.
 * It's the same pattern as your Util.h.
 * CARGS_EXPORTS will be defined by cargs_dll.c
 */
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

/**
 * @brief DllMain is the entry point for a Windows DLL.
 * It's called by the system when the DLL is loaded or unloaded.
 */
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Called when the DLL is loaded into a process's address space.
        // We can do initialization here.
        printf("cargs.dll loaded!\n");
        break;
    case DLL_THREAD_ATTACH:
        // Called when a new thread is created in the process.
        break;
    case DLL_THREAD_DETACH:
        // Called when a thread exits.
        break;
    case DLL_PROCESS_DETACH:
        // Called when the DLL is unloaded.
        // We can do cleanup here.
        printf("cargs.dll unloaded.\n");
        break;
    }
    return TRUE; // Successful
}
    #ifdef CARGS_EXPORTS
        #define CARGS_API __declspec(dllexport)
    #else
        #define CARGS_API __declspec(dllimport)
    #endif
#else
    // On Linux/macOS, we don't need this
    #define CARGS_API
#endif
//
// --- END OF FIXES ---
//


#define CAG_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cag_option {
	/**
	 * This is the identifier for the option. It is used to identify the
	 * option in the context. It should be a unique character.
 */
	const char identifier;

	/**
	 * This is a list of access letters for the option. It is a string
	 * containing all letters that should be mapped to this option.
	 * For example, if the access letters are "abc", then -a, -b and -c
	 * will all be mapped to this option.
	 * This can be NULL, in which case the option can only be accessed
	 * using the access name.
	 */
	const char *access_letters;

	/**
	 * This is the access name for the option. It is a string containing
	 * the name of the option. For example, if the access name is
	 * "help", then --help will be mapped to this option.
	 * This can be NULL, in which case the option can only be accessed
	 * using the access letters.
	 */
	const char *access_name;

	/**
	 * This is the name of the value for the option. It is used to
	 * display the help message for the option. For example, if the
	 * value name is "FILE", then the help message will be:
	 * -f, --file FILE   description
	 * This can be NULL, in which case the option is considered a flag.
	 */
	const char *value_name;

	/**
	 * This is the description of the option. It is used to display the
	 * help message for the option.
 * display the help message for the option.
 */
	const char *description;
} cag_option;

/**
 * @brief This structure defines the context for the command line option
 * parser.
 *
 * It is used to store the state of the parser.
 */
typedef struct cag_option_context {
	const cag_option *options;
	size_t option_count;
	int argc;
	char **argv;
	int index;
	const char *value;
	char identifier;
	const cag_option *current;
} cag_option_context;

/**
 * @brief Initializes the command line option parser context.
 *
 * @param context The context to initialize.
 * @param options The array of options.
 * @param option_count The number of options in the array.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 */
// We add our CARGS_API macro to each function
CARGS_API void cag_option_init(cag_option_context *context, const cag_option *options,
			size_t option_count, int argc, char **argv);

/**
 * @brief Fetches the next command line option.
 *
 * @param context The context to use.
 * @return Returns true if an option was found, false otherwise.
 */
CARGS_API bool cag_option_fetch(cag_option_context *context);

/**
 * @brief Gets the identifier of the current option.
 *
 * @param context The context to use.
 * @return Returns the identifier of the current option.
 */
CARGS_API char cag_option_get_identifier(const cag_option_context *context);

/**
 * @brief Gets the value of the current option.
 *
 * @param context The context to use.
 * @return Returns the value of the current option.
 */
CARGS_API const char *cag_option_get_value(const cag_option_context *context);

/**
 * @brief Gets the current index of the arguments.
 *
 * @param context The context to use.
 * @return Returns the current index of the arguments.
 */
CARGS_API int cag_option_get_index(const cag_option_context *context);

/**
 * @brief Prints the help message for the options.
 *
 * @param context The context to use.
 * @param fp The file pointer to print to.
 */
CARGS_API void cag_option_print_help(const cag_option_context *context, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* CARGS_H */

#ifdef CARGS_IMPLEMENTATION

// stdio.h is already included above
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

// --- ADDED: Forward declarations for static functions ---
static const cag_option *cag_option_find(const cag_option_context *context,
					const char *access);

static const cag_option *cag_option_find_char(const cag_option_context *context,
						char access);
// --- END OF ADDED DECLARATIONS ---


// --- START OF FIXED SECTION: Added missing function bodies ---
static const cag_option *cag_option_find(const cag_option_context *context,
					const char *access)
{
	size_t i;

	if (access == NULL) {
		return NULL;
	}

	for (i = 0; i < context->option_count; ++i) {
		if (context->options[i].access_name != NULL &&
			strcmp(context->options[i].access_name, access) == 0) {
			return &context->options[i];
		}
	}

	return NULL;
}

static const cag_option *cag_option_find_char(const cag_option_context *context,
						char access)
{
	size_t i;

	for (i = 0; i < context->option_count; ++i) {
		if (context->options[i].access_letters != NULL &&
			strchr(context->options[i].access_letters, access) !=
				NULL) {
			return &context->options[i];
		}
	}

	return NULL;
}
// --- END OF FIXED SECTION ---


// Add CARGS_API to the implementation definitions as well
CARGS_API void cag_option_init(cag_option_context *context, const cag_option *options,
			size_t option_count, int argc, char **argv)
{
	memset(context, 0, sizeof(cag_option_context));
	context->options = options;
	context->option_count = option_count;
	context->argc = argc;
	context->argv = argv;
	context->index = 1;
}

CARGS_API bool cag_option_fetch(cag_option_context *context)
{
	char *argument; // <-- FIXED: Removed 'const'
	size_t argument_length;
	const cag_option *option;
	char *access;
	char *value;
	char *rest;

	context->value = NULL;
	context->identifier = '\0';
	context->current = NULL;

	if (context->index >= context->argc) {
		return false;
	}

	argument = context->argv[context->index];
	argument_length = strlen(argument);
	if (argument_length == 0 || argument[0] != '-') {
		return false;
	}

	// long option
	if (argument_length > 1 && argument[1] == '-') {
		access = (char *)argument + 2;
		value = strchr(access, '=');
		if (value != NULL) {
			*value = '\0';
			value++;
		}

		option = cag_option_find(context, access);
		if (option == NULL) {
			context->identifier = '?';
			return true;
		}

		if (option->value_name == NULL) {
			if (value != NULL) {
				context->identifier = '!';
				return true;
			}
		} else {
			if (value != NULL) {
				context->value = value;
			} else if (context->index + 1 < context->argc) {
				context->value =
					context->argv[context->index + 1];
				context->index++;
			} else {
				context->identifier = ':';
				return true;
			}
		}
		context->current = option;
		context->identifier = option->identifier;
		context->index++;
	} else { // short option
		access = (char *)argument + 1;
		option = cag_option_find_char(context, *access);
		if (option == NULL) {
			context->identifier = '?';
			return true;
		}

		if (option->value_name == NULL) {
			if (strlen(access) > 1) {
				memmove(argument + 1, access + 1,
					strlen(access));
				context->identifier = option->identifier;
				context->current = option;
				return true;
			}
		} else {
			value = (char *)access + 1;
			if (strlen(value) > 0) {
				context->value = value;
			} else if (context->index + 1 < context->argc) {
				context->value =
					context->argv[context->index + 1];
				context->index++;
			} else {
				context->identifier = ':';
				return true;
			}
		}

		context->identifier = option->identifier;
		context->current = option;
		context->index++;
	}

	return true;
}

CARGS_API char cag_option_get_identifier(const cag_option_context *context)
{
	return context->identifier;
}

CARGS_API const char *cag_option_get_value(const cag_option_context *context)
{
	return context->value;
}

CARGS_API int cag_option_get_index(const cag_option_context *context)
{
	return context->index;
}

static void print_help_entry(FILE *fp, const cag_option *option)
{
	char access_letters_buffer[1024];
	char access_name_buffer[1024];
	size_t i, pos;

	pos = 0;
	memset(access_letters_buffer, 0, sizeof(access_letters_buffer));
	if (option->access_letters != NULL) {
		for (i = 0; i < strlen(option->access_letters); ++i) {
			pos += (size_t)snprintf(access_letters_buffer + pos,
						sizeof(access_letters_buffer) -
							pos,
						"-%c, ",
						option->access_letters[i]);
		}
	}

	memset(access_name_buffer, 0, sizeof(access_name_buffer));
	if (option->access_name != NULL) {
		snprintf(access_name_buffer, sizeof(access_name_buffer),
				"--%s", option->access_name);
	}

	fprintf(fp, "  %s%s", access_letters_buffer, access_name_buffer);
	if (option->value_name != NULL) {
		fprintf(fp, " %s", option->value_name);
	}

	fprintf(fp, "\n      %s\n", option->description);
}

CARGS_API void cag_option_print_help(const cag_option_context *context, FILE *fp)
{
	size_t i;

	fprintf(fp, "Usage: %s [options]\n", context->argv[0]);
	fprintf(fp, "Options:\n");

	for (i = 0; i < context->option_count; ++i) {
		print_help_entry(fp, &context->options[i]);
	}
}

#endif /* CARGS_IMPLEMENTATION */