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

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4820")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4710")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4255")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd5045")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4668")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4711")
else(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

if(ARGTABLE3_ENABLE_ARG_REX_DEBUG)
  add_definitions(-DARG_REX_DEBUG)
endif()

add_definitions(-D_XOPEN_SOURCE=700)

if(WIN32)
  set(COMPANY_NAME "The Argtable3 Project")
  set(FILE_DESC "ANSI C command-line parsing library")
  set(INTERNAL_NAME "${PROJECT_NAME}")
  set(LEGAL_COPYRIGHT "Copyright (C) 1998-2001,2003-2011,2013 Stewart Heitmann. All rights reserved.")
  set(ORIGINAL_FILE_NAME "${PROJECT_NAME}")
  set(PRODUCT_NAME "${PROJECT_NAME}")
  configure_file(
    "${PROJECT_SOURCE_DIR}/src/version.rc.in"
    "${PROJECT_BINARY_DIR}/src/version.rc"
  )
  add_library(argtable3 SHARED ${ARGTABLE3_SRC_FILES} "${PROJECT_BINARY_DIR}/src/version.rc")
  target_compile_definitions(argtable3 INTERFACE argtable3_IMPORTS)
else()
  add_library(argtable3 SHARED ${ARGTABLE3_SRC_FILES})
endif()
target_include_directories(argtable3 PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_library(argtable3_static STATIC ${ARGTABLE3_SRC_FILES})
target_include_directories(argtable3 PRIVATE ${PROJECT_SOURCE_DIR}/src)

set_target_properties(argtable3 argtable3_static PROPERTIES
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
  SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

include(GNUInstallDirs)
if(UNIX OR MSYS OR MINGW)
  set(ARGTABLE3_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/argtable3)
elseif(WIN32)
  set(ARGTABLE3_INSTALL_CMAKEDIR "cmake")
endif()

install(TARGETS argtable3
  EXPORT ${ARGTABLE3_PACKAGE_NAME}Config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS argtable3_static
  EXPORT ${ARGTABLE3_PACKAGE_NAME}Config
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${PROJECT_SOURCE_DIR}/src/argtable3.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${ARGTABLE3_PACKAGE_NAME}Config DESTINATION ${ARGTABLE3_INSTALL_CMAKEDIR})
