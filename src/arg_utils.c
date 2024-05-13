/*******************************************************************************
 * arg_utils: Implements memory, panic, and other utility functions
 *
 * This file is part of the argtable3 library.
 *
 * Copyright (C) 2013-2019 Tom G. Huang
 * <tomghuang@gmail.com>
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

#include "argtable3.h"

#ifndef ARG_AMALGAMATION
#include "argtable3_private.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void panic(const char* fmt, ...);
static arg_panicfn* s_panic = panic;

void dbg_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

static void panic(const char* fmt, ...) {
    va_list args;
    char* s;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    s = getenv("EF_DUMPCORE");
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

    if (s != NULL && *s != '\0') {
        abort();
    } else {
        exit(EXIT_FAILURE);
    }
}

void arg_set_panic(arg_panicfn* proc) {
    s_panic = proc;
}

void* xmalloc(size_t size) {
    void* ret = malloc(size);
    if (!ret) {
        s_panic("Out of memory!\n");
    }
    return ret;
}

void* xcalloc(size_t count, size_t size) {
    size_t allocated_count = count && size ? count : 1;
    size_t allocated_size = count && size ? size : 1;
    void* ret = calloc(allocated_count, allocated_size);
    if (!ret) {
        s_panic("Out of memory!\n");
    }
    return ret;
}

void* xrealloc(void* ptr, size_t size) {
    size_t allocated_size = size ? size : 1;
    void* ret = realloc(ptr, allocated_size);
    if (!ret) {
        s_panic("Out of memory!\n");
    }
    return ret;
}

void xfree(void* ptr) {
    free(ptr);
}

static void merge(void* data, int esize, int i, int j, int k, arg_comparefn* comparefn) {
    char* a = (char*)data;
    char* m;
    int ipos, jpos, mpos;

    /* Initialize the counters used in merging. */
    ipos = i;
    jpos = j + 1;
    mpos = 0;

    /* Allocate storage for the merged elements. */
    m = (char*)xmalloc((size_t)(esize * ((k - i) + 1)));

    /* Continue while either division has elements to merge. */
    while (ipos <= j || jpos <= k) {
        if (ipos > j) {
            /* The left division has no more elements to merge. */
            while (jpos <= k) {
                memcpy(&m[mpos * esize], &a[jpos * esize], (size_t)esize);
                jpos++;
                mpos++;
            }

            continue;
        } else if (jpos > k) {
            /* The right division has no more elements to merge. */
            while (ipos <= j) {
                memcpy(&m[mpos * esize], &a[ipos * esize], (size_t)esize);
                ipos++;
                mpos++;
            }

            continue;
        }

        /* Append the next ordered element to the merged elements. */
        if (comparefn(&a[ipos * esize], &a[jpos * esize]) < 0) {
            memcpy(&m[mpos * esize], &a[ipos * esize], (size_t)esize);
            ipos++;
            mpos++;
        } else {
            memcpy(&m[mpos * esize], &a[jpos * esize], (size_t)esize);
            jpos++;
            mpos++;
        }
    }

    /* Prepare to pass back the merged data. */
    memcpy(&a[i * esize], m, (size_t)(esize * ((k - i) + 1)));
    xfree(m);
}

void arg_mgsort(void* data, int size, int esize, int i, int k, arg_comparefn* comparefn) {
    int j;

    /* Stop the recursion when no more divisions can be made. */
    if (i < k) {
        /* Determine where to divide the elements. */
        j = (int)(((i + k - 1)) / 2);

        /* Recursively sort the two divisions. */
        arg_mgsort(data, size, esize, i, j, comparefn);
        arg_mgsort(data, size, esize, j + 1, k, comparefn);
        merge(data, esize, i, j, k, comparefn);
    }
}

/* strtol0x() is like strtol() except that the numeric string is    */
/* expected to be prefixed by "0X" where X is a user supplied char. */
/* The string may optionally be prefixed by white space and + or -  */
/* as in +0X123 or -0X123.                                          */
/* Once the prefix has been scanned, the remainder of the numeric   */
/* string is converted using strtol() with the given base.          */
/* eg: to parse hex str="-0X12324", specify X='X' and base=16.      */
/* eg: to parse oct str="+0o12324", specify X='O' and base=8.       */
/* eg: to parse bin str="-0B01010", specify X='B' and base=2.       */
/* Failure of conversion is indicated by result where *endptr==str. */
long int strtol0X(const char* str, const char** endptr, char X, int base) {
    long int val;          /* stores result */
    int s = 1;             /* sign is +1 or -1 */
    const char* ptr = str; /* ptr to current position in str */

    /* skip leading whitespace */
    while (isspace((int)(*ptr)))
        ptr++;
    /* printf("1) %s\n",ptr); */

    /* scan optional sign character */
    switch (*ptr) {
        case '+':
            ptr++;
            s = 1;
            break;
        case '-':
            ptr++;
            s = -1;
            break;
        default:
            s = 1;
            break;
    }
    /* printf("2) %s\n",ptr); */

    /* '0X' prefix */
    if ((*ptr++) != '0') {
        /* printf("failed to detect '0'\n"); */
        *endptr = str;
        return 0;
    }
    /* printf("3) %s\n",ptr); */
    if (toupper(*ptr++) != toupper(X)) {
        /* printf("failed to detect '%c'\n",X); */
        *endptr = str;
        return 0;
    }
    /* printf("4) %s\n",ptr); */

    /* attempt conversion on remainder of string using strtol() */
    val = strtol(ptr, (char**)endptr, base);
    if (*endptr == ptr) {
        /* conversion failed */
        *endptr = str;
        return 0;
    }

    /* success */
    return s * val;
}
