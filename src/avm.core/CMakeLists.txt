add_library(avm.core
    error.c
    core.c
    string.c
    typeinfo.c
    types.c
)

if(USE_GC)
    include(GC)
    target_link_libraries(avm.core ${LIBGC})
endif()
