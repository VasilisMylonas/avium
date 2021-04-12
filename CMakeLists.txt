cmake_minimum_required(VERSION 3.5.0)
project(Avium
        VERSION 0.1.0
        DESCRIPTION "An object oriented C utility runtime."
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(AVM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(AVM_INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/include)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_ENABLE_EXPORTS TRUE)

include(GNUInstallDirs)
include(CTest)
include(GenerateExportHeader)

if(MSVC)
    # Enable warnings for MSVC and treat them as errors.
    add_compile_options(/Wall)
    add_compile_options(/WX)

    # Disable silly warnings.
    add_compile_options(/wd4710) # Function not inlined.
    add_compile_options(/wd4201) # Nameless struct/union.
    add_compile_options(/wd5045) # Spectre mitigation.
    add_compile_options(/wd4668) # <macro> not defined as preprocessor macro, replacing with 0.
    add_compile_options(/wd4324) # Struct padded due to alignment specifier.
    add_compile_options(/wd4200) # Zero-sized array in struct/union.
    add_compile_options(/wd4063) # Invalid value for switch of enum.
else()
    # Enable warnings for GCC compatible compilers.
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -ftest-coverage)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${AVM_INCLUDE_DIR} ${AVM_INCLUDE_OUTPUT_DIR})
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

option(AVM_GEN_DOCS "Generate docs?" ON)
option(AVM_RUN_TESTS "Run tests?" ON)
option(AVM_USE_GC "Use garbage collector?" ON)

# Include subdirectories.
add_subdirectory(include)
add_subdirectory(src)

# Documentation
if(AVM_GEN_DOCS)
    add_subdirectory(docs)
endif()

# Tests.
if(AVM_RUN_TESTS)
    add_subdirectory(tests)
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
set(CPACK_PACKGAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CPACK_PACKGAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
include(CPack)
