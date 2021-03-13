cmake_minimum_required(VERSION 3.5.0)
project(avium
        VERSION 0.1.0
        DESCRIPTION "An object oriented C utility runtime."
        LANGUAGES C)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(AVM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(AVM_INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/include)

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
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${AVM_INCLUDE_DIR} ${AVM_INCLUDE_OUTPUT_DIR})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(docs)

if(${BUILD_TESTING})
    file(GLOB AVM_TEST_FILES tests/*.c)
    foreach(FILE ${AVM_TEST_FILES})
        string(REPLACE ".c" "" NAME ${FILE})
        get_filename_component(NAME ${NAME} NAME_WE)
        add_executable(${NAME} ${FILE})
        target_link_libraries(${NAME} avm-static avmtest)
        add_test(NAME ${NAME} COMMAND ${NAME} FAIL_REGULAR_EXPRESSION "not ok;# ERROR;Bail out!")
    endforeach()
endif()

# add_custom_target(Coverage ALL
#     COMMAND ${AVM_COVERAGE_PROGRAM} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#     WORKING_DIRECTORY ${AVM_COVERAGE_DIR}
# )
# set(AVM_COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)
# set(AVM_COVERAGE_PROGRAM ${CMAKE_SOURCE_DIR}/scripts/coverage.sh)
# file(MAKE_DIRECTORY ${AVM_COVERAGE_DIR})
