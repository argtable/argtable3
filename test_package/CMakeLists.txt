# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

cmake_minimum_required(VERSION 3.3)

project(ArgTable3TestConan C)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)
find_package(argtable3 REQUIRED)

add_executable(${PROJECT_NAME} testargtable3.c)
target_link_libraries(${PROJECT_NAME} PUBLIC argtable3)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} "$<TARGET_FILE:${PROJECT_NAME}>" --help)
