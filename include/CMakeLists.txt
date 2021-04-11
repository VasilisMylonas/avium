# Include files.

# All include files to be distributed.
set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/exports.h
    avium/alloc.h
    avium/array-list.h
    avium/array.h
    avium/fmt.h
    avium/optional.h
    avium/options.h
    avium/runtime.h
    avium/result.h
    avium/string.h
    avium/testing.h
    avium/types.h
    avium/value.h
    avium/version.h)

# Platform definitions.
if (CMAKE_SYSTEM_NAME MATCHES Linux)
    message("Building for Linux.")
    set(AVM_LINUX ON)
elseif(CMAKE_SYSTEM_NAME MATCHES Darwin)
    message("Building for Darwin.")
    set(AVM_DARWIN ON)
elseif (CMAKE_SYSTEM_NAME MATCHES Windows)
    message("Building for Win32.")
    set(AVM_WIN32 ON)
else()
    message(FATAL_ERROR "Unknown platform.")
endif()

# Compiler definitions.
if(MSVC)
    set(AVM_MSVC ON)
else()
    set(AVM_GNU ON)
endif()

include(CheckTypeSize)

check_type_size(long AVM_LONG_SIZE)

if(${AVM_LONG_SIZE} EQUAL 8)
    set(AVM_LONG_TYPE long)
else()
    set(AVM_LONG_TYPE long long)
endif()

# Process header config file.
configure_file(avium/config.h.in avium/config.h @ONLY)

# Installation.
install(FILES ${AVM_INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avium)
