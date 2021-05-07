add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

# include(TCC)

target_link_libraries(avm.reflect avm.core ${LIBTCC} dl basename)
