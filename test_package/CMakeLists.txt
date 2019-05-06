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

project(test_conanpkg)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

if(UNIX)
  set(ARGTABLE3_EXTRA_LIBS m)
endif()

enable_testing()

if (CONAN_SETTINGS_OS)
  find_package(Argtable3 REQUIRED)
  add_executable(${PROJECT_NAME} testargtable3.c)
  target_link_libraries(${PROJECT_NAME} PUBLIC argtable3 ${ARGTABLE3_EXTRA_LIBS})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:argtable3>"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
  )

  add_test(NAME ${PROJECT_NAME} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} "$<TARGET_FILE:${PROJECT_NAME}>" --help)
else ()
  add_subdirectory("${CONAN_ARGTABLE3_ROOT}/src" "${CMAKE_CURRENT_BINARY_DIR}/argtable3")

  # Compiled library should exist and be a target in the current CMake project
  add_test(NAME lib_exists COMMAND "${CMAKE_COMMAND}" -E md5sum "$<TARGET_FILE:argtable3>")
endif ()
