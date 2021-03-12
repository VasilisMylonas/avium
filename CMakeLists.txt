cmake_minimum_required(VERSION 3.0.0)
project(avium VERSION 0.1.0 DESCRIPTION "An object oriented C utility runtime." LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(AVM_PROJECT_COPYRIGHT "Copyright © 2021 Vasilis Mylonas")
set(AVM_TIMESTAMP_FORMAT "yyyy-mm-dd")
set(AVM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(AVM_INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/include)
set(AVM_DOC_DIR ${CMAKE_BINARY_DIR}/docs)
set(AVM_IMAGES_DIR ${CMAKE_BINARY_DIR}/images)
set(AVM_TAB_WIDTH 4)
set(AVM_TAP_FILE ${CMAKE_BINARY_DIR}/tests/results.tap)
set(AVM_TESTS_FILE ${CMAKE_BINARY_DIR}/tests/results.html)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(AVM_LINUX ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(AVM_DARWIN ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(AVM_WIN32 ON)
endif()

if (MSVC)
set(AVM_COMPILER_OPTIONS
    /Wall   # Enable all warnings.
    /WX     # Treat warnings as errors.
    /wd4710 # Function not inlined.
    /wd4201 # Nameless struct/union.
    /wd5045 # Spectre mitigation.
    /wd4668 # <macro> not defined as preprocessor macro, replacing with 0.
    /wd4324 # Struct padded due to alignment specifier.
    /wd4200 # Zero-sized array in struct/union.
    /wd4063 # Invalid value for switch of enum.
)
else()
set(AVM_COMPILER_OPTIONS
    -Wall      # Enable all warnings.
    -Wextra    # Enable extra warnings.
    -Wpedantic # No extensions.
    -Werror    # Treat warnings as errors.
)
endif()

include_directories(${AVM_INCLUDE_DIR})
include_directories(${AVM_INCLUDE_OUTPUT_DIR})
add_compile_options(${AVM_COMPILER_OPTIONS})

configure_file(${AVM_INCLUDE_DIR}/avium/config.h.in ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h @ONLY)
configure_file(Project.txt.in Project.txt @ONLY)
configure_file(Comments.txt.in Comments.txt @ONLY)
configure_file(Languages.txt.in Languages.txt @ONLY)

file(MAKE_DIRECTORY ${AVM_IMAGES_DIR})
file(MAKE_DIRECTORY ${AVM_DOC_DIR})

find_program(AVM_DOC_PROGRAM natural-docs REQUIRED)
find_program(AVM_TAP_PROGRAM tapview REQUIRED)
find_program(AVM_TAP2_PROGRAM pytap2 REQUIRED)

add_custom_target(gen-docs ALL COMMAND ${AVM_DOC_PROGRAM} ${CMAKE_BINARY_DIR})
add_custom_target(gen-tests ALL COMMAND
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests --avm-tap-output=${AVM_TAP_FILE} &&
    cat ${AVM_TAP_FILE} | ${AVM_TAP_PROGRAM} &&
    ${AVM_TAP2_PROGRAM} html ${AVM_TAP_FILE} > ${AVM_TESTS_FILE} &&
    x-www-browser ${AVM_TESTS_FILE} DEPENDS src tests)

add_subdirectory(src)
add_subdirectory(tests)

set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_DIR}/avium/avium.h
    ${AVM_INCLUDE_DIR}/avium/core.h
    ${AVM_INCLUDE_DIR}/avium/fmt.h
    ${AVM_INCLUDE_DIR}/avium/options.h
    ${AVM_INCLUDE_DIR}/avium/string.h
    ${AVM_INCLUDE_DIR}/avium/testing.h)

set(AVM_DOC_FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/COPYING
    ${CMAKE_SOURCE_DIR}/COPYING.LESSER)

include(GNUInstallDirs)

install(DIRECTORY ${AVM_DOCS_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avium)
install(TARGETS avm avm-static avmtest DESTINATION ${CMAKE_INSTALL_LIBDIR}/avium)
