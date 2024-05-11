#!/bin/bash
mkdir fuzz-build
cd fuzz-build
cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_CXX_FLAGS="$CXXFLAGS" ../
sed -i 's/SHARED/STATIC/g' ../CMakeLists.txt
make V=1 || true

$CC $CFLAGS $LIB_FUZZING_ENGINE $SRC/fuzzer.c -Wl,--whole-archive $SRC/argtable3/fuzz-build$SRC/libargtable3_static.a -Wl,--allow-multiple-definition -I$SRC/argtable3/dist/tests -I$SRC/argtable3/tests -I$SRC/argtable3/src -I$SRC/argtable3/dist  -o $OUT/fuzzer