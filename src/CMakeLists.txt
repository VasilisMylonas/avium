include(GenerateExportHeader)

add_subdirectory(avm.core)
add_subdirectory(avm.argparse)

if(USE_IO)
    add_subdirectory(avm.io)
endif()

add_library(avm ${LIB_TYPE} module.c)
target_link_libraries(avm avm.core avm.io avm.argparse)

generate_export_header(avm
    BASE_NAME AVM
    EXPORT_MACRO_NAME AVMAPI
    INCLUDE_GUARD_NAME AVIUM_EXPORTS_H
    EXPORT_FILE_NAME ${INCLUDE_OUT_DIR}/avium/exports.h
)

install(TARGETS avm DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
