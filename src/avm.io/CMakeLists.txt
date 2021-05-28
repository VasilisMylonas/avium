add_library(avm.io
    file-stream.c
    file.c
    io.c
    memory-stream.c
    path.c
)

target_link_libraries(avm.io avm.core)
