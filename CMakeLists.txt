cmake_minimum_required(VERSION 3.5.0)
project(avium VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CheckTypeSize)
include(CheckIncludeFile)

enable_testing()

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    message("Building for Linux.")
    set(AVM_LINUX ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    message("Building for Darwin.")
    set(AVM_DARWIN ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL Windows)
    message("Building for Win32.")
    set(AVM_WIN32 ON)
else()
    message(FATAL_ERROR "Unknown platform.")
endif()

if(MSVC)
    set(AVM_MSVC ON)
else()
    set(AVM_GNU ON)
endif()

set(AVM_BUILDING ON)

check_type_size(long AVM_LONG_SIZE)
check_include_file(uchar.h AVM_HAVE_UCHAR_H)
check_include_file(dlfcn.h AVM_HAVE_DLFCN_H)

if(${AVM_LONG_SIZE} EQUAL 8)
    set(AVM_LONG_TYPE "long")
else()
    set(AVM_LONG_TYPE "long long")
endif()

include_directories(${AVM_INCLUDE_DIR} ${AVM_INCLUDE_OUT_DIR} deps/libatomic_ops/src)

configure_file(include/avium/config.h.in include/avium/config.h @ONLY)

set(enable_threads ON)

add_subdirectory(deps/bdwgc)
add_subdirectory(docs)
add_subdirectory(src/core)
add_subdirectory(tests)

if(MSVC)
    add_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        add_compile_options(-fprofile-arcs -ftest-coverage)
        add_link_options(-fprofile-arcs -ftest-coverage)
    endif()
endif()
