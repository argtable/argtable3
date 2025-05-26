/*******************************************************************************
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

#ifndef APP_H_60753EEF_6B02_4514_BF13_59B4C923AB5A_INCLUDED
#define APP_H_60753EEF_6B02_4514_BF13_59B4C923AB5A_INCLUDED

#define APP_NAME "multicmd"
#define APP_VER_MAJOR 1
#define APP_VER_MINOR 0
#define APP_VER_PATCH 0
#define APP_VER_TAG "alpha"

typedef enum app_error {
    APP_OK,
    APP_FAIL,
    APP_ERR_OUTOFMEMORY,
    APP_ERR_UNKNOWN_CMD,
    APP_LAST_ERROR_CODE
} app_error_t;

const char *app_strerror(app_error_t code);

#endif  // #ifndef APP_H_60753EEF_6B02_4514_BF13_59B4C923AB5A_INCLUDED
