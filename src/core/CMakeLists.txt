add_library(avm-core STATIC core.c string.c typeinfo.c thread.c thread-posix.c)
target_link_libraries(avm-core PRIVATE gc Threads::Threads)

target_include_directories(avm-core
    PRIVATE ${CMAKE_SOURCE_DIR}/deps
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/include
)
