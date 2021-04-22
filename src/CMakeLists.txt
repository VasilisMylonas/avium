include(GenerateExportHeader)

file(GLOB SOURCE_FILES *.c)

add_library(${SHARED_NAME} SHARED ${SOURCE_FILES})
add_library(${STATIC_NAME} STATIC ${SOURCE_FILES})

generate_export_header(avm
    BASE_NAME AVM
    EXPORT_MACRO_NAME AVMAPI
    INCLUDE_GUARD_NAME AVIUM_EXPORTS_H
    EXPORT_FILE_NAME ${INCLUDE_OUT_DIR}/avium/exports.h
)

if(USE_GC)
    target_link_libraries(${STATIC_NAME} ${LIBGC})
endif()

install(TARGETS ${STATIC_NAME} ${SHARED_NAME}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME})
