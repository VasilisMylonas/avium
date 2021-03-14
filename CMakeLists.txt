cmake_minimum_required(VERSION 3.5.0)
project(avium
        VERSION 0.1.0
        DESCRIPTION "An object oriented C utility runtime."
        LANGUAGES C)

include(GNUInstallDirs)
include(CTest)
include(ExternalProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(AVM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(AVM_INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/include)

# Include cmocka
ExternalProject_Add(cmocka
    URL https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz
    PREFIX ${CMAKE_BINARY_DIR}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR})

if (MSVC)
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
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${AVM_INCLUDE_DIR} ${AVM_INCLUDE_OUTPUT_DIR})
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Include subdirectories.
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(docs)

option(AVM_RUN_TESTS "Run tests?" ON)

# Tests.
if(${AVM_RUN_TESTS})
    add_subdirectory(tests)
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
set(CPACK_PACKGAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CPACK_PACKGAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
include(CPack)

configure_file(avium-descriptor.xml.in avium-descriptor.xml @ONLY)
configure_file(scripts/api-sanity-check.sh.in scripts/api-sanity-check.sh @ONLY)

add_custom_target(api-sanity-check ALL scripts/api-sanity-check.sh)
