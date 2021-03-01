cmake_minimum_required(VERSION 3.0.0)
project(avium VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(AVM_LINUX ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(AVM_DARWIN ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(AVM_WIN32 ON)
endif()

# /wd4201 Nameless struct/union
# /wd5045 Spectre mitigation
# /wd4710 Function not inlined
# /wd4668 <macro> is not defined as preprocessor macro, replacing with 0
# /wd4324 Structure was padded due to alignment specifier
# /wd4200 Zero-sized array in struct/union
# /wd4063 Invalid value for switch of enum...
set(AVM_MSVC_COMPILER_OPTIONS /Wall /WX /wd4710 /wd4201 /wd5045 /wd4668 /wd4324 /wd4200 /wd4063)
set(AVM_COMPILER_OPTIONS -Wall -Wextra -Wpedantic -Werror -Wno-ignored-attributes -Wno-switch)
set(AVM_INCLUDE_PATH include)

include_directories(${CMAKE_SOURCE_DIR}/${AVM_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR}/${AVM_INCLUDE_PATH})

if (MSVC)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_options(${AVM_MSVC_COMPILER_OPTIONS})
else()
add_compile_options(${AVM_COMPILER_OPTIONS})
endif()

add_subdirectory(test)
