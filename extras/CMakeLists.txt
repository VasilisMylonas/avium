add_library(dl STATIC dlfcn.c)

if(NOT WIN32)
    find_library(LIBDL dl REQUIRED)
    target_link_libraries(dl ${LIBDL})
endif()
