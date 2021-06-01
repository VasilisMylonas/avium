add_library(avm.core core.c string.c typeinfo.c)

include(GC)
target_link_libraries(avm.core ${LIBGC})
