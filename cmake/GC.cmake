set(BDWGC_TAG v8.0.4)
set(BDWGC_URL https://github.com/ivmai/bdwgc)
set(BDWGC_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/bdwgc-src/include)

FetchContent_Declare(bdwgc GIT_REPOSITORY ${BDWGC_URL} GIT_TAG ${BDWGC_TAG})
FetchContent_MakeAvailable(bdwgc)
include_directories(${BDWGC_INCLUDE_DIR})

set(LIBGC gcmt-lib)
