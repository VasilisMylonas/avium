add_library(avm.core ${LIB_TYPE}
    error.c
    core.c
    string.c
    typeinfo.c
    types.c
)

if(USE_GC)
    target_link_libraries(avm.core gcmt-lib)
endif()
