# Tests.

find_package(Python3 REQUIRED)

function(add_avm_test TEST)
    set(RUNNER ${CMAKE_BINARY_DIR}/tests/${TEST}_runner.c)
    set(TEST_FILE ${CMAKE_SOURCE_DIR}/tests/${TEST}.c)

    add_custom_command(OUTPUT ${RUNNER}
                       COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/testgen.py ${TEST_FILE} -o ${RUNNER}
                       MAIN_DEPENDENCY ${TEST_FILE})

    set_source_files_properties(${RUNNER} PROPERTIES GENERATED TRUE)

    find_library(AVM_GCOV gcov)

    add_executable(${TEST} ${RUNNER})
    if(AVM_GCOV)
        target_link_libraries(${TEST} ${AVM_GCOV})
    endif()
    target_link_libraries(${TEST} avm-static)
    add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
endfunction(add_avm_test)

make_directory(${CMAKE_BINARY_DIR}/coverage)

add_avm_test(array-list)
add_avm_test(array)
add_avm_test(file)
add_avm_test(io)
add_avm_test(optional)
add_avm_test(options)
add_avm_test(result)
add_avm_test(string)
add_avm_test(value)
add_avm_test(version)
