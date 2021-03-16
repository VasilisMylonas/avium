# Documentation

# Where to place documentation related build files.
set(AVM_DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(AVM_NATURAL_DOCS_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/natural-docs)
set(AVM_DOXYGEN_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/doxygen)
set(AVM_SPHINX_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/sphinx)

file(MAKE_DIRECTORY ${AVM_NATURAL_DOCS_OUTPUT_DIR}/images)
file(MAKE_DIRECTORY ${AVM_NATURAL_DOCS_OUTPUT_DIR}/html)

# Copyright notice, timestamp format and tab width.
set(AVM_PROJECT_COPYRIGHT "Copyright (C) 2021 Vasilis Mylonas")
set(AVM_TIMESTAMP_FORMAT yyyy-mm-dd)
set(AVM_TAB_WIDTH 4)
set(AVM_DOXYGEN_INDEX_FILE ${AVM_DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(AVM_NATURAL_DOCS_INDEX_FILE ${AVM_NATURAL_DOCS_OUTPUT_DIR}/html/index.html)

# Additional files to be included in the documentation.
set(AVM_DOC_FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/CONTRIBUTING.md
    ${CMAKE_SOURCE_DIR}/INSTALL.md
    ${CMAKE_SOURCE_DIR}/AUTHORS
    ${CMAKE_SOURCE_DIR}/COPYING
    ${CMAKE_SOURCE_DIR}/COPYING.LESSER)

find_package(NaturalDocs REQUIRED)
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# Process NaturalDocs and Doxygen config files.
configure_file(Project.txt.in Project.txt @ONLY)
configure_file(Comments.txt.in Comments.txt @ONLY)
configure_file(Languages.txt.in Languages.txt @ONLY)
configure_file(Doxyfile.in Doxyfile @ONLY)
configure_file(conf.py.in conf.py @ONLY)
configure_file(index.rst.in index.rst @ONLY)

# Targets for documentation.
add_custom_target(NaturalDocs COMMAND ${NATURAL_DOCS_EXECUTABLE} .)
add_custom_target(Doxygen COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)
add_custom_target(Sphinx ALL DEPENDS Doxygen
                  COMMAND ${SPHINX_EXECUTABLE} -b html
                  ${AVM_DOC_OUTPUT_DIR} ${AVM_SPHINX_OUTPUT_DIR})

# Installation
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY ${AVM_NATURAL_DOCS_OUTPUT_DIR}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
