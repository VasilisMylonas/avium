set(TCC_URL https://github.com/LuaDist/tcc)
set(TCC_TAG 0.9.25-Ubuntu-x86_64)
set(TCC_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/tcc-src/include)

FetchContent_Declare(tcc GIT_REPOSITORY ${TCC_URL} GIT_TAG ${TCC_TAG})
FetchContent_MakeAvailable(tcc)
include_directories(${TCC_INCLUDE_DIR})

find_library(LIBTCC libtcc REQUIRED HINTS ${CMAKE_BINARY_DIR}/_deps/tcc-src/lib)
