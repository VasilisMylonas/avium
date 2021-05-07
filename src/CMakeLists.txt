include(GenerateExportHeader)

add_library(avm module.c)

generate_export_header(avm
    BASE_NAME AVM
    EXPORT_MACRO_NAME AVMAPI
    INCLUDE_GUARD_NAME AVIUM_EXPORTS_H
    EXPORT_FILE_NAME ${INCLUDE_OUT_DIR}/avium/exports.h
)

add_subdirectory(avm.core)
target_link_libraries(avm avm.core)

if(USE_ARGPARSE)
    add_subdirectory(avm.argparse)
    target_link_libraries(avm avm.argparse)
endif()

if(USE_IO)
    add_subdirectory(avm.io)
    target_link_libraries(avm avm.io)
endif()

if(USE_REFLECT)
    add_subdirectory(avm.reflect)
    target_link_libraries(avm avm.reflect)
endif()

install(TARGETS avm DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
