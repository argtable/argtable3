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

# DEB and RPM disabled by default, but can be built manually via `cpack -G DEB`
set(CPACK_GENERATOR
    TGZ
    ZIP
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_BINARY_DIR}"
    "${PROJECT_NAME}"
    ALL
    .
)


set(CPACK_PACKAGE_VENDOR "Tom G. Huang")
set(CPACK_PACKAGE_CONTACT "Tom G. Huang <tomghuang@gmail.com>")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
if (PROJECT_VERSION_TWEAK)
    set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}.${PROJECT_VERSION_TWEAK})
endif ()
set(CPACK_PACKAGE_RELOCATABLE ON)

# Debian Specific
set(CPACK_DEBIAN_PACKAGE_SECTION                    devel)
set(CPACK_DEBIAN_PACKAGE_PRIORITY                   optional)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS                  ON)
set(CPACK_DEBIAN_PACKAGE_RELEASE                    1)
set(CPACK_DEBIAN_FILE_NAME                          DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION  ON)

# RPM Specific
set(CPACK_RPM_FILE_NAME                         RPM-DEFAULT)
set(CPACK_RPM_PACKAGE_LICENSE                   "BSD 3-Clause")
set(CPACK_RPM_RELOCATION_PATHS                  /)

include(CPack)
