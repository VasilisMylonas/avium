if(WIN32)
    add_library(dl STATIC dlfcn.c)
endif()

add_library(basename STATIC basename.c)
