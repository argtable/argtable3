/*******************************************************************************
 * argtable3: Declares the main interfaces of the library
 *
 * This file is part of the argtable3 library.
 *
 * Copyright (C) 1998-2001,2003-2011,2013 Stewart Heitmann
 * <sheitmann@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of STEWART HEITMANN nor the  names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef ARGTABLE3
#define ARGTABLE3

#include <stdio.h> /* FILE */
#include <time.h>  /* struct tm */

#ifdef __cplusplus
extern "C" {
#endif

#define ARG_REX_ICASE 1

/* Maximum length of the command name */
#ifndef ARG_CMD_NAME_LEN
#define ARG_CMD_NAME_LEN 100
#endif /* ARG_CMD_NAME_LEN */

/* Maximum length of the command description */
#ifndef ARG_CMD_DESCRIPTION_LEN
#define ARG_CMD_DESCRIPTION_LEN 256
#endif /* ARG_CMD_DESCRIPTION_LEN */

/**
 * Error codes returned by argument parsing and validation.
 *
 * These error codes are used by Argtable3 to indicate specific problems
 * encountered during command-line parsing and validation. They are typically
 * stored in the `error` array of the `arg_end_t` struct and can be used to
 * generate detailed error messages for the user.
 */
enum {
    ARG_ELIMIT = 1, /**< Too many occurrences of an option or argument */
    ARG_EMALLOC,    /**< Memory allocation failure */
    ARG_ENOMATCH,   /**< Argument value does not match the expected format or pattern */
    ARG_ELONGOPT,   /**< Unknown or invalid long option encountered */
    ARG_EMISSARG    /**< Missing required argument value */
};

/**
 * Bit masks for the `flag` field in the `arg_hdr` struct.
 *
 * The `arg_hdr_flag` enum defines bitwise flags that describe special
 * properties of an argument entry in Argtable3. These flags are stored in the
 * `flag` field of the `arg_hdr` struct and can be combined using bitwise OR.
 *
 * These flags help Argtable3 functions determine how to parse and process each
 * argument entry.
 */
enum arg_hdr_flag {
    ARG_TERMINATOR = 0x1, /**< Marks the end of an argument table (sentinel entry) */
    ARG_HASVALUE = 0x2,   /**< Argument expects a value (e.g., `--output <file>`) */
    ARG_HASOPTVALUE = 0x4 /**< Argument can optionally take a value (e.g., `--color[=WHEN]`) */
};

#if defined(_WIN32)
#if defined(argtable3_EXPORTS)
#define ARG_EXTERN __declspec(dllexport)
#elif defined(argtable3_IMPORTS)
#define ARG_EXTERN __declspec(dllimport)
#else
#define ARG_EXTERN
#endif
#else
#define ARG_EXTERN
#endif

typedef struct _internal_arg_dstr* arg_dstr_t;
typedef void* arg_cmd_itr_t;

typedef void(arg_resetfn)(void* parent);
typedef int(arg_scanfn)(void* parent, const char* argval);
typedef int(arg_checkfn)(void* parent);
typedef void(arg_errorfn)(void* parent, arg_dstr_t ds, int error, const char* argval, const char* progname);
typedef void(arg_dstr_freefn)(char* buf);
typedef int(arg_cmdfn)(int argc, char* argv[], arg_dstr_t res, void* ctx);
typedef int(arg_comparefn)(const void* k1, const void* k2);

/**
 * Defines common properties shared by all `arg_<type>` structs.
 *
 * In the Argtable3 library, every `arg_<type>` struct must begin with an
 * `arg_hdr` struct as its first member. This allows Argtable3 functions to
 * access shared metadata about the command-line option, such as its option
 * tags, data type string, glossary text, and other attributes.
 *
 * The `arg_hdr` struct also contains pointers to type-specific functions
 * provided by each `arg_<type>` implementation. These functions handle tasks
 * such as parsing the option, performing post-parse validation, and reporting
 * errors. Although these function pointers are initialized by the constructor
 * of the respective `arg_<type>` struct, they can be modified by the user after
 * construction if necessary. However, they are intended to remain unchanged
 * once initialized.
 */
typedef struct arg_hdr {
    char flag;             /**< Modifier flags for this option (see `enum arg_hdr_flag`) */
    const char* shortopts; /**< String listing the short option characters (e.g., "hv") */
    const char* longopts;  /**< String listing the long option names, comma-separated (e.g., "verbose,debug") */
    const char* datatype;  /**< Description of the argument data type (e.g., "<file>") */
    const char* glossary;  /**< Description of the option as shown in the glossary/help output */
    int mincount;          /**< Minimum number of occurrences of this option accepted */
    int maxcount;          /**< Maximum number of occurrences of this option accepted */
    void* parent;          /**< Pointer to the parent arg_<type> struct instance */
    arg_resetfn* resetfn;  /**< Pointer to the type-specific reset function for this argument */
    arg_scanfn* scanfn;    /**< Pointer to the type-specific scan (parsing) function */
    arg_checkfn* checkfn;  /**< Pointer to the type-specific validation function */
    arg_errorfn* errorfn;  /**< Pointer to the type-specific error reporting function */
    void* priv;            /**< Pointer to private data for use by arg_<type> functions */
} arg_hdr_t;

/**
 * Structure for storing remarks or custom lines in the syntax or glossary
 * output.
 *
 * The `arg_rem` struct is used to add extra lines of text to the syntax or
 * glossary output generated by Argtable3. Unlike other argument types,
 * `arg_rem` does not correspond to a command-line argument and does not affect
 * argument parsing. Instead, it is a dummy entry that allows you to insert
 * remarks, explanations, or custom formatting into the help or usage messages.
 *
 * This is especially useful for providing additional context, grouping related
 * options, or clarifying the usage of certain arguments in the generated
 * documentation.
 *
 * Example usage:
 * ```
 * // Add extra lines to the glossary for the --update option
 * arg_lit_t *update  = arg_litn("u", "update", 0, 1, "copy only when SOURCE files are");
 * arg_rem_t *update1 = arg_rem(NULL,                 "  newer than destination files");
 * arg_rem_t *update2 = arg_rem(NULL,                 "  or when destination files");
 * arg_rem_t *update3 = arg_rem(NULL,                 "  are missing");
 * void *argtable[] = { update, update1, update2, update3, ... };
 *
 * // Add a data type entry for a positional argument in the syntax
 * arg_rem_t *dest = arg_rem("DEST|DIRECTORY", NULL);
 * void *argtable[] = { ..., dest, ... };
 * ```
 *
 * @see arg_rem()
 */
typedef struct arg_rem {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
} arg_rem_t;

/**
 * Structure for storing literal (boolean flag) argument information.
 *
 * The `arg_lit` struct is used to parse and store literal arguments (boolean
 * flags) from the command line. It is suitable for options that do not take a
 * value, such as `-h` for help or `--verbose` for enabling verbose output. Each
 * occurrence of the flag increases the `count` field, allowing you to detect
 * how many times the flag was specified.
 *
 * Example usage:
 * ```
 * // Accepts a help flag and a verbose flag (which can be specified multiple times)
 * arg_lit_t *help    = arg_lit0("h", "help", "Display help");
 * arg_lit_t *verbose = arg_litn("v", "verbose", 0, 3, "Increase verbosity");
 * arg_end_t *end     = arg_end(20);
 * void *argtable[] = { help, verbose, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (help->count > 0) {
 *     printf("Help requested\n");
 * }
 * if (verbose->count > 0) {
 *     printf("Verbosity level: %d\n", verbose->count);
 * }
 * ```
 *
 * @see arg_lit0, arg_lit1, arg_litn
 */
typedef struct arg_lit {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    int count;          /**< Number of times this flag appears on the command line */
} arg_lit_t;

/**
 * Structure for storing int-typed argument information.
 *
 * The `arg_int` struct is used to parse and store integer arguments from the
 * command line. It is suitable for options that accept numeric values without
 * fractional parts, such as counts, indices, or other whole-number parameters.
 *
 * The `count` field stores the number of successfully matched integer
 * arguments, and the `ival` array holds the parsed integer values as provided
 * by the user.
 *
 * Example usage:
 * ```
 * // Accepts one or more integer arguments
 * arg_int_t *numbers = arg_intn("n", "number", "<int>", 1, 5, "Input numbers");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {numbers, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && numbers->count > 0) {
 *     for (int i = 0; i < numbers->count; ++i) {
 *         printf("Input number: %d\n", numbers->ival[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_int0, arg_int1, arg_intn
 */
typedef struct arg_int {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    int count;          /**< Number of times this argument appears on the command line */
    int* ival;          /**< Array of parsed integer argument values */
} arg_int_t;

/**
 * Structure for storing double-typed argument information.
 *
 * The `arg_dbl` struct is used to parse and store double-precision
 * floating-point arguments from the command line. It is suitable for options
 * that accept numeric values with fractional parts, such as thresholds, ratios,
 * or measurements.
 *
 * The `count` field stores the number of successfully matched double arguments,
 * and the `dval` array holds the parsed double values as provided by the user.
 *
 * Example usage:
 * ```
 * // Accepts one or more double arguments
 * arg_dbl_t *values = arg_dbln("v", "value", "<double>", 1, 5, "Input values");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {values, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && values->count > 0) {
 *     for (int i = 0; i < values->count; ++i) {
 *         printf("Input value: %f\n", values->dval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_dbl0, arg_dbl1, arg_dbln
 */
typedef struct arg_dbl {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    int count;          /**< Number of times this argument appears on the command line */
    double* dval;       /**< Array of parsed double argument values */
} arg_dbl_t;

/**
 * Structure for storing string-typed argument information.
 *
 * The `arg_str` struct is used to parse and store string arguments from the
 * command line. It is suitable for options that accept arbitrary text input,
 * such as file names, user names, or other string values.
 *
 * The `count` field stores the number of successfully matched string arguments,
 * and the `sval` array holds the parsed string values as provided by the user.
 *
 * Example usage:
 * ```
 * // Accepts one or more string arguments
 * arg_str_t *inputs = arg_strn(NULL, NULL, "<input>", 1, 10, "Input strings");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {inputs, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && inputs->count > 0) {
 *     for (int i = 0; i < inputs->count; ++i) {
 *         printf("Input string: %s\n", inputs->sval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_str0, arg_str1, arg_strn
 */
typedef struct arg_str {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    int count;          /**< Number of times this argument appears on the command line */
    const char** sval;  /**< Array of parsed string argument values */
} arg_str_t;

/**
 * Structure for storing regular expression-typed argument information.
 *
 * The `arg_rex` struct is used to parse and store command-line arguments that
 * must match a specified regular expression pattern. This allows applications
 * to validate input against complex patterns, such as email addresses,
 * identifiers, or custom formats.
 *
 * The `pattern` is specified when constructing the argument and is used to
 * check each input value. The `count` field stores the number of successfully
 * matched arguments, and the `sval` array holds the matched strings.
 *
 * Example usage:
 * ```
 * // Accepts one or more arguments matching a simple email pattern
 * arg_rex_t *emails = arg_rexn(NULL, "email", "^[^@]+@[^@]+\\.[^@]+$", "<email>", 1, 10, 0, "Email addresses");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {emails, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && emails->count > 0) {
 *     for (int i = 0; i < emails->count; ++i) {
 *         printf("Matched email: %s\n", emails->sval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_rex0, arg_rex1, arg_rexn
 */
typedef struct arg_rex {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    int count;          /**< Number of times this argument appears on the command line */
    const char** sval;  /**< Array of parsed string argument values */
} arg_rex_t;

/**
 * Structure for storing file-typed argument information.
 *
 * The `arg_file` struct is used to parse and store file path arguments from the
 * command line. It provides convenient access to the full filename, the
 * basename (file name without path), and the file extension for each matched
 * argument. This allows applications to easily process and validate
 * file-related options.
 *
 * The `count` field stores the number of successfully matched file arguments.
 * The `filename` array holds the full file paths as provided by the user, while
 * the `basename` and `extension` arrays provide the corresponding file names
 * and extensions, respectively.
 *
 * Example usage:
 * ```
 * // Accepts one or more file arguments
 * arg_file_t *files = arg_filen(NULL, NULL, "<file>", 1, 100, "Input files");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {files, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && files->count > 0) {
 *     for (int i = 0; i < files->count; ++i) {
 *         printf("File: %s, Basename: %s, Extension: %s\n",
 *             files->filename[i], files->basename[i], files->extension[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_file0, arg_file1, arg_filen
 */
typedef struct arg_file {
    struct arg_hdr hdr;     /**< The mandatory argtable header struct */
    int count;              /**< Number of times this argument appears on the command line */
    const char** filename;  /**< Array of parsed file path values (e.g., /home/foo.bar) */
    const char** basename;  /**< Array of parsed base names (e.g., foo.bar) */
    const char** extension; /**< Array of parsed file extensions (e.g., .bar) */
} arg_file_t;

/**
 * Structure for storing date-typed argument information.
 *
 * The `arg_date` struct is used to parse and store date or time arguments from
 * the command line. It supports flexible date/time formats, which are specified
 * using a `strptime`-style format string. Each successfully parsed date
 * argument is converted into a `struct tm` value and stored in the `tmval`
 * array.
 *
 * The `format` field defines the expected input format for date arguments,
 * allowing you to accept a wide range of date/time styles. The `count` field
 * stores the number of successfully matched arguments, and the `tmval` array
 * holds the parsed results.
 *
 * Example usage:
 * ```
 * // Accepts one required date argument in YYYY-MM-DD format
 * arg_date_t *date = arg_date1(NULL, "date", "%Y-%m-%d", "<date>", "Date in YYYY-MM-DD format");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {date, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && date->count > 0) {
 *     printf("Parsed date: %04d-%02d-%02d\n",
 *         date->tmval[0].tm_year + 1900, date->tmval[0].tm_mon + 1, date->tmval[0].tm_mday);
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @see arg_date0, arg_date1, arg_daten
 */
typedef struct arg_date {
    struct arg_hdr hdr; /**< The mandatory argtable header struct */
    const char* format; /**< strptime format string used to parse the date */
    int count;          /**< Number of times this argument appears on the command line */
    struct tm* tmval;   /**< Array of parsed time values */
} arg_date_t;

/**
 * Structure for collecting parser errors and terminating an argument table.
 *
 * The `arg_end` struct is used in Argtable3 to mark the end of an argument
 * table and to collect information about any errors encountered during
 * command-line parsing. It stores pointers to offending arguments in the input
 * array, allowing the application to report detailed error messages to the
 * user.
 *
 * Typically, an `arg_end_t` instance is created using the `arg_end` function
 * and placed as the last element in the argument table array. After parsing,
 * the structure contains information about missing required arguments, invalid
 * values, or other parsing errors.
 *
 * Example usage:
 * ```
 * arg_lit_t *help = arg_lit0("h", "help", "Display help");
 * arg_int_t *count = arg_int0("c", "count", "<n>", "Number of times");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {help, count, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors > 0) {
 *     arg_print_errors(stdout, end, argv[0]); // handle errors...
 * }
 * ```
 */
typedef struct arg_end {
    struct arg_hdr hdr;  /**< The mandatory argtable header struct */
    int count;           /**< Number of errors encountered */
    int* error;          /**< Array of error codes */
    void** parent;       /**< Array of pointers to offending arg_<type> struct */
    const char** argval; /**< Array of pointers to offending argv[] string */
} arg_end_t;

/**
 * Structure for storing sub-command information.
 *
 * The `arg_cmd_info` struct is used to represent metadata and handler
 * information for a sub-command in a multi-command command-line application.
 * Each sub-command can have its own name, description, handler function, and
 * context pointer, allowing you to build flexible interfaces similar to tools
 * like `git` or `docker`.
 *
 * The `name` field stores the sub-command name, while the `description`
 * provides a short summary for help and usage messages. The `proc` field is a
 * pointer to the function that implements the sub-command's behavior, and `ctx`
 * is a user-defined context pointer that can be used to pass additional data to
 * the handler.
 *
 * Example usage:
 * ```
 * // Define a handler function for the "list" sub-command
 * int list_cmd(int argc, char* argv[], arg_dstr_t res, void* ctx) {
 *     // Implementation for the "list" command
 *     return 0;
 * }
 *
 * // Register the sub-command
 * arg_cmd_register("list", list_cmd, "List all items", NULL);
 *
 * // Retrieve sub-command info
 * arg_cmd_info_t* info = arg_cmd_info("list");
 * if (info) {
 *     printf("Sub-command: %s - %s\n", info->name, info->description);
 * }
 * ```
 *
 * @see arg_cmd_register, arg_cmd_info, arg_cmd_dispatch
 */
typedef struct arg_cmd_info {
    char name[ARG_CMD_NAME_LEN];               /**< Sub-command name */
    char description[ARG_CMD_DESCRIPTION_LEN]; /**< Short description of the sub-command */
    arg_cmdfn* proc;                           /**< Sub-command handler function */
    void* ctx;                                 /**< User-defined context pointer for the sub-command */
} arg_cmd_info_t;

/**** arg_<type> constructor functions *********************************/

/**
 * Adds a remark or custom line to the syntax or glossary output.
 *
 * The `arg_rem` function allows you to insert extra lines of text into the
 * syntax or glossary output generated by Argtable3. Instead of embedding
 * newline characters directly in your argument table strings—which can make
 * the code messy—you can add `arg_rem` structs to your argument table. These
 * are dummy entries: they do not affect argument parsing, but their `datatype`
 * and `glossary` strings are included in the output of `arg_print_syntax` and
 * `arg_print_glossary`.
 *
 * The name `arg_rem` stands for *remark*, inspired by the `REM` statement in
 * the BASIC programming language.
 *
 * For example, in the `mv` example program, we use `arg_rem` to add additional
 * lines for the `-u|--update` option in the glossary:
 * ```
 * arg_lit_t *update  = arg_litn("u", "update", 0, 1, "copy only when SOURCE files are");
 * arg_rem_t *update1 = arg_rem(NULL,                 "  newer than destination files");
 * arg_rem_t *update1 = arg_rem(NULL,                 "  or when destination files");
 * arg_rem_t *update2 = arg_rem(NULL,                 "  are missing");
 * ```
 *
 * which will make the glossay look like:
 * ```
 *   -u, --update                   copy only when SOURCE files are
 *                                    newer than destination files
 *                                    or when the destination files
 *                                    are missing
 * ```
 *
 * We also use `arg_rem` to add a data type entry for the ordinary argument in
 * the syntax:
 * ```
 * arg_rem_t *dest = arg_rem ("DEST|DIRECTORY", NULL);
 * ```
 *
 * which will make the syntax look like:
 * ```
 * $ mv --help
 * Usage: mv [-bfiuv] [--backup=[CONTROL]] [--reply={yes,no,query}]
 * [--strip-trailing-slashes] [-S SUFFIX] [--target-directory=DIRECTORY]
 * [--help] [--version] SOURCE [SOURCE]... DEST|DIRECTORY
 * ```
 *
 * @param datatype  The data type or positional argument string to display in the
 *                  syntax output, or NULL if not needed.
 * @param glossary  The remark or extra line to display in the glossary output,
 *                  or NULL if not needed.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_rem_t`. Otherwise,
 *   returns `NULL` if there is insufficient memory available.
 */
ARG_EXTERN arg_rem_t* arg_rem(const char* datatype, const char* glossary);

/**
 * Creates a literal (boolean flag) argument for the command-line parser.
 *
 * The `arg_litn` function defines an option that does not take a value, such as
 * a boolean flag (e.g., `-h` for help or `--verbose` for verbosity). You can
 * specify the minimum and maximum number of times the flag can appear, making
 * it suitable for optional, required, or repeatable flags. Each occurrence of
 * the flag increases the `count` field in the resulting `arg_lit_t` struct.
 *
 * A classic example is the `tar` utility, which uses `--verbose` or `-v` to
 * show the files being worked on as `tar` is creating an archive:
 * ```
 * $ tar -cvf afiles.tar apple angst aspic
 * apple
 * angst
 * aspic
 * ```
 *
 * Each occurrence of `--verbose` or `-v` on the command line increases the
 * verbosity level by one. Therefore, if you need more details on the output,
 * specify it twice:
 * ```
 * $ tar -cvvf afiles.tar apple angst aspic
 * -rw-r--r-- gray/staff    62373 2006-06-09 12:06 apple
 * -rw-r--r-- gray/staff    11481 2006-06-09 12:06 angst
 * -rw-r--r-- gray/staff    23152 2006-06-09 12:06 aspic
 * ```
 *
 * The `arg_litn` function allows you to specify both the minimum and maximum
 * number of times a flag can appear. For convenience and backward
 * compatibility, `arg_lit0` is provided as a helper for optional flags
 * (where `mincount = 0` and `maxcount = 1`), and `arg_lit1` is a helper for
 * required flags (where `mincount = 1` and `maxcount = 1`). While `arg_lit0`
 * and `arg_lit1` are available, it is recommended to use `arg_litn` in new
 * code as it is more explicit and flexible.
 *
 * Example usage:
 * ```
 * arg_lit_t *list    = arg_litn("lL",NULL,           0, 1, "list files");
 * arg_lit_t *verbose = arg_litn("v","verbose,debug", 0, 3, "verbosity level");
 * arg_lit_t *help    = arg_litn("h","help",          0, 1, "print this help");
 * arg_lit_t *version = arg_litn(NULL,"version",      0, 1, "print version info");
 * ```
 *
 * @param shortopts A string of single characters, each representing a short option
 *                  name (e.g., `"v"` for `-v`). Pass `NULL` if no short option is
 *                  desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"verbose"` for `--verbose`). Pass `NULL` if no long option is
 *                  desired.
 * @param mincount  The minimum number of times the flag must appear (set to 0 for
 *                  optional).
 * @param maxcount  The maximum number of times the flag can appear (controls memory
 *                  allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_lit_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_lit_t* arg_litn(const char* shortopts, const char* longopts, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_lit_t* arg_lit0(const char* shortopts, const char* longopts, const char* glossary);
ARG_EXTERN arg_lit_t* arg_lit1(const char* shortopts, const char* longopts, const char* glossary);

/**
 * Creates an integer argument for the command-line parser.
 *
 * The `arg_intn` function defines an option that accepts integer values from
 * the command line. You can specify the minimum and maximum number of times the
 * argument can appear, making it suitable for optional, required, or repeatable
 * integer options. Each occurrence of the option is parsed and stored in the
 * `ival` array of the resulting `arg_int_t` struct, allowing you to retrieve
 * all provided integer values after parsing.
 *
 * For convenience and backward compatibility, `arg_int0` is provided as a
 * helper for optional integer arguments (where `mincount = 0` and
 * `maxcount = 1`), and `arg_int1` is a helper for required integer arguments
 * (where `mincount = 1` and `maxcount = 1`). While `arg_int0` and `arg_int1`
 * are available, it is recommended to use `arg_intn` in new code as it is more
 * explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one or more integer arguments
 * arg_int_t *numbers = arg_intn("n", "number", "<int>", 1, 5, "Input numbers");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = { numbers, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && numbers->count > 0) {
 *     for (int i = 0; i < numbers->count; ++i) {
 *         printf("Input number: %d\n", numbers->ival[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"n"` for `-n`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"number"` for `--number`). Pass `NULL` if no long option is
 *                  desired.
 * @param datatype  A string describing the expected data type (e.g., `"<int>"`),
 *                  shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to 0
 *                  for optional).
 * @param maxcount  The maximum number of times the argument can appear (controls
 *                  memory allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_int_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_int_t* arg_intn(const char* shortopts, const char* longopts, const char* datatype, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_int_t* arg_int0(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);
ARG_EXTERN arg_int_t* arg_int1(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);

/**
 * Creates a double-precision floating-point argument for the command-line parser.
 *
 * The `arg_dbln` function defines an option that accepts double-precision
 * floating-point values from the command line. You can specify the minimum and
 * maximum number of times the argument can appear, making it suitable for
 * optional, required, or repeatable floating-point options. Each occurrence of
 * the option is parsed and stored in the `dval` array of the resulting
 * `arg_dbl_t` struct, allowing you to retrieve all provided double values after
 * parsing.
 *
 * For convenience and backward compatibility, `arg_dbl0` is provided as a
 * helper for optional double arguments (where `mincount = 0` and
 * `maxcount = 1`), and `arg_dbl1` is a helper for required double arguments
 * (where `mincount = 1` and `maxcount = 1`). While `arg_dbl0` and `arg_dbl1`
 * are available, it is recommended to use `arg_dbln` in new code as it is more
 * explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one or more double arguments
 * arg_dbl_t *values = arg_dbln("v", "value", "<double>", 1, 5, "Input values");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {values, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && values->count > 0) {
 *     for (int i = 0; i < values->count; ++i) {
 *         printf("Input value: %f\n", values->dval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"v"` for `-v`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"value"` for `--value`). Pass `NULL` if no long option is
 *                  desired.
 * @param datatype  A string describing the expected data type (e.g.,
 *                  `"<double>"`), shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to
 *                  0 for optional).
 * @param maxcount  The maximum number of times the argument can appear
 *                  (controls memory allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_dbl_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_dbl_t* arg_dbln(const char* shortopts, const char* longopts, const char* datatype, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_dbl_t* arg_dbl0(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);
ARG_EXTERN arg_dbl_t* arg_dbl1(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);

/**
 * Creates a string argument for the command-line parser.
 *
 * The `arg_strn` function defines an option that accepts string values from the
 * command line. You can specify the minimum and maximum number of times the
 * argument can appear, making it suitable for optional, required, or repeatable
 * string options. Each occurrence of the option is parsed and stored in the
 * `sval` array of the resulting `arg_str_t` struct, allowing you to retrieve
 * all provided string values after parsing.
 *
 * For convenience and backward compatibility, `arg_str0` is provided as a
 * helper for optional string arguments (where `mincount = 0` and
 * `maxcount = 1`), and `arg_str1` is a helper for required string arguments
 * (where `mincount = 1` and `maxcount = 1`). While `arg_str0` and `arg_str1`
 * are available, it is recommended to use `arg_strn` in new code as it is more
 * explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one or more string arguments
 * arg_str_t *inputs = arg_strn(NULL, NULL, "<input>", 1, 10, "Input strings");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = { inputs, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && inputs->count > 0) {
 *     for (int i = 0; i < inputs->count; ++i) {
 *         printf("Input string: %s\n", inputs->sval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"i"` for `-i`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"input"` for `--input`). Pass `NULL` if no long option is
 *                  desired.
 * @param datatype  A string describing the expected data type (e.g.,
 *                  `"<input>"`), shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to
 *                  0 for optional).
 * @param maxcount  The maximum number of times the argument can appear
 *                  (controls memory allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_str_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_str_t* arg_strn(const char* shortopts, const char* longopts, const char* datatype, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_str_t* arg_str0(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);
ARG_EXTERN arg_str_t* arg_str1(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);

/**
 * Creates a regular expression argument for the command-line parser.
 *
 * The `arg_rexn` function defines an option that accepts values matching a
 * specified regular expression pattern. You can specify the minimum and maximum
 * number of times the argument can appear, making it suitable for optional,
 * required, or repeatable regex-matched options. Each occurrence of the option
 * is parsed and stored in the `sval` array of the resulting `arg_rex_t` struct,
 * allowing you to retrieve all provided values that match the regular expression
 * after parsing.
 *
 * For convenience and backward compatibility, `arg_rex0` is provided as a
 * helper for optional regex arguments (where `mincount = 0` and `maxcount = 1`),
 * and `arg_rex1` is a helper for required regex arguments (where `mincount = 1`
 * and `maxcount = 1`). While `arg_rex0` and `arg_rex1` are available, it is
 * recommended to use `arg_rexn` in new code as it is more explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one or more arguments matching a simple email pattern
 * arg_rex_t *emails = arg_rexn(NULL, "email", "^[^@]+@[^@]+\\.[^@]+$", "<email>", 1, 10, 0, "Email addresses");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = { emails, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && emails->count > 0) {
 *     for (int i = 0; i < emails->count; ++i) {
 *         printf("Matched email: %s\n", emails->sval[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"e"` for `-e`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"email"` for `--email`). Pass `NULL` if no long option is
 *                  desired.
 * @param pattern   The regular expression pattern to match input values.
 * @param datatype  A string describing the expected data type (e.g.,
 *                  `"<email>"`), shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to
 *                  0 for optional).
 * @param maxcount  The maximum number of times the argument can appear
 *                  (controls memory allocation).
 * @param flags     Flags to modify regex matching behavior (e.g.,
 *                  `ARG_REX_ICASE` for case-insensitive).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_rex_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_rex_t* arg_rexn(const char* shortopts,
                               const char* longopts,
                               const char* pattern,
                               const char* datatype,
                               int mincount,
                               int maxcount,
                               int flags,
                               const char* glossary);
ARG_EXTERN arg_rex_t* arg_rex0(const char* shortopts,
                               const char* longopts,
                               const char* pattern,
                               const char* datatype,
                               int flags,
                               const char* glossary);
ARG_EXTERN arg_rex_t* arg_rex1(const char* shortopts,
                               const char* longopts,
                               const char* pattern,
                               const char* datatype,
                               int flags,
                               const char* glossary);

/**
 * Creates a file path argument for the command-line parser.
 *
 * The `arg_filen` function defines an option that accepts file path values from
 * the command line. You can specify the minimum and maximum number of times the
 * argument can appear, making it suitable for optional, required, or repeatable
 * file arguments. Each occurrence of the option is parsed and stored in the
 * `filename` array of the resulting `arg_file_t` struct. The struct also
 * provides convenient access to the basename and extension for each file.
 *
 * For convenience and backward compatibility, `arg_file0` is provided as a
 * helper for optional file arguments (where `mincount = 0` and `maxcount = 1`),
 * and `arg_file1` is a helper for required file arguments (where `mincount = 1`
 * and `maxcount = 1`). While `arg_file0` and `arg_file1` are available, it is
 * recommended to use `arg_filen` in new code as it is more explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one or more file arguments
 * arg_file_t *files = arg_filen(NULL, NULL, "<file>", 1, 100, "Input files");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = { files, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && files->count > 0) {
 *     for (int i = 0; i < files->count; ++i) {
 *         printf("File: %s, Basename: %s, Extension: %s\n",
 *             files->filename[i], files->basename[i], files->extension[i]);
 *     }
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"f"` for `-f`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"file"` for `--file`). Pass `NULL` if no long option is
 *                  desired.
 * @param datatype  A string describing the expected data type (e.g.,
 *                  `"<file>"`), shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to
 *                  0 for optional).
 * @param maxcount  The maximum number of times the argument can appear
 *                  (controls memory allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_file_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_file_t* arg_filen(const char* shortopts, const char* longopts, const char* datatype, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_file_t* arg_file0(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);
ARG_EXTERN arg_file_t* arg_file1(const char* shortopts, const char* longopts, const char* datatype, const char* glossary);

/**
 * Creates a date/time argument for the command-line parser.
 *
 * The `arg_daten` function defines an option that accepts date or time values
 * from the command line. You can specify the minimum and maximum number of
 * times the argument can appear, making it suitable for optional, required, or
 * repeatable date/time arguments. Each occurrence of the option is parsed using
 * the specified `format` (a `strptime`-style format string) and stored as a
 * `struct tm` value in the `tmval` array of the resulting `arg_date_t` struct,
 * allowing you to retrieve all provided date/time values after parsing.
 *
 * For convenience and backward compatibility, `arg_date0` is provided as a
 * helper for optional date arguments (where `mincount = 0` and `maxcount = 1`),
 * and `arg_date1` is a helper for required date arguments (where `mincount = 1`
 * and `maxcount = 1`). While `arg_date0` and `arg_date1` are available, it is
 * recommended to use `arg_daten` in new code as it is more explicit and flexible.
 *
 * Example usage:
 * ```
 * // Accepts one required date argument in YYYY-MM-DD format
 * arg_date_t *date = arg_date1(NULL, "date", "%Y-%m-%d", "<date>", "Date in YYYY-MM-DD format");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = { date, end };
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors == 0 && date->count > 0) {
 *     printf("Parsed date: %04d-%02d-%02d\n",
 *         date->tmval[0].tm_year + 1900, date->tmval[0].tm_mon + 1, date->tmval[0].tm_mday);
 * } else {
 *     arg_print_errors(stdout, end, argv[0]);
 * }
 * ```
 *
 * @param shortopts A string of single characters, each representing a short
 *                  option name (e.g., `"d"` for `-d`). Pass `NULL` if no short
 *                  option is desired.
 * @param longopts  A string of comma-separated long option names (e.g.,
 *                  `"date"` for `--date`). Pass `NULL` if no long option is
 *                  desired.
 * @param format    A `strptime`-style format string describing the expected
 *                  date/time input (e.g., `"%Y-%m-%d"`).
 * @param datatype  A string describing the expected data type (e.g.,
 *                  `"<date>"`), shown in help messages.
 * @param mincount  The minimum number of times the argument must appear (set to
 *                  0 for optional).
 * @param maxcount  The maximum number of times the argument can appear
 *                  (controls memory allocation).
 * @param glossary  A short description of the argument for the glossary/help
 *                  output. Pass `NULL` to omit.
 *
 * @return
 *   If successful, returns a pointer to the allocated `arg_date_t`. Returns
 *   `NULL` if there is insufficient memory.
 */
ARG_EXTERN arg_date_t*
arg_daten(const char* shortopts, const char* longopts, const char* format, const char* datatype, int mincount, int maxcount, const char* glossary);
ARG_EXTERN arg_date_t* arg_date0(const char* shortopts, const char* longopts, const char* format, const char* datatype, const char* glossary);
ARG_EXTERN arg_date_t* arg_date1(const char* shortopts, const char* longopts, const char* format, const char* datatype, const char* glossary);

/**
 * Creates an end-of-table marker and error collector for the argument table.
 *
 * The `arg_end` function is used to create an `arg_end_t` struct, which should
 * be placed as the last element in the argument table array. This structure
 * serves two purposes: it marks the end of the argument table for the parser,
 * and it collects information about any errors encountered during command-line
 * parsing.
 *
 * The `maxerrors` parameter specifies the maximum number of errors that can be
 * recorded. After parsing, the `arg_end_t` struct contains details about
 * missing required arguments, invalid values, or other parsing errors, which
 * can be reported to the user using functions like `arg_print_errors`.
 *
 * Example usage:
 * ```
 * arg_lit_t *help = arg_lit0("h", "help", "Display help");
 * arg_int_t *count = arg_int0("c", "count", "<n>", "Number of times");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {help, count, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors > 0) {
 *     arg_print_errors(stdout, end, argv[0]);
 *     // handle errors...
 * }
 * ```
 *
 * @param maxcount The maximum number of errors to record during parsing.
 *                 Choose a value large enough to capture all possible errors.
 * @return
 *   If successful, returns a pointer to the allocated `arg_end_t`. Returns
 *   `NULL` if there is insufficient memory.
 *
 * @see arg_end_t, arg_parse, arg_print_errors
 */
ARG_EXTERN arg_end_t* arg_end(int maxcount);

#define ARG_DSTR_STATIC ((arg_dstr_freefn*)0)
#define ARG_DSTR_VOLATILE ((arg_dstr_freefn*)1)
#define ARG_DSTR_DYNAMIC ((arg_dstr_freefn*)3)

/**** other functions *******************************************/

/**
 * Checks the argument table for null entries.
 *
 * The `arg_nullcheck` function scans the provided argument table array and
 * returns 1 if any entry is NULL, or 0 if all entries are valid. This is useful
 * for detecting memory allocation failures after constructing all argument
 * table entries with `arg_<type>` constructor functions, such as `arg_litn`.
 *
 * Instead of checking the return value of each constructor individually, you
 * can call `arg_nullcheck` once after building the argument table to ensure
 * that all arguments were allocated successfully. This helps make your code
 * cleaner and more robust.
 *
 * Example usage:
 * ```
 * arg_lit_t *list    = arg_lit0("lL",NULL,           "list files");
 * arg_lit_t *verbose = arg_lit0("v","verbose,debug", "verbose messages");
 * arg_lit_t *help    = arg_lit0(NULL,"help",         "print this help and exit");
 * arg_lit_t *version = arg_lit0(NULL,"version",      "print version and exit");
 * arg_end_t *end     = arg_end(20);
 * void *argtable[] = {list, verbose, help, version, end};
 * const char *progname = "myprog";
 * int exitcode = 0;
 *
 * if (arg_nullcheck(argtable) != 0) {
 *     printf("%s: insufficient memory\n", progname);
 *     exitcode = 1;
 *     goto exit;
 * }
 * ```
 *
 * @param argtable Array of pointers to argument table structs.
 *
 * @return Returns 1 if any entry is NULL, 0 if all entries are valid.
 */
ARG_EXTERN int arg_nullcheck(void** argtable);

/**
 * Parses the command-line arguments according to the specified argument table.
 *
 * The `arg_parse` function processes the command-line arguments provided in
 * `argv` and populates the fields of each argument structure in the `argtable`
 * array. It checks for the presence, validity, and value of each option or
 * positional argument as defined by the argument table. Any errors encountered
 * during parsing, such as missing required arguments or invalid values, are
 * recorded in the `arg_end_t` structure (typically the last entry in the table).
 *
 * After calling `arg_parse`, you can inspect the fields of each argument struct
 * (such as `count`, `ival`, `dval`, `sval`, etc.) to retrieve the parsed
 * values. If errors are detected (i.e., the return value is greater than zero),
 * you can use `arg_print_errors` to display detailed error messages to the user.
 *
 * Example usage:
 * ```
 * arg_lit_t *help = arg_lit0("h", "help", "Display help");
 * arg_int_t *count = arg_int0("c", "count", "<n>", "Number of times");
 * arg_end_t *end = arg_end(20);
 * void *argtable[] = {help, count, end};
 *
 * int nerrors = arg_parse(argc, argv, argtable);
 * if (nerrors > 0) {
 *     arg_print_errors(stdout, end, argv[0]);
 *     // handle errors...
 * }
 * ```
 *
 * @param argc     The number of command-line arguments passed to the program.
 *                 The value is always greater than or equal to 1.
 * @param argv     An array of null-terminated strings representing the
 *                 command-line arguments. By convention, `argv[0]` is the
 *                 program name, and `argv[1]` to `argv[argc-1]` are the
 *                 arguments. `argv[argc]` is always NULL.
 * @param argtable An array of pointers to argument table structs, each created
 *                 by an `arg_<type>` constructor. The last entry should be an
 *                 `arg_end` struct.
 *
 * @return The number of errors found during parsing. Returns 0 if parsing was
 *         successful and no errors were detected.
 */
ARG_EXTERN int arg_parse(int argc, char** argv, void** argtable);

/**
 * Prints a formatted command-line option specification to a file stream.
 *
 * The `arg_print_option` function generates a formatted representation of a
 * command-line option, including its short options, long options, data type,
 * and an optional suffix. This is useful for displaying option syntax in help
 * messages, usage output, or documentation.
 *
 * The formatted option is written to the specified file stream (`fp`). You can
 * control the appearance of the option specification, making it easy to
 * integrate with custom help or documentation systems.
 *
 * Example usage:
 * ```
 * // Print option to stdout
 * arg_print_option(stdout, "h", "help", NULL, NULL);
 * ```
 *
 * @param fp        Output file stream to write to (e.g., `stdout` or `stderr`).
 * @param shortopts String of short option characters (e.g., `"h"` for `-h`).
 * @param longopts  String of long option names, comma-separated (e.g., `"help"` for `--help`).
 * @param datatype  String describing the expected data type (e.g., `"<file>"`).
 * @param suffix    Optional string to append after the option specification.
 */
ARG_EXTERN void arg_print_option(FILE* fp, const char* shortopts, const char* longopts, const char* datatype, const char* suffix);

/**
 * Prints a formatted command-line option specification to a dynamic string.
 *
 * The `arg_print_option_ds` function generates a formatted representation of a
 * command-line option, including its short options, long options, data type,
 * and an optional suffix. This is useful for displaying option syntax in help
 * messages, usage output, or documentation that is built using dynamic string
 * buffers.
 *
 * The formatted option is written to the specified dynamic string object
 * (`arg_dstr_t`). You can control the appearance of the option specification,
 * making it easy to integrate with custom help or documentation systems that
 * require string-based output.
 *
 * Example usage:
 * ```
 * // Print option to a dynamic string
 * arg_dstr_t ds = arg_dstr_create();
 * arg_print_option_ds(ds, "v", "verbose", "<level>", NULL);
 * printf("%s", arg_dstr_cstr(ds));
 * arg_dstr_destroy(ds);
 * ```
 *
 * @param ds        Dynamic string object to write to.
 * @param shortopts String of short option characters (e.g., `"h"` for `-h`).
 * @param longopts  String of long option names, comma-separated (e.g., `"help"` for `--help`).
 * @param datatype  String describing the expected data type (e.g., `"<file>"`).
 * @param suffix    Optional string to append after the option specification.
 */
ARG_EXTERN void arg_print_option_ds(arg_dstr_t ds, const char* shortopts, const char* longopts, const char* datatype, const char* suffix);

/**
 * Prints a compact, single-line command-line syntax summary to a file stream.
 *
 * The `arg_print_syntax` function generates a concise, single-line usage
 * summary for the command-line options and arguments defined in the argument
 * table. This summary is useful for displaying quick usage information to
 * users, such as in the output of a `--help` or usage message.
 *
 * The formatted syntax summary is written to the specified file stream (`fp`).
 * You can append a custom `suffix` string to the end of the summary, for
 * example, to indicate positional arguments.
 *
 * Example usage:
 * ```
 * // Print compact syntax summary to stdout
 * arg_print_syntax(stdout, argtable, "[FILES...]");
 * ```
 *
 * @param fp      Output file stream to write to (e.g., `stdout` or `stderr`).
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param suffix  String to append at the end of the syntax summary (e.g., for
 *                positional arguments).
 */
ARG_EXTERN void arg_print_syntax(FILE* fp, void** argtable, const char* suffix);

/**
 * Prints a compact, single-line command-line syntax summary to a dynamic string.
 *
 * The `arg_print_syntax_ds` function generates a concise, single-line usage
 * summary for the command-line options and arguments defined in the argument
 * table. This summary is useful for displaying quick usage information to
 * users, such as in the output of a `--help` or usage message, and is written
 * to a dynamic string object (`arg_dstr_t`).
 *
 * You can append a custom `suffix` string to the end of the summary, for
 * example, to indicate positional arguments.
 *
 * Example usage:
 * ```
 * // Print compact syntax summary to a dynamic string
 * arg_dstr_t ds = arg_dstr_create();
 * arg_print_syntax_ds(ds, argtable, "[FILES...]");
 * printf("%s", arg_dstr_cstr(ds));
 * arg_dstr_destroy(ds);
 * ```
 *
 * @param ds      Dynamic string object to write to.
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param suffix  String to append at the end of the syntax summary (e.g., for
 *                positional arguments).
 */
ARG_EXTERN void arg_print_syntax_ds(arg_dstr_t ds, void** argtable, const char* suffix);

/**
 * Prints a verbose, multi-line command-line syntax summary to a file stream.
 *
 * The `arg_print_syntaxv` function generates a detailed, multi-line usage
 * summary for the command-line options and arguments defined in the argument
 * table. This verbose style provides more clarity than the compact single-line
 * form, making it easier for users to understand complex command-line
 * interfaces.
 *
 * The formatted syntax summary is written to the specified file stream (`fp`).
 * You can append a custom `suffix` string to the end of the summary, for
 * example, to indicate positional arguments.
 *
 * Example usage:
 * ```
 * // Print verbose syntax summary to stdout
 * arg_print_syntaxv(stdout, argtable, "[FILES...]");
 * ```
 *
 * @param fp      Output file stream to write to (e.g., `stdout` or `stderr`).
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param suffix  String to append at the end of the syntax summary (e.g., for
 *                positional arguments).
 */
ARG_EXTERN void arg_print_syntaxv(FILE* fp, void** argtable, const char* suffix);

/**
 * Prints a verbose, multi-line command-line syntax summary to a dynamic string.
 *
 * The `arg_print_syntaxv_ds` function generates a detailed, multi-line usage
 * summary for the command-line options and arguments defined in the argument
 * table. This verbose style provides more clarity than the compact single-line
 * form, making it easier for users to understand complex command-line
 * interfaces.
 *
 * The formatted syntax summary is written to the specified dynamic string
 * object (`arg_dstr_t`). You can append a custom `suffix` string to the end of
 * the summary, for example, to indicate positional arguments.
 *
 * Example usage:
 * ```
 * // Print verbose syntax summary to a dynamic string
 * arg_dstr_t ds = arg_dstr_create();
 * arg_print_syntaxv_ds(ds, argtable, "[FILES...]");
 * printf("%s", arg_dstr_cstr(ds));
 * arg_dstr_destroy(ds);
 * ```
 *
 * @param ds      Dynamic string object to write to.
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param suffix  String to append at the end of the syntax summary (e.g., for
 *                positional arguments).
 */
ARG_EXTERN void arg_print_syntaxv_ds(arg_dstr_t ds, void** argtable, const char* suffix);

/**
 * Prints the argument glossary in a customizable format to a file stream.
 *
 * The `arg_print_glossary` function generates a glossary of command-line
 * options and arguments, formatted according to the specified format string.
 * This glossary provides users with a summary of available options, their data
 * types, and descriptions, making it easier to understand the command-line
 * interface.
 *
 * The formatted glossary is written to the specified file stream (`fp`). The
 * format string allows you to control the layout and appearance of each
 * glossary entry, enabling integration with custom help or documentation
 * systems.
 *
 * Example usage:
 * ```
 * // Print glossary to stdout with a custom format
 * arg_print_glossary(stdout, argtable, "  %-20s %s\n");
 * ```
 *
 * @param fp      Output file stream to write to (e.g., `stdout` or `stderr`).
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param format  Format string controlling the layout of each glossary entry.
 */
ARG_EXTERN void arg_print_glossary(FILE* fp, void** argtable, const char* format);

/**
 * Prints the argument glossary in a customizable format to a dynamic string.
 *
 * The `arg_print_glossary_ds` function generates a glossary of command-line
 * options and arguments, formatted according to the specified format string.
 * This glossary provides users with a summary of available options, their data
 * types, and descriptions, making it easier to understand the command-line
 * interface.
 *
 * The formatted glossary is written to the specified dynamic string object
 * (`arg_dstr_t`). The format string allows you to control the layout and
 * appearance of each glossary entry, enabling integration with custom help or
 * documentation systems.
 *
 * Example usage:
 * ```
 * // Print glossary to a dynamic string with a custom format
 * arg_dstr_t ds = arg_dstr_create();
 * arg_print_glossary_ds(ds, argtable, "  %-20s %s\n");
 * printf("%s", arg_dstr_cstr(ds));
 * arg_dstr_destroy(ds);
 * ```
 *
 * @param ds      Dynamic string object to write to.
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 * @param format  Format string controlling the layout of each glossary entry.
 */
ARG_EXTERN void arg_print_glossary_ds(arg_dstr_t ds, void** argtable, const char* format);

/**
 * Prints the argument glossary using strict GNU formatting conventions to a file
 * stream.
 *
 * The `arg_print_glossary_gnu` function generates a glossary of command-line
 * options and arguments, formatted to comply with GNU style conventions. In this
 * format, long options are vertically aligned in a second column, and lines are
 * wrapped at 80 characters for improved readability and consistency with GNU
 * command-line tool documentation.
 *
 * The formatted glossary is written to the specified file stream (`fp`). This
 * function is useful for generating help output that matches the look and feel
 * of standard GNU utilities.
 *
 * Example usage:
 * ```
 * // Print GNU-style glossary to stdout
 * arg_print_glossary_gnu(stdout, argtable);
 * ```
 *
 * @param fp      Output file stream to write to (e.g., `stdout` or `stderr`).
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 */
ARG_EXTERN void arg_print_glossary_gnu(FILE* fp, void** argtable);

/**
 * Prints the argument glossary using strict GNU formatting conventions to a
 * dynamic string.
 *
 * The `arg_print_glossary_gnu_ds` function generates a glossary of command-line
 * options and arguments, formatted to comply with GNU style conventions. In
 * this format, long options are vertically aligned in a second column, and
 * lines are wrapped at 80 characters for improved readability and consistency
 * with GNU command-line tool documentation.
 *
 * The formatted glossary is written to the specified dynamic string object
 * (`arg_dstr_t`). This function is useful for generating help output that
 * matches the look and feel of standard GNU utilities, especially when you need
 * the output as a string for further processing, logging, or GUI display.
 *
 * Example usage:
 * ```
 * // Print GNU-style glossary to a dynamic string
 * arg_dstr_t ds = arg_dstr_create();
 * arg_print_glossary_gnu_ds(ds, argtable);
 * printf("%s", arg_dstr_cstr(ds));
 * arg_dstr_destroy(ds);
 * ```
 *
 * @param ds      Dynamic string object to write to.
 * @param argtable Array of argument table structs describing the available
 *                 options and arguments.
 */
ARG_EXTERN void arg_print_glossary_gnu_ds(arg_dstr_t ds, void** argtable);

ARG_EXTERN void arg_print_errors(FILE* fp, struct arg_end* end, const char* progname);
ARG_EXTERN void arg_print_errors_ds(arg_dstr_t ds, struct arg_end* end, const char* progname);

ARG_EXTERN void arg_print_formatted(FILE* fp, const unsigned lmargin, const unsigned rmargin, const char* text);

/**
 * Deallocates or frees non-null entries of the argument table.
 *
 * @param argtable An array of argument table structs.
 * @param n The number of structs in the argument table.
 */
ARG_EXTERN void arg_freetable(void** argtable, size_t n);

ARG_EXTERN arg_dstr_t arg_dstr_create(void);
ARG_EXTERN void arg_dstr_destroy(arg_dstr_t ds);
ARG_EXTERN void arg_dstr_reset(arg_dstr_t ds);
ARG_EXTERN void arg_dstr_free(arg_dstr_t ds);
ARG_EXTERN void arg_dstr_set(arg_dstr_t ds, char* str, arg_dstr_freefn* free_proc);
ARG_EXTERN void arg_dstr_cat(arg_dstr_t ds, const char* str);
ARG_EXTERN void arg_dstr_catc(arg_dstr_t ds, char c);
ARG_EXTERN void arg_dstr_catf(arg_dstr_t ds, const char* fmt, ...);
ARG_EXTERN char* arg_dstr_cstr(arg_dstr_t ds);

ARG_EXTERN void arg_cmd_init(void);
ARG_EXTERN void arg_cmd_uninit(void);
ARG_EXTERN void arg_cmd_register(const char* name, arg_cmdfn* proc, const char* description, void* ctx);
ARG_EXTERN void arg_cmd_unregister(const char* name);
ARG_EXTERN int arg_cmd_dispatch(const char* name, int argc, char* argv[], arg_dstr_t res);
ARG_EXTERN unsigned int arg_cmd_count(void);
ARG_EXTERN arg_cmd_info_t* arg_cmd_info(const char* name);
ARG_EXTERN arg_cmd_itr_t arg_cmd_itr_create(void);
ARG_EXTERN void arg_cmd_itr_destroy(arg_cmd_itr_t itr);
ARG_EXTERN int arg_cmd_itr_advance(arg_cmd_itr_t itr);
ARG_EXTERN char* arg_cmd_itr_key(arg_cmd_itr_t itr);
ARG_EXTERN arg_cmd_info_t* arg_cmd_itr_value(arg_cmd_itr_t itr);
ARG_EXTERN int arg_cmd_itr_search(arg_cmd_itr_t itr, void* k);
ARG_EXTERN void arg_mgsort(void* data, int size, int esize, int i, int k, arg_comparefn* comparefn);
ARG_EXTERN void arg_make_get_help_msg(arg_dstr_t res);
ARG_EXTERN void arg_make_help_msg(arg_dstr_t ds, const char* cmd_name, void** argtable);
ARG_EXTERN void arg_make_syntax_err_msg(arg_dstr_t ds, void** argtable, struct arg_end* end);
ARG_EXTERN int
arg_make_syntax_err_help_msg(arg_dstr_t ds, const char* name, int help, int nerrors, void** argtable, struct arg_end* end, int* exitcode);
ARG_EXTERN void arg_set_module_name(const char* name);
ARG_EXTERN void arg_set_module_version(int major, int minor, int patch, const char* tag);

/**** deprecated functions, for back-compatibility only ********/

/**
 * arg_free() is deprecated in favour of arg_freetable() due to a flaw in its design.
 * The flaw results in memory leak in the (very rare) case that an intermediate
 * entry in the argtable array failed its memory allocation while others following
 * that entry were still allocated ok. Those subsequent allocations will not be
 * deallocated by arg_free().
 * Despite the unlikeliness of the problem occurring, and the even unlikelier event
 * that it has any deliterious effect, it is fixed regardless by replacing arg_free()
 * with the newer arg_freetable() function.
 * We still keep arg_free() for backwards compatibility.
 */
ARG_EXTERN void arg_free(void** argtable);

#ifdef __cplusplus
}
#endif
#endif
