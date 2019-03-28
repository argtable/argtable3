################################################################################
# This file is part of the argtable3 library.
#
# Copyright (C) 2019 Tom G. Huang
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

cmake_minimum_required(VERSION 3.0.0)

file(STRINGS "version.txt" VERSION)
project(argtable3 VERSION "${VERSION}")

option(CONAN "Enable Conan dependency manager" ON)
option(BUILD_SHARED_LIBS "Build shared library when enabled, static otherwise" ON)
option(argtable3_build_tests "Build argtable3 unit tests." ON)
option(argtable3_build_examples "Build argtable3 examples." ON)

if (CONAN AND EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
  conan_basic_setup(TARGETS)
  string(REPLACE ";" ":" LINK_FLAGS "${CONAN_LIB_DIRS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${LINK_FLAGS}")
endif ()

add_subdirectory(src)

if(argtable3_build_tests)
  enable_testing()
  add_subdirectory(tests)
endif()

if(argtable3_build_examples)
  add_subdirectory(examples)
endif()

install(EXPORT ${PROJECT_NAME}-config DESTINATION lib/cmake/${PROJECT_NAME})

