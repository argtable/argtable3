################################################################################
# This file is part of the argtable3 library.
#
# Copyright (C) 2016-2019 Tom G. Huang
# <tomghuang@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of STEWART HEITMANN nor the  names of its contributors
#       may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL STEWART HEITMANN BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

cmake_minimum_required(VERSION 2.8.0)
project(tests)

include_directories(
  ${PROJECT_SOURCE_DIR}
)

set(TEST_SRC_FILES
  testall.c
  testarglit.c
  testargstr.c
  testargint.c
  testargdate.c
  testargdbl.c
  testargfile.c
  testargrex.c
  testarghashtable.c
  testargdstr.c
  testargcmd.c
  CuTest.c
  ../src/arg_cmd.c
  ../src/arg_date.c
  ../src/arg_dbl.c
  ../src/arg_dstr.c
  ../src/arg_end.c
  ../src/arg_file.c
  ../src/arg_hashtable.c
  ../src/arg_int.c
  ../src/arg_lit.c
  ../src/arg_rem.c
  ../src/arg_rex.c
  ../src/arg_str.c
  ../src/arg_utils.c
  ../src/argtable3.c
  ../src/getopt_long.c
)

add_executable(testall ${TEST_SRC_FILES})
target_include_directories(testall PRIVATE ../src)
add_test(NAME testall COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} "$<TARGET_FILE:testall>")
