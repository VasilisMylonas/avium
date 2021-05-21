function(run_test TEST)
    add_executable(${TEST} ${TEST}.c)
    target_link_libraries(${TEST} avm)
    add_test(NAME ${TEST} COMMAND ${TEST})
endfunction()

run_test(string)
run_test(reflect)
run_test(path)
