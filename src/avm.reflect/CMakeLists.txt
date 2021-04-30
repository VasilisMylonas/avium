add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

find_library(LIBDL dl REQUIRED)
find_library(LIBTCC tcc REQUIRED)

target_link_libraries(avm.reflect avm.core ${LIBDL} ${LIBTCC})
