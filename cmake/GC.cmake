set(BDWGC_TAG v8.0.4)
set(BDWGC_URL https://github.com/ivmai/bdwgc)
set(BDWGC_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/bdwgc-src/include)

set(ATOMIC_OPS_URL https://github.com/ivmai/libatomic_ops)
set(ATOMIC_OPS_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/atomic_ops-src/src)

set(enable_threads ON)

FetchContent_Declare(atomic_ops GIT_REPOSITORY ${ATOMIC_OPS_URL})
FetchContent_Declare(bdwgc GIT_REPOSITORY ${BDWGC_URL} GIT_TAG ${BDWGC_TAG})

include_directories(${BDWGC_INCLUDE_DIR} ${ATOMIC_OPS_INCLUDE_DIR})

FetchContent_MakeAvailable(atomic_ops)
FetchContent_MakeAvailable(bdwgc)

set(LIBGC gcmt-lib)
