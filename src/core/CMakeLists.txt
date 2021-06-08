if (AVM_HAVE_POSIX_THREADS)
    set(AVM_THREAD_BACKEND thread-posix.c)
else()
    set(AVM_THREAD_BACKEND thread-win32.c)
endif()

add_library(avm-core STATIC core.c string.c typeinfo.c thread.c ${AVM_THREAD_BACKEND})
target_link_libraries(avm-core PRIVATE gc Threads::Threads)

if(MSVC)
    target_compile_options(avm-core PRIVATE ${AVM_MSVC_COMPILE_OPTIONS})
else()
    target_compile_options(avm-core PRIVATE ${AVM_GNU_COMPILE_OPTIONS})
endif()

target_include_directories(avm-core
    PRIVATE ${CMAKE_SOURCE_DIR}/deps
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/include
)
