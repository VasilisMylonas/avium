# Documentation

# Where to place documentation related build files.
set(AVM_DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(AVM_SPHINX_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/sphinx)
set(AVM_IMAGE_DIR ${CMAKE_SOURCE_DIR}/images)

# Additional files to be included in the documentation.
set(AVM_DOC_FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/CONTRIBUTING.md
    ${CMAKE_SOURCE_DIR}/INSTALL.md
    ${CMAKE_SOURCE_DIR}/AUTHORS
    ${CMAKE_SOURCE_DIR}/COPYING
    ${CMAKE_SOURCE_DIR}/COPYING.LESSER)

find_package(Doxygen)
find_package(Sphinx)

if(DOXYGEN_FOUND AND SPHINX_FOUND)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C TRUE)
    set(DOXYGEN_GENERATE_HTML FALSE)
    set(DOXYGEN_GENERATE_LATEX FALSE)
    set(DOXYGEN_GENERATE_XML TRUE)
    set(DOXYGEN_GENERATE_MAN TRUE)
    set(DOXYGEN_TYPEDEF_HIDES_STRUCT TRUE)
    set(DOXYGEN_OUTPUT_DIRECTORY ${AVM_DOC_OUTPUT_DIR})
    set(DOXYGEN_RECURSIVE TRUE)
    set(DOXYGEN_MACRO_EXPANSION TRUE)
    set(DOXYGEN_EXPAND_ONLY_PREDEF TRUE)
    set(DOXYGEN_PREDEFINED AVMAPI=extern
            static_assert=static_assert
            static_assert_s=static_assert
            AVM_PTR_TYPE=long AVM_LONG_TYPE=long)

    doxygen_add_docs(Doxygen ${AVM_INCLUDE_DIR})

    configure_file(conf.py.in xml/conf.py @ONLY)
    configure_file(index.rst xml/index.rst @ONLY)
    add_custom_target(Sphinx DEPENDS Doxygen
                      COMMAND ${SPHINX_EXECUTABLE} -q -b html
                      ${AVM_DOC_OUTPUT_DIR}/xml ${AVM_SPHINX_OUTPUT_DIR})
else()
    message(WARNING "Install Doxygen and Sphinx to generate documentation.")
endif()

# Installation
install(DIRECTORY ${AVM_DOC_OUTPUT_DIR}/man/man3 DESTINATION ${CMAKE_INSTALL_MANDIR})
install(DIRECTORY ${AVM_IMAGE_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
