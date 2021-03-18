# Documentation

# Where to place documentation related build files.
set(AVM_DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(AVM_SPHINX_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/sphinx)
set(AVM_IMAGE_DIR ${CMAKE_SOURCE_DIR}/images)

# Copyright notice, timestamp format and tab width.
set(AVM_PROJECT_COPYRIGHT "Copyright (C) 2021 Vasilis Mylonas")
set(AVM_TIMESTAMP_FORMAT yyyy-mm-dd)
set(AVM_TAB_WIDTH 4)

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
    configure_file(Doxyfile.in Doxyfile @ONLY)
    add_custom_target(Doxygen COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)

    configure_file(conf.py.in xml/conf.py @ONLY)
    configure_file(index.rst.in xml/index.rst @ONLY)
    add_custom_target(Sphinx ALL DEPENDS Doxygen
                      COMMAND ${SPHINX_EXECUTABLE} -q -b html
                      ${AVM_DOC_OUTPUT_DIR}/xml ${AVM_SPHINX_OUTPUT_DIR})
else()
    message(WARNING "Install Doxygen and Sphinx to generate documentation.")
endif()

# Installation
install(DIRECTORY ${AVM_DOC_OUTPUT_DIR}/man/man3 DESTINATION ${CMAKE_INSTALL_MANDIR})
install(DIRECTORY ${AVM_IMAGE_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
