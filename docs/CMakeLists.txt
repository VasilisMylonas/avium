# Documentation

# Where to place documentation related build files.
set(AVM_DOC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(AVM_IMAGE_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/images)
set(AVM_HTML_OUTPUT_DIR ${AVM_DOC_OUTPUT_DIR}/html)

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

# We use NaturalDocs for documentation.
find_program(AVM_DOC_PROGRAM natural-docs)

if(AVM_DOC_PROGRAM MATCHES "AVM_DOC_PROGRAM-NOTFOUND")
    message(WARNING "natural-docs not found.")
else()
    # Custom 'docs' target for documentation generation.
    add_custom_target(docs ALL COMMAND ${AVM_DOC_PROGRAM} ${AVM_DOC_OUTPUT_DIR})
endif()

# Create required directories.
file(MAKE_DIRECTORY ${AVM_DOC_OUTPUT_DIR})
file(MAKE_DIRECTORY ${AVM_IMAGE_OUTPUT_DIR})
file(MAKE_DIRECTORY ${AVM_HTML_OUTPUT_DIR})

# Process NaturalDocs config files.
configure_file(Project.txt.in Project.txt @ONLY)
configure_file(Comments.txt.in Comments.txt @ONLY)
configure_file(Languages.txt.in Languages.txt @ONLY)

# Installation
install(FILES ${AVM_DOC_FILES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY ${AVM_HTML_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY ${AVM_IMAGE_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})
