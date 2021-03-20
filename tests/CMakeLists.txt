# Tests.

function(avm_add_test TEST)
    add_executable(${TEST} ${TEST}.c)
    add_dependencies(${TEST} cmocka)
    target_link_libraries(${TEST} avm-static libcmocka.so)
    add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
endfunction()

avm_add_test(string)
