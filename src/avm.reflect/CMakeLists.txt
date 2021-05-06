add_library(avm.reflect ${LIB_TYPE}
    codegen.c
    reflect.c
)

if (WIN32)
    add_subdirectory(extras)
    include_directories(extras)
endif()

# include(TCC)
find_library(LIBDL dl REQUIRED)

target_link_libraries(avm.reflect avm.core ${LIBDL} ${LIBTCC})
