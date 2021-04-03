# Compilation

# All source files to be compiled.
set(AVM_SOURCE_FILES
    array.c
    collection.c
    fmt.c
    options.c
    string.c
    typeinfo.c
    types.c
    value.c
    version.c)

# Target names.
set(AVM_SHARED_NAME avm)
set(AVM_STATIC_NAME avm-static)

# Add the libraries.
add_library(${AVM_SHARED_NAME} SHARED ${AVM_SOURCE_FILES})
add_library(${AVM_STATIC_NAME} STATIC ${AVM_SOURCE_FILES})

generate_export_header(${AVM_SHARED_NAME} BASE_NAME AVM
                       EXPORT_MACRO_NAME AVMAPI
                       INCLUDE_GUARD_NAME AVIUM_EXPORTS_H
                       EXPORT_FILE_NAME ${AVM_INCLUDE_OUTPUT_DIR}/avium/exports.h)

if(MSVC)
    # Disable some MSVC warnings.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Installation.
install(TARGETS ${AVM_STATIC_NAME} ${AVM_SHARED_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
