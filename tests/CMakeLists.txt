# Tests.

function(add_avm_test TEST)
    set(RUNNER ${CMAKE_BINARY_DIR}/tests/${TEST}_runner.c)
    set(TEST_FILE ${CMAKE_SOURCE_DIR}/tests/${TEST}.c)

    add_custom_command(OUTPUT ${RUNNER}
                       COMMAND ${CMAKE_SOURCE_DIR}/scripts/testgen.py ${TEST_FILE} -o ${RUNNER}
                       MAIN_DEPENDENCY ${TEST_FILE})

    set_source_files_properties(${RUNNER} PROPERTIES GENERATED TRUE)

    add_executable(${TEST} ${RUNNER})
    target_link_libraries(${TEST} avm-static)
    add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
endfunction(add_avm_test)

add_avm_test(string)
add_avm_test(result)
add_avm_test(optional)
add_avm_test(version)
add_avm_test(value)
# add_avm_test(array-list)
# add_avm_test(options)
add_avm_test(array)

add_custom_target(BuildCoverageHtml COMMAND ${CMAKE_SOURCE_DIR}/scripts/gcovr.sh)
