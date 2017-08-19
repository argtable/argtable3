PROJECT( argtable3 )

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

option(argtable3_build_tests "Build all of argtable3's own tests." ON)

option(argtable3_build_examples "Build argtable3's examples." ON)

add_library( argtable3 argtable3.c )

install( TARGETS argtable3 ARCHIVE DESTINATION lib)
set_target_properties( argtable3 PROPERTIES DEBUG_POSTFIX d )

if (argtable3_build_tests)
    add_subdirectory(tests)
endif()

if (argtable3_build_examples)
    add_subdirectory(examples)
endif()
