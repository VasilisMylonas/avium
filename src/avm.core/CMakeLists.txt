add_library(avm.core
    core.c
    string.c
    typeinfo.c
)

if(USE_GC)
    include(GC)
    target_link_libraries(avm.core ${LIBGC})
endif()
