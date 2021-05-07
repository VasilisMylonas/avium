add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

# include(TCC)

if(WIN32)
    set(LIBDL dl)
else()
    find_library(LIBDL dl REQUIRED)
endif()

target_link_libraries(avm.reflect avm.core ${LIBDL} ${LIBTCC} basename)
