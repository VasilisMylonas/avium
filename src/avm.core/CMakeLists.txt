add_library(avm.core ${LIB_TYPE}
    error.c
    runtime.c
    string.c
    types.c
    version.c
)

if(USE_GC)
    target_link_libraries(avm.core gcmt-lib)
endif()
