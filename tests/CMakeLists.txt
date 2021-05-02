function(run_test TEST)
    add_library(${TEST} SHARED ${TEST}.c)
    target_link_libraries(${TEST} avm)
    add_test(NAME ${TEST} COMMAND avmtest ${CMAKE_BINARY_DIR}/lib/${TEST})
endfunction()

run_test(string)
