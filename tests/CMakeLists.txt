# Tests.

# NOTE: xunit-viewer hangs after HTML generation. To fix it, add this after
# line 33 in xunit-viewer.js located in the package folder:
# process.exit(0)
find_program(AVM_XUNIT_VIEWER_PROGRAM xunit-viewer)

if(AVM_XUNIT_VIEWER_PROGRAM MATCHES "AVM_XUNIT_VIEWER_PROGRAM-NOTFOUND")
    message(WARNING "xunit-viewer not found.")
endif()

function(avm_add_test TEST)
    add_executable(${TEST} ${TEST}.c)
    add_dependencies(${TEST} cmocka)
    target_link_libraries(${TEST} avm-static libcmocka.so)
    if(AVM_XUNIT_VIEWER_PROGRAM MATCHES "AVM_XUNIT_VIEWER_PROGRAM-NOTFOUND")
        add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
    else()
        add_test(NAME ${TEST}
                 COMMAND ${CMAKE_SOURCE_DIR}/scripts/run-test.sh
                         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
    endif()
endfunction()

avm_add_test(string)
