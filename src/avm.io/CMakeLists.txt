add_library(avm.io ${LIB_TYPE}
    file-stream.c
    file.c
    io.c
    memory-stream.c
)

target_link_libraries(avm.io avm.core)
