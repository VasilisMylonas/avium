set(AVM_SOURCE_FILES core.c string.c options.c fmt.c)
set(AVM_SHARED_NAME avm)
set(AVM_STATIC_NAME avm-static)

add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
add_link_options(-fprofile-instr-generate -fcoverage-mapping)

add_library(${AVM_SHARED_NAME} SHARED ${AVM_SOURCE_FILES})
add_library(${AVM_STATIC_NAME} STATIC ${AVM_SOURCE_FILES})
add_library(avmtest STATIC testing.c)

target_compile_definitions(${AVM_SHARED_NAME} PRIVATE AVM_EXPORT)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

install(TARGETS ${AVM_STATIC_NAME} ${AVM_SHARED_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
