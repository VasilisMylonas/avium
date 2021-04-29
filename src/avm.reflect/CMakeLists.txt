add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

target_link_libraries(avm.reflect avm.core)
