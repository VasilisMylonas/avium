# Tests.

# NOTE: xunit-viewer hangs after HTML generation. To fix it, add this after
# line 33 in xunit-viewer.js located in the package folder:
# process.exit(0)
find_package(XUnitViewer)

function(avm_add_test TEST)
    add_executable(${TEST} ${TEST}.c)
    add_dependencies(${TEST} cmocka)
    target_link_libraries(${TEST} avm-static libcmocka.so)
    if(XUNITVIEWER_FOUND)
        add_test(NAME ${TEST}
                 COMMAND ${CMAKE_SOURCE_DIR}/scripts/run-test.sh
                         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
    else()
        add_test(NAME ${TEST} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST})
    endif()
endfunction()

avm_add_test(string)
