include(GenerateExportHeader)

add_subdirectory(core)

if(USE_IO)
    add_subdirectory(io)
endif()

add_library(avm ${LIB_TYPE} options.c value.c)
target_link_libraries(avm avm.io avm.core)

generate_export_header(avm
    BASE_NAME AVM
    EXPORT_MACRO_NAME AVMAPI
    INCLUDE_GUARD_NAME AVIUM_EXPORTS_H
    EXPORT_FILE_NAME ${INCLUDE_OUT_DIR}/avium/exports.h
)

if(USE_GC)
    target_link_libraries(avm gcmt-lib)
endif()

install(TARGETS avm DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
