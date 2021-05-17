add_library(dl STATIC dlfcn.c)
add_library(basename STATIC basename.c)

if(NOT WIN32)
    find_library(LIBDL dl REQUIRED)
    target_link_libraries(dl ${LIBDL})
endif()
