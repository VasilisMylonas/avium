add_executable(tests tests.c)
target_link_libraries(tests avm)
target_link_libraries(tests avmtest)
