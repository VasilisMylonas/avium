cmake_minimum_required(VERSION 3.5.0)
project(avium VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(INCLUDE_OUT_DIR ${CMAKE_BINARY_DIR}/include)
set(INCLUDE_EXTRAS_DIR ${CMAKE_SOURCE_DIR}/extras/include)

set(DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(HTML_OUTPUT_DIR ${DOC_OUTPUT_DIR}/html)

include(GNUInstallDirs)
include(FetchContent)
include(CTest)

option(BUILD_SHARED_LIBS "Build dynamic library instead?" OFF)
option(BUILD_TESTS "Generate tests?" ON)
option(USE_IO "Use the IO library?" ON)
option(USE_ARGPARSE "Use the argument parsing library?" ON)
option(USE_REFLECT "Use the reflection library?" ON)
option(USE_COLLECTIONS "Use the collections library?" ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    add_compile_options(/WX)

    # Disable silly warnings.
    add_compile_options(/wd4710) # Function not inlined.
    add_compile_options(/wd4098) # Returning void.
    add_compile_options(/wd4201) # Nameless struct/union.
    add_compile_options(/wd5045) # Spectre mitigation.
    add_compile_options(/wd4668) # <macro> not defined as preprocessor macro, replacing with 0.
    add_compile_options(/wd4324) # Struct padded due to alignment specifier.
    add_compile_options(/wd4820) # 'x' bytes padding added after member...
    add_compile_options(/wd4200) # Zero-sized array in struct/union.
    add_compile_options(/wd4063) # Invalid value for switch of enum.
    add_compile_options(/wd5105) # Macro expansion producing 'defined'...
    add_compile_options(/wd4711) # Function not selected for inlining.
    add_compile_options(/wd4244) # Conversion ... possible loss of data.
    add_compile_options(/wd4242) # Conversion ... possible loss of data.
    add_compile_options(/wd4267) # Conversion ... possible loss of data.
    add_compile_options(/wd4232) # Address of dllimport ... is not static, identity not guaranteed.
    add_compile_options(/wd4273) # TODO: Temporary
    add_compile_options(/wd4100)
else()
    if(NOT CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(-fprofile-arcs -ftest-coverage)
        add_link_options(-fprofile-arcs -ftest-coverage)
    endif()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-deprecated)
endif()

include_directories(${INCLUDE_DIR} ${INCLUDE_OUT_DIR} ${INCLUDE_EXTRAS_DIR})

add_subdirectory(extras)
add_subdirectory(include)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(docs)

install(FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/CONTRIBUTING.md
    ${CMAKE_SOURCE_DIR}/CODE_OF_CONDUCT.md
    ${CMAKE_SOURCE_DIR}/AUTHORS
    ${CMAKE_SOURCE_DIR}/COPYING
    ${CMAKE_SOURCE_DIR}/COPYING.LESSER
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
