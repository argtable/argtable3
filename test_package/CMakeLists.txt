# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

cmake_minimum_required(VERSION 3.3)

project(ArgTable3TestConan C)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

enable_testing()

if (CONAN_SETTINGS_OS)
    find_package(argtable3 REQUIRED)

    add_executable(${PROJECT_NAME} testargtable3.c)
    target_link_libraries(${PROJECT_NAME} PUBLIC argtable3)

    add_test(NAME ${PROJECT_NAME} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} "$<TARGET_FILE:${PROJECT_NAME}>" --help)
else ()
    add_subdirectory("${CONAN_ARGTABLE3_ROOT}/src" "${CMAKE_CURRENT_BINARY_DIR}/argtable3")

    # Compiled library should exist and be a target in the current CMake project
    add_test(NAME lib_exists COMMAND "${CMAKE_COMMAND}" -E md5sum "$<TARGET_FILE:argtable3>")
endif ()
