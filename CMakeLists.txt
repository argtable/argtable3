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

cmake_minimum_required(VERSION 3.0)

set(ARGTABLE3_PROJECT_NAME "argtable3")
set(ARGTABLE3_PACKAGE_NAME "Argtable3")

project(${ARGTABLE3_PROJECT_NAME})

option(ARGTABLE3_ENABLE_CONAN "Enable Conan dependency manager" OFF)
option(ARGTABLE3_ENABLE_TESTS "Enable unit tests" ON)
option(ARGTABLE3_ENABLE_ARG_REX_DEBUG "Enable arg_rex debug output" OFF)
option(ARGTABLE3_BUILD_STATIC_EXAMPLES "Build examples with the static argtable3 library" OFF)

get_filename_component(VERSION_TAG_PATH "version.tag" ABSOLUTE)
if(EXISTS ${VERSION_TAG_PATH})
  file(READ version.tag VERSION_TAG OFFSET 1)
  string(REPLACE "." ";" VERSION_LIST ${VERSION_TAG})
  list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
  list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
  list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)
  list(GET VERSION_LIST 3 PROJECT_VERSION_BUILD)
else()
  set(PROJECT_VERSION_MAJOR 0)
  set(PROJECT_VERSION_MINOR 0)
  set(PROJECT_VERSION_PATCH 0)
  set(PROJECT_VERSION_BUILD "mainline")
endif()
set(PROJECT_VERSION_TWEAK 0)
set(ARGTABLE3_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})
set(ARGTABLE3_FULL_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD})

if(ARGTABLE3_ENABLE_CONAN AND EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  conan_basic_setup()
  string(REPLACE ";" ":" LINK_FLAGS "${CONAN_LIB_DIRS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
endif()

set(ARGTABLE3_AMALGAMATION_SRC_FILE ${PROJECT_SOURCE_DIR}/dist/argtable3.c)
set(ARGTABLE3_SRC_FILES
  ${PROJECT_SOURCE_DIR}/src/arg_cmd.c
  ${PROJECT_SOURCE_DIR}/src/arg_date.c
  ${PROJECT_SOURCE_DIR}/src/arg_dbl.c
  ${PROJECT_SOURCE_DIR}/src/arg_dstr.c
  ${PROJECT_SOURCE_DIR}/src/arg_end.c
  ${PROJECT_SOURCE_DIR}/src/arg_file.c
  ${PROJECT_SOURCE_DIR}/src/arg_hashtable.c
  ${PROJECT_SOURCE_DIR}/src/arg_int.c
  ${PROJECT_SOURCE_DIR}/src/arg_lit.c
  ${PROJECT_SOURCE_DIR}/src/arg_rem.c
  ${PROJECT_SOURCE_DIR}/src/arg_rex.c
  ${PROJECT_SOURCE_DIR}/src/arg_str.c
  ${PROJECT_SOURCE_DIR}/src/arg_utils.c
  ${PROJECT_SOURCE_DIR}/src/argtable3.c
  ${PROJECT_SOURCE_DIR}/src/getopt_long.c
)

# Platform specific settings for installation
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

add_subdirectory(src)
add_subdirectory(examples)

if(ARGTABLE3_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
