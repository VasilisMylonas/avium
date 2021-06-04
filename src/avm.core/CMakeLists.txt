add_library(avm.core core.c string.c typeinfo.c threads.c)

include(GC)
target_link_libraries(avm.core ${LIBGC})

if(NOT WIN32)
    target_link_libraries(avm.core pthread)
endif()
