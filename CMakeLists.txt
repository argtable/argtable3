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
file(STRINGS "version.txt" ARGTABLE3_VERSION)

project(${ARGTABLE3_PROJECT_NAME} VERSION "${ARGTABLE3_VERSION}")

option(ARGTABLE3_ENABLE_CONAN "Enable Conan dependency manager" OFF)
option(ARGTABLE3_ENABLE_TESTS "Enable unit tests" ON)
option(ARGTABLE3_USE_AMALGAMATION "Use the amalgamation distribution" OFF)

if(ARGTABLE3_ENABLE_CONAN AND EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  conan_basic_setup(TARGETS)
  string(REPLACE ";" ":" LINK_FLAGS "${CONAN_LIB_DIRS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
endif()

if(ARGTABLE3_ENABLE_TESTS)
  enable_testing()
endif()

if(ARGTABLE3_USE_AMALGAMATION)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/dist/argtable3.c")
    execute_process(
      COMMAND "${CMAKE_SOURCE_DIR}/tools/build" dist
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools"
      )
  endif()
  
  set(ARGTABLE3_SRC_FILES
    ${CMAKE_SOURCE_DIR}/dist/argtable3.c
  )
else()
  set(ARGTABLE3_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/arg_cmd.c
    ${CMAKE_SOURCE_DIR}/src/arg_date.c
    ${CMAKE_SOURCE_DIR}/src/arg_dbl.c
    ${CMAKE_SOURCE_DIR}/src/arg_dstr.c
    ${CMAKE_SOURCE_DIR}/src/arg_end.c
    ${CMAKE_SOURCE_DIR}/src/arg_file.c
    ${CMAKE_SOURCE_DIR}/src/arg_hashtable.c
    ${CMAKE_SOURCE_DIR}/src/arg_int.c
    ${CMAKE_SOURCE_DIR}/src/arg_lit.c
    ${CMAKE_SOURCE_DIR}/src/arg_rem.c
    ${CMAKE_SOURCE_DIR}/src/arg_rex.c
    ${CMAKE_SOURCE_DIR}/src/arg_str.c
    ${CMAKE_SOURCE_DIR}/src/arg_utils.c
    ${CMAKE_SOURCE_DIR}/src/argtable3.c
    ${CMAKE_SOURCE_DIR}/src/getopt_long.c
  )
endif()

# Platform specific settings for installation
if(UNIX)
  include(GNUInstallDirs)
  set(ARGTABLE3_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
  set(ARGTABLE3_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
  set(ARGTABLE3_INSTALL_CMAKEDIR ${ARGTABLE3_LIBDIR}/cmake)
elseif(WIN32)
  set(ARGTABLE3_INSTALL_LIBDIR "lib")
  set(ARGTABLE3_INSTALL_INCLUDEDIR "include")
  set(ARGTABLE3_INSTALL_CMAKEDIR "cmake")
endif(UNIX)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
