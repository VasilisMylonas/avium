# Include files.

# All include files to be distributed.
set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/exports.h
    avium/array-list.h
    avium/fmt.h
    avium/collection.h
    avium/options.h
    avium/prologue.h
    avium/string.h
    avium/testing.h
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
check_type_size(void* AVM_PTR_SIZE)

if(${AVM_LONG_SIZE} EQUAL 8)
    set(AVM_LONG_TYPE long)
else()
    set(AVM_LONG_TYPE long long)
endif()

if(${AVM_PTR_SIZE} EQUAL 8)
    set(AVM_PTR_TYPE ${AVM_LONG_TYPE})
else()
    set(AVM_PTR_TYPE int)
endif()

# Process header config file.
configure_file(avium/config.h.in avium/config.h @ONLY)

# Installation.
install(FILES ${AVM_INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/avium)
