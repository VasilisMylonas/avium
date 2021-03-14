# Include files.

# All include files to be distributed.
set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_DIR}/avium/avium.h
    ${AVM_INCLUDE_DIR}/avium/core.h
    ${AVM_INCLUDE_DIR}/avium/fmt.h
    ${AVM_INCLUDE_DIR}/avium/options.h
    ${AVM_INCLUDE_DIR}/avium/string.h)

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

# Process header config file.
configure_file(${AVM_INCLUDE_DIR}/${CMAKE_PROJECT_NAME}/config.h.in
               ${AVM_INCLUDE_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}/config.h @ONLY)

# Installation.
install(FILES ${AVM_INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME})
