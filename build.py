#!/usr/bin/python

import platform
from cpt.packager import ConanMultiPackager

if __name__ == "__main__":
    # Do not attempt to cross-compile because this CMake/Conan setup requires a CMake toolchain file to make that work
    builder = ConanMultiPackager(archs=[{
        'x86_64': 'x86_64',
        'i386': 'x86'
    }[platform.machine()]])
    builder.add_common_builds(shared_option_name="argtable3:shared")
    builder.run()
