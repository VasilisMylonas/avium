set(AVM_INCLUDE_FILES
    ${AVM_INCLUDE_OUTPUT_DIR}/avium/config.h
    ${AVM_INCLUDE_DIR}/avium/avium.h
    ${AVM_INCLUDE_DIR}/avium/core.h
    ${AVM_INCLUDE_DIR}/avium/fmt.h
    ${AVM_INCLUDE_DIR}/avium/options.h
    ${AVM_INCLUDE_DIR}/avium/string.h
    ${AVM_INCLUDE_DIR}/avium/testing.h)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(AVM_LINUX ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(AVM_DARWIN ON)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(AVM_WIN32 ON)
endif()

if(MSVC)
    set(AVM_MSVC ON)
else()
    set(AVM_GNU ON)
endif()

configure_file(${AVM_INCLUDE_DIR}/${CMAKE_PROJECT_NAME}/config.h.in
               ${AVM_INCLUDE_OUTPUT_DIR}/${CMAKE_PROJECT_NAME}/config.h @ONLY)

install(FILES ${AVM_INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME})
