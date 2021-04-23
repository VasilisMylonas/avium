find_package(Python3 REQUIRED)

function(run_test TEST)
    set(RUNNER ${CMAKE_BINARY_DIR}/tests/${TEST}_runner.c)
    set(TEST_FILE ${CMAKE_SOURCE_DIR}/tests/${TEST}.c)

    add_custom_command(OUTPUT ${RUNNER}
                       COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/testgen.py ${TEST_FILE} -o ${RUNNER}
                       MAIN_DEPENDENCY ${TEST_FILE})

    set_source_files_properties(${RUNNER} PROPERTIES GENERATED TRUE)

    find_library(GCOV_PROGRAM gcov)
    add_executable(${TEST} ${RUNNER})

    if(GCOV_PROGRAM)
        target_link_libraries(${TEST} ${GCOV_PROGRAM})
    endif()

    target_link_libraries(${TEST} ${STATIC_NAME})
    add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
endfunction()

run_test(array-list)
run_test(array)
run_test(io)
run_test(options)
run_test(string)
run_test(value)
run_test(version)
