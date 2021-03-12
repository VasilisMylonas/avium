cmake_minimum_required(VERSION 3.0.0)
project(avium VERSION 0.1.0 DESCRIPTION "An object oriented C utility runtime." LANGUAGES C)

include(GNUInstallDirs)
include(CTest)

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
set(AVM_COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)
set(AVM_TAB_WIDTH 4)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(AVM_COVERAGE_PROGRAM ${CMAKE_SOURCE_DIR}/scripts/coverage.sh)

find_program(AVM_DOC_PROGRAM natural-docs REQUIRED)

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
    add_compile_options(/Wall) # Enable all warnings.
    add_compile_options(/WX) # Treat warnings as errors.
    add_compile_options(/wd4710) # Function not inlined.
    add_compile_options(/wd4201) # Nameless struct/union.
    add_compile_options(/wd5045) # Spectre mitigation.
    add_compile_options(/wd4668) # <macro> not defined as preprocessor macro, replacing with 0.
    add_compile_options(/wd4324) # Struct padded due to alignment specifier.
    add_compile_options(/wd4200) # Zero-sized array in struct/union.
    add_compile_options(/wd4063) # Invalid value for switch of enum.
else()
    add_compile_options(-fprofile-instr-generate)
    add_compile_options(-fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
    add_link_options(-fcoverage-mapping)
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-Werror)
endif()

include_directories(${AVM_INCLUDE_DIR})
include_directories(${AVM_INCLUDE_OUTPUT_DIR})

configure_file(${AVM_INCLUDE_DIR}/avium/config.h.in ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h @ONLY)
configure_file(Project.txt.in Project.txt @ONLY)
configure_file(Comments.txt.in Comments.txt @ONLY)
configure_file(Languages.txt.in Languages.txt @ONLY)

file(MAKE_DIRECTORY ${AVM_IMAGES_DIR})
file(MAKE_DIRECTORY ${AVM_DOC_DIR})
file(MAKE_DIRECTORY ${AVM_COVERAGE_DIR})

add_subdirectory(src)

set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_DIR}/avium/avium.h
    ${AVM_INCLUDE_DIR}/avium/core.h
    ${AVM_INCLUDE_DIR}/avium/fmt.h
    ${AVM_INCLUDE_DIR}/avium/options.h
    ${AVM_INCLUDE_DIR}/avium/string.h
    ${AVM_INCLUDE_DIR}/avium/testing.h
)

set(AVM_DOC_FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/COPYING
    ${CMAKE_SOURCE_DIR}/COPYING.LESSER
)

install(DIRECTORY ${AVM_DOCS_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avium)
install(TARGETS avm avm-static avmtest DESTINATION ${CMAKE_INSTALL_LIBDIR}/avium)

add_custom_target(Documentation ALL COMMAND ${AVM_DOC_PROGRAM} ${CMAKE_BINARY_DIR})
add_custom_target(Coverage ALL
    COMMAND ${AVM_COVERAGE_PROGRAM} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${AVM_COVERAGE_DIR}
)

if(${BUILD_TESTING})
    file(GLOB AVM_TEST_FILES tests/*.c)
    foreach(FILE ${AVM_TEST_FILES})
        string(REPLACE ".c" "" NAME ${FILE})
        get_filename_component(NAME ${NAME} NAME_WE)
        add_executable(${NAME} ${FILE})
        target_link_libraries(${NAME} avm-static)
        target_link_libraries(${NAME} avmtest)
        add_test(NAME ${NAME} COMMAND ${NAME} FAIL_REGULAR_EXPRESSION "not ok;# ERROR;Bail out!")
    endforeach()
endif()
