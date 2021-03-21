# Tests.

function(add_avm_test TEST)
    set(RUNNER ${CMAKE_BINARY_DIR}/tests/${TEST}_runner.c)

    add_custom_command(OUTPUT ${RUNNER}
                       COMMAND ${CMAKE_SOURCE_DIR}/testgen.py ${CMAKE_SOURCE_DIR}/tests/${TEST}.c -o ${RUNNER}
                       MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/tests/${TEST}.c)

    set_source_files_properties(${RUNNER} PROPERTIES GENERATED TRUE)

    add_executable(${TEST} ${RUNNER})
    target_link_libraries(${TEST} avm-static)
    add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
endfunction(add_avm_test)

add_avm_test(string)
