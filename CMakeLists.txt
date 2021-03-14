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
set(AVM_COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/coverage)
set(AVM_TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)

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

file(MAKE_DIRECTORY ${AVM_TEST_OUTPUT_DIR})
file(MAKE_DIRECTORY ${AVM_COVERAGE_OUTPUT_DIR})

add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
add_link_options(-fprofile-instr-generate -fcoverage-mapping)

configure_file(./scripts/run-test.sh.in run-test.sh @ONLY)

function(avm_add_test TEST)
    add_executable(${TEST} tests/${TEST}.c)
    add_dependencies(${TEST} cmocka)
    target_link_libraries(${TEST} avm-static libcmocka.so)
    add_test(NAME ${TEST} COMMAND run-test.sh ${TEST})
endfunction()

# NOTE: xunit-viewer hangs after HTML generation. To fix it, add this after
# line 33 in xunit-viewer.js located in the package folder:
# process.exit(0)
find_program(AVM_XUNIT_VIEWER_PROGRAM xxxunit-viewer)

if(AVM_XUNIT_VIEWER_PROGRAM MATCHES "AVM_XUNIT_VIEWER_PROGRAM-NOTFOUND")
    message(WARNING "xunit-viewer not found.")
endif()

# Tests.
if(${BUILD_TESTING})
    avm_add_test(string)
endif()

# set(AVM_COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/coverage)
# set(AVM_COVERAGE_PROGRAM ${CMAKE_SOURCE_DIR}/scripts/coverage.sh)
# file(MAKE_DIRECTORY ${AVM_COVERAGE_OUTPUT_DIR})
# add_custom_target(Coverage ALL
#     COMMAND ${AVM_COVERAGE_PROGRAM} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#     WORKING_DIRECTORY ${AVM_COVERAGE_OUTPUT_DIR})

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
set(CPACK_PACKGAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CPACK_PACKGAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
include(CPack)
