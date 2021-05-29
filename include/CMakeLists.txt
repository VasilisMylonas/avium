include(CheckTypeSize)
include(CheckIncludeFile)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    message("Building for Linux.")
    set(AVM_LINUX ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    message("Building for Darwin.")
    set(AVM_DARWIN ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL Windows)
    message("Building for Win32.")
    set(AVM_WIN32 ON)
else()
    message(FATAL_ERROR "Unknown platform.")
endif()

if(MSVC)
    set(AVM_MSVC ON)
else()
    set(AVM_GNU ON)
endif()

check_type_size(long AVM_LONG_SIZE)
check_include_file(uchar.h AVM_HAVE_UCHAR_H)
check_include_file(dlfcn.h AVM_HAVE_DLFCN_H)
check_include_file(libtcc.h AVM_HAVE_TCC_H)

if(${AVM_LONG_SIZE} EQUAL 8)
    set(AVM_LONG_TYPE "long")
else()
    set(AVM_LONG_TYPE "long long")
endif()

if(USE_ARGPARSE)
    set(AVM_USE_ARGPARSE ON)
endif()

if(USE_REFLECT)
    set(AVM_USE_REFLECT ON)
endif()

if(USE_IO)
    set(AVM_USE_IO ON)
endif()

configure_file(avium/config.h.in avium/config.h @ONLY)

file(GLOB INCLUDE_FILES avium/*.h)
list(APPEND INCLUDE_FILES ${INCLUDE_OUT_DIR}/avium/config.h)
list(APPEND INCLUDE_FILES ${INCLUDE_OUT_DIR}/avium/exports.h)

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avium)
