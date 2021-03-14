# Tests.

set(AVM_COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/coverage)
set(AVM_TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)

file(MAKE_DIRECTORY ${AVM_TEST_OUTPUT_DIR})
file(MAKE_DIRECTORY ${AVM_COVERAGE_OUTPUT_DIR})

configure_file(${CMAKE_SOURCE_DIR}/scripts/run-test.sh.in run-test.sh @ONLY)

function(avm_add_test TEST)
    add_executable(${TEST} ${TEST}.c)
    add_dependencies(${TEST} cmocka)
    target_compile_options(${TEST} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(${TEST} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_libraries(${TEST} avm-static libcmocka.so)
    add_test(NAME ${TEST} COMMAND run-test.sh ${TEST})
endfunction()

# NOTE: xunit-viewer hangs after HTML generation. To fix it, add this after
# line 33 in xunit-viewer.js located in the package folder:
# process.exit(0)
find_program(AVM_XUNIT_VIEWER_PROGRAM xunit-viewer)

if(AVM_XUNIT_VIEWER_PROGRAM MATCHES "AVM_XUNIT_VIEWER_PROGRAM-NOTFOUND")
    message(WARNING "xunit-viewer not found.")
endif()

configure_file(avium-descriptor.xml.in avium-descriptor.xml @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/api-sanity-check.sh.in
               ${CMAKE_BINARY_DIR}/scripts/api-sanity-check.sh @ONLY)

add_custom_target(api-sanity-check ${CMAKE_BINARY_DIR}/scripts/api-sanity-check.sh
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

avm_add_test(string)
