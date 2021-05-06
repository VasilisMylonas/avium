add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

# include(TCC)
find_library(LIBDL dl REQUIRED)

target_link_libraries(avm.reflect avm.core ${LIBDL} ${LIBTCC})
