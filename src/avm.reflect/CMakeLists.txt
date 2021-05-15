add_library(avm.reflect codegen.c)

target_link_libraries(avm.reflect avm.core dl basename)
