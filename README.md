[![Build Status](https://travis-ci.org/argtable/argtable3.svg?branch=master)](https://travis-ci.org/argtable/argtable3)


Introduction of Argtable3
=========================

**Argtable3** is an open source ANSI C library that parses GNU-style
command-line options. It simplifies command-line parsing by defining a
declarative-style API that you can use to specify what your command-line syntax
looks like. Argtable3 will automatically generate consistent error handling
logic and textual descriptions of the command line syntax, which are essential
but tedious to implement for a robust CLI program.


Quick Start
-----------

> We no longer provide the amalgamation source code (`argtable3.c` and
> `argtable3.h`) in the source code repository. You can get the amalgamation
> distribution either from the release page (`argtable-3.x.x-amalgamation.zip`),
> or generate the distribution yourself by using the generator under the `tools`
> directory:
> 
> 1. Navigate to the `tools` directory.
> 2. Run `./build dist`, which will generate the distribution under the `<ROOT>/dist`
>    directory.


Argtable3 is a single-file ANSI-C library. All you have to do is adding
`argtable3.c` to your projects, and including `argtable3.h` in your source code.

To build the library, examples, and unit tests, use CMake to generate
out-of-source build:

* If you use GCC (Linux, MacOSX, MinGW, Cygwin), run:

  ```
  $ mkdir build
  $ cd build
  $ cmake -DCMAKE_BUILD_TYPE=Debug ..
  $ make
  $ make test
  ```

  Makefile-based generators in CMake only support one configuration at a time,
  so you need to specify `CMAKE_BUILD_TYPE` to `Debug`, `Release`, `MinSizeRel`,
  or `RelWithDebInfo`. To build multiple configurations, you need to create a
  build directory for each configuraiton.
  
  To cleanup, run `make clean` or remove the build directory:
  
  ```
  $ rm -rf build
  ```

* If you use Microsoft Visual C++ compiler, run:

  ```
  $ mkdir build
  $ cd build
  $ cmake -G "Visual Studio 15 2017 Win64" ..
  $ cmake --build . --config Debug
  $ ctest -C Debug
  ```

  You can also use Visual Studio 2017 IDE to open the generated solution. To
  cleanup, just remove the `build` directory.


To build a tagged version, go to the project root directory, and use the
`Makefile` in the project root folder to check out the specified version:

  ```
  $ make taglist
  Available TAGs:
  v3.1.1.432a160
  $ make co TAG=v3.1.1.432a160
  $ cd .tags/v3.1.1.432a160
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ make test
  ```

You will find the shared library (or Windows DLL), static library, and the
amalgamation distribution under the build directory.


Documentation and Examples
--------------------------

To learn how to use the Argtable3 API, you can see the documentation on the web
site, or examples in the `examples` folder.


Unit Tests
----------

Argtable3 is a BSD-licensed open source library, so you can modify the library
anyway you want. However, before committing your code to your own repository or
the Argtable3 official repository, please make sure your changes won't cause any
compiler warning and can pass the unit tests included in the distribution.

To build and test each configuration (`Debug`, `Release`, `MinSizeRel`,
`RelWithDebInfo`), you can run CMake and CTest on all supported platforms:

```
$ mkdir build_debug && cd build_debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ cmake --build . --config Debug
$ ctest -C Debug

$ cd .. && mkdir build_release && cd build_release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build . --config Release
$ ctest -C Release

$ cd .. && mkdir build_minsizerel && cd build_minsizerel
$ cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
$ cmake --build . --config MinSizeRel
$ ctest -C MinSizeRel

$ cd .. && mkdir build_relwithdebinfo && cd build_relwithdebinfo
$ cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
$ cmake --build . --config RelWithDebInfo
$ ctest -C RelWithDebInfo
```


Authors
-------

Argtable is Copyright (C) 1998-2001,2003-2011 Stewart Heitmann.
Parts are Copyright (C) 1989-1994, 1996-1999, 2001, 2003
  Free Software Foundation, Inc.

Argtable was written by Stewart Heitmann <sheitmann@users.sourceforge.net>

Argtable is now maintained by Tom G. Huang <tomghuang@gmail.com>
The project homepage of argtable 3.x is http://www.argtable.org
The project homepage of argtable 2.x is http://argtable.sourceforge.net/

Here is a list of contributors who have helped to improve argtable:

- **Nina Clemson**: Editing the original argtable-1.0 documentation.
- **Livio Bertacco**: For bug fixes and the argtable-2.x Visual C++ Makefiles.                              
- **Justin Dearing**: For bug fixes and Windows DLL support, plus code support for the Open Watcom compiler and help with the Mac OS X configuration.
- **Asa Packer**: Contributing bug fixes and upgrades to the Visual C++ Makefiles.
- **Danilo Cicerone**: For the Italian translation of "Introduction to Argtable-2x" on http://www.digitazero.org.
- **Uli Fouquet**: For configuration patches and documentation related to cross-compiling argtable from Unix to Windows, as well as providing the arg_print_glossary_gnu function.
- **Shachar Shemesh**: For Debian package integration and kick-starting the migration to automake/autoconf.
- **Jasper Lievisse Adriaanse**:  Maintaining the argtable package in OpenBSD ports.
- **Ulrich Mohr**: For bug fixes relating to Texas Instrument DSP platforms.
- **John Vickers**: For bug fixes relating to Solaris/Motorola platforms.
- **Steve O'Neil**: For bug fixes relating to Solaris/Motorola platforms.
- **Lori A. Pritchett-Sheats**: Fixing a makefile bug relating to "make dist".
- **Paolo Bormida**: For instructions on building argtable with date and regex support on Windows.
- **Michel Valin**: For bug fixes relating to the configure scripts on IBM AIX platforms and instructions on compiling the example code under AIX.
- **Steve Christensen**: Providing prebuilt packages for SPARC/Solaris and x86/Solaris platforms on www.sunfreeware.com.
- **Jess Portnoy**: Reworking the rpm package and integrating argtable into Fedora Linux.
- **Michael Brown**: Incorporating support for pkg-config into the autoconf scripts.
- **Alexander Lindert**: For extensions to the parser to support hex, octal and binary integer formats as well as KB/MB/GB suffixes.
- **Rob Zaborowski**: Providing build configuration files for CMake.
- **Moczik Gabor**: For bug fixes relating to the parsing of filepaths and filename extensions.

Argtable 2.x uses `getopt` from GNU, which is LGPL licensed. In order to switch to BSD, we replaced GNU `getopt` with NetBSD `getopt`. We really appreciate this high quality library that lays the foundation of Argtable 3.x and here is its copyright notice:

```
Copyright (c) 2000 The NetBSD Foundation, Inc.
All rights reserved.

This code is derived from software contributed to The NetBSD Foundation
by Dieter Baron and Thomas Klausner.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
       This product includes software developed by the NetBSD
       Foundation, Inc. and its contributors.
4. Neither the name of The NetBSD Foundation nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```
