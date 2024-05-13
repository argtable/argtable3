/*******************************************************************************
 * arg_csi: Implements the comma separated integer command-line option
 *
 * This file is part of the argtable3 library.
 *
 * Copyright (C) 1998-2001,2003-2011,2013 Stewart Heitmann
 * <sheitmann@users.sourceforge.net>
 * All rights reserved.
 * Copyright 2023 Morse Micro
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

#include "argtable3.h"

#ifndef ARG_AMALGAMATION
#include "argtable3_private.h"
#endif

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static void arg_csi_resetfn(struct arg_csi* parent) {
    ARG_TRACE(("%s:resetfn(%p)\n", __FILE__, parent));
    parent->count = 0;
}

static int arg_csi_scanfn(struct arg_csi* parent, const char* argval) {
    int errorcode = 0;
    const char *head = argval;

    if (parent->count >= parent->hdr.maxcount) {
        /* maximum number of arguments exceeded */
        errorcode = ARG_ERR_MAXCOUNT;
    } else if (!argval) {
        /* a valid argument with no argument value was given. */
        /* This happens when an optional argument value was invoked. */
        /* leave parent argument value unaltered but still count the argument. */
    } else {
        int num_vals;
        for (num_vals = 0; num_vals < parent->num_vals; num_vals++) {
            long int val;
            const char* end;

            if (!head || head[0] == '\0') {
                return ARG_ERR_NOTENOUGH;
            }

            /* attempt to extract hex integer (eg: +0x123) from argval into val conversion */
            val = strtol0X(head, &end, 'X', 16);
            if (end == head) {
                /* hex failed, attempt octal conversion (eg +0o123) */
                val = strtol0X(head, &end, 'O', 8);
                if (end == head) {
                    /* octal failed, attempt binary conversion (eg +0B101) */
                    val = strtol0X(head, &end, 'B', 2);
                    if (end == head) {
                        /* binary failed, attempt decimal conversion with no prefix (eg 1234) */
                        val = strtol(head, (char**)&end, 10);
                        if (end == head) {
                            /* all supported number formats failed */
                            return ARG_ERR_BADINT;
                        }
                    }
                }
            }

            /* Safety check for integer overflow. WARNING: this check    */
            /* achieves nothing on machines where size(int)==size(long). */
            if (val > INT_MAX || val < INT_MIN)
                return ARG_ERR_OVERFLOW;

            /* if success then store result in parent->ival[] array */
            if (errorcode == 0) {
                parent->ival[parent->count][num_vals] = (int)val;
                head = strchr(head, ',');
                if (head)
                    head++;
            }
        }
    }

    /* if we still have a head, there are too many commas and therefore params. */
    if (head)
        errorcode = ARG_ERR_TOOMANY;

    if (errorcode == 0)
        parent->count++;

    /* printf("%s:scanfn(%p,%p) returns %d\n",__FILE__,parent,argval,errorcode); */
    return errorcode;
}

static int arg_csi_checkfn(struct arg_csi* parent) {
    int errorcode = (parent->count < parent->hdr.mincount) ? ARG_ERR_MINCOUNT : 0;
    /*printf("%s:checkfn(%p) returns %d\n",__FILE__,parent,errorcode);*/
    return errorcode;
}

static void arg_csi_errorfn(struct arg_csi* parent, arg_dstr_t ds, int errorcode, const char* argval, const char* progname) {
    const char* shortopts = parent->hdr.shortopts;
    const char* longopts = parent->hdr.longopts;
    const char* datatype = parent->hdr.datatype;

    /* make argval NULL safe */
    argval = argval ? argval : "";

    arg_dstr_catf(ds, "%s: ", progname);
    switch (errorcode) {
        case ARG_ERR_MINCOUNT:
            arg_dstr_cat(ds, "missing option ");
            arg_print_option_ds(ds, shortopts, longopts, datatype, "\n");
            break;

        case ARG_ERR_MAXCOUNT:
            arg_dstr_cat(ds, "excess option ");
            arg_print_option_ds(ds, shortopts, longopts, argval, "\n");
            break;

        case ARG_ERR_BADINT:
            arg_dstr_catf(ds, "invalid argument \"%s\" to option ", argval);
            arg_print_option_ds(ds, shortopts, longopts, datatype, "\n");
            break;

        case ARG_ERR_OVERFLOW:
            arg_dstr_cat(ds, "integer overflow at option ");
            arg_print_option_ds(ds, shortopts, longopts, datatype, " ");
            arg_dstr_catf(ds, "(%s is too large)\n", argval);
            break;

        case ARG_ERR_NOTENOUGH:
            arg_dstr_cat(ds, "not enough comma separated values seen ");
            arg_print_option_ds(ds, shortopts, longopts, argval, "\n");
            arg_dstr_catf(ds, "(need %d)\n", parent->num_vals);
            break;

        case ARG_ERR_TOOMANY:
            arg_dstr_cat(ds, "too many comma separated values seen ");
            arg_print_option_ds(ds, shortopts, longopts, argval, "\n");
            arg_dstr_catf(ds, "(need %d)\n", parent->num_vals);
            break;
    }
}

struct arg_csi* arg_csi0(const char* shortopts, const char* longopts, const char* datatype, int num_vals, const char* glossary) {
    return arg_csin(shortopts, longopts, datatype, num_vals, 0, 1, glossary);
}

struct arg_csi* arg_csi1(const char* shortopts, const char* longopts, const char* datatype, int num_vals, const char* glossary) {
    return arg_csin(shortopts, longopts, datatype, num_vals, 1, 1, glossary);
}

struct arg_csi* arg_csin(const char* shortopts, const char* longopts, const char* datatype, int num_vals, int mincount, int maxcount, const char* glossary) {
    size_t nbytes;
    struct arg_csi* result;
    int i;
    int *data_start;

    /* foolproof things by ensuring maxcount is not less than mincount */
    maxcount = (maxcount < mincount) ? mincount : maxcount;

    nbytes = sizeof(struct arg_csi)    /* storage for struct arg_csi */
             + ((size_t)maxcount * sizeof(int*)) /* storage for ival[maxcount] */
             + ((size_t)maxcount * (size_t)num_vals * sizeof(int)); /* storage for ival[maxcount][num_vals] array */

    result = (struct arg_csi*)xmalloc(nbytes);

    /* init the arg_hdr struct */
    result->hdr.flag = ARG_HASVALUE;
    result->hdr.shortopts = shortopts;
    result->hdr.longopts = longopts;
    result->hdr.datatype = datatype ? datatype : "<int>,...";
    result->hdr.glossary = glossary;
    result->hdr.mincount = mincount;
    result->hdr.maxcount = maxcount;
    result->hdr.parent = result;
    result->hdr.resetfn = (arg_resetfn*)arg_csi_resetfn;
    result->hdr.scanfn = (arg_scanfn*)arg_csi_scanfn;
    result->hdr.checkfn = (arg_checkfn*)arg_csi_checkfn;
    result->hdr.errorfn = (arg_errorfn*)arg_csi_errorfn;

    /* store the ival[maxcount] array immediately after the arg_csi struct */
    result->ival = (int**)(result + 1);
    /* store the data after the pointers to each var */
    data_start = (int*)&result->ival[maxcount];

    for (i = 0; i < maxcount; i++) {
        result->ival[i] = &data_start[i * num_vals];
    }

    result->count = 0;
    result->num_vals = num_vals;

    ARG_TRACE(("arg_csin() returns %p\n", result));
    return result;
}
