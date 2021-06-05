add_library(avm-core STATIC core.c string.c typeinfo.c) # threads.c
target_link_libraries(avm-core gc)

# if(NOT WIN32)
#     find_library(LIBPTHREAD pthread REQUIRED)
#     target_link_libraries(avm-core ${LIBPTHREAD})
# endif()

target_include_directories(avm-core
    PRIVATE ${CMAKE_SOURCE_DIR}/deps
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/include
)
