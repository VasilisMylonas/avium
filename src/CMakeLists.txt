# Compilation

# All source files to be compiled.
set(AVM_SOURCE_FILES core.c string.c options.c fmt.c)

# Target names.
set(AVM_SHARED_NAME avm)
set(AVM_STATIC_NAME avm-static)

# Coverage.
add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
add_link_options(-fprofile-instr-generate -fcoverage-mapping)

# Add the libraries.
add_library(${AVM_SHARED_NAME} SHARED ${AVM_SOURCE_FILES})
add_library(${AVM_STATIC_NAME} STATIC ${AVM_SOURCE_FILES})

# When building a shared library, export symbols.
target_compile_definitions(${AVM_SHARED_NAME} PRIVATE AVM_EXPORT)

if(MSVC)
    # Disable some MSVC warnings.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Installation.
install(TARGETS ${AVM_STATIC_NAME} ${AVM_SHARED_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
