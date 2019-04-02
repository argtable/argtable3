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

import os
import os.path

from conans import ConanFile, CMake, tools

class Argtable3(ConanFile):
    name = 'argtable3'
    description = 'A single-file, ANSI C, command-line parsing library that parses GNU-style command-line options.'
    url = 'https://github.com/argtable/argtable3'
    version = tools.load('version.tag').strip() if os.path.isfile('version.tag') else "v0.0.0"
    license = 'BSD 3-Clause'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {'shared': [True, False]}
    default_options = {'shared': True}
    generators = 'cmake'
    scm = {
        'type': 'git',
        'url': 'auto',
        'revision': 'auto'
    }
    exports = 'version.tag'

    def build(self):
        if self.settings.os:
            cmake = self.configure_cmake()
            cmake.build()

    def package(self):
        if self.settings.os:
            cmake = self.configure_cmake()
            cmake.install()
        else:
            self.copy('*')

    def package_info(self):
        if self.settings.os:
            self.cpp_info.libs = [self.name]

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(defs={}, build_folder=self.binary_folder)
        return cmake

    @property
    def binary_folder(self):
        return os.path.join(self.build_folder, 'build')
