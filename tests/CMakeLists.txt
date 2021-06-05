function(run_test TEST)
    add_executable(${TEST} ${TEST}.c)
    target_link_libraries(${TEST} avm-core)
    add_test(NAME ${TEST} COMMAND ${TEST})
endfunction()

run_test(string)
