add_library(avm.reflect codegen.c reflect.c callable.c)

target_link_libraries(avm.reflect avm.core dl ffi)
