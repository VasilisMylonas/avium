# include(GC)



add_library(avm-core STATIC core.c string.c typeinfo.c threads.c)
target_link_libraries(avm-core gc)

target_include_directories(avm-core
    PRIVATE ${CMAKE_BINARY_DIR}/_deps
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/include
)
